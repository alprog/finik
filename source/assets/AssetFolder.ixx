module;
#include "Windows.h"
export module AssetFolder;

import Finik.Core;
import AssetBundle;
import AssetPath;

struct DirectoryChangeWatcher
{
    DirectoryChangeWatcher(Path path)
    {
        constexpr int32 flags = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE;
        change_handle = FindFirstChangeNotificationA(path.c_str(), true, flags);
    }

    bool has_changes()
    {
        bool changed = false;
        while (WaitForSingleObject(change_handle, 0) == WAIT_OBJECT_0)
        {
            changed = true;
            FindNextChangeNotification(change_handle);
        }

        return changed;
    }

    HANDLE change_handle;
};

struct FileInfo
{
    AssetPath relative_path;
    std::filesystem::file_time_type time;

    auto operator<=>(const FileInfo& other) const { return relative_path <=> other.relative_path; }
};

export class AssetFolder : public AssetBundle
{
public:
    AssetFolder(Path folder_path)
        : folder_path{ folder_path }
        , watcher{ folder_path }
    {
        for (auto& info : get_folder_files())
        {
            entries[info.relative_path] = AssetStatus::Added;
            times[info.relative_path] = info.time;
        }
    }

    void update() override
    {
        // detecting if any changes by OS watcher (efficient)
        if (watcher.has_changes())
        {
            // updating by polling filesystem and check file dates (non-efficient, but rare)

            auto files = get_folder_files();
            for (auto& info : files)
            {
                auto it = entries.find_value(info.relative_path);
                if (!it)
                {
                    entries[info.relative_path] = AssetStatus::Added;
                    times[info.relative_path] = info.time;
                    synced = false;
                }
                else if (update_time(info.relative_path, info.time))
                {
                    *it = AssetStatus::Modified;
                    synced = false;
                }
            }

            if (entries.count() > files.count())
            {
                files.sort();
                files.emplace_back(); // sentinel
                auto it = files.begin();
                for (auto& [path, status] : entries)
                {
                    if (path == it->relative_path)
                    {
                        it++;
                    }
                    else
                    {
                        status = AssetStatus::Removing;
                        synced = false;
                    }
                }
            }
        }
    }

    Path get_folder_path() const
    {
        return folder_path;
    }

    ByteBlob get(AssetPath asset_path) const override
    {
        return { Path::combine(folder_path, asset_path) };
    }

private:
    Array<FileInfo> get_folder_files()
    {
        Array<FileInfo> result;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(folder_path.str()))
        {
            if (entry.is_regular_file())
            {
                const Path file_path = entry.path();
                const AssetPath relative_path = file_path.getRelativeTo(folder_path);
                const auto time = std::filesystem::last_write_time(file_path.c_str());
                result.emplace_back(relative_path, time);
            }
        }
        return result;
    }

    bool update_time(AssetPath asset_path, std::filesystem::file_time_type new_time)
    {
        auto& time = times[asset_path];
        if (time != new_time)
        {
            time = new_time;
            return true;
        }
        return false;
    }

private:
    Path folder_path;
    SortedMap<AssetPath, std::filesystem::file_time_type> times;
    DirectoryChangeWatcher watcher;
};
