export module Assets;

import Finik.Core;
import Texture;
import AssetPath;
import ShaderSourceFile;
import AssetBundle;
import AssetFolder;
import AssetDesc;
import ByteBlob;
import Asset;

export class Assets : public Singleton<Assets>
{
    friend class AssetsView;

public:
    Assets()
    {
        Path working_directory = toStr(std::filesystem::current_path().c_str());
        AssetDirectory = working_directory / "assets";

        mount_folder(working_directory / "assets");
        mount_folder(working_directory / "mods");

        update();
    }

    ~Assets()
    {
        for (auto p_bundle : bundles)
        {
            delete p_bundle;
        }
    }

    void update()
    {
        for (auto p_bundle : bundles)
        {
            p_bundle->update();
        }

        for (auto p_bundle : bundles)
        {
            sync_bundle(*p_bundle);
        }

        if (need_hot_reload)
        {
            hot_reload();
        }
    }

    void hot_reload()
    {
        bool success = true;
        for (auto& [_, desc] : asset_descs)
        {
            if (desc.need_reload())
            {
                success |= desc.try_reload();
            }
        }
        need_hot_reload = !success;
    }

    void sync_bundle(AssetBundle& bundle)
    {
        bool changed = false;
        if (!bundle.synced)
        {
            for (auto& [path, status] : bundle.entries)
            {
                switch (status)
                {
                case AssetStatus::Added:
                {
                    auto it = asset_descs.find_value(path);
                    if (!it)
                    {
                        // add fully new
                        asset_descs[path] = AssetDesc(path, &bundle);
                    }
                    else
                    {
                        if (bundle.priority > it->actual_bundle->priority)
                        {
                            // override existing
                            it->actual_bundle = &bundle;
                            it->version++;
                            need_hot_reload = true;
                        }
                    }
                    status = AssetStatus::Synced;
                    break;
                }

                case AssetStatus::Modified:
                {
                    auto desc = asset_descs.find_value(path);
                    if (desc && desc->actual_bundle == &bundle)
                    {
                        desc->version++;
                        need_hot_reload = true;
                    }
                    status = AssetStatus::Synced;
                    break;
                }

                case AssetStatus::Removing:
                    auto desc = asset_descs.find_value(path);
                    if (desc && desc->actual_bundle == &bundle)
                    {
                        desc->actual_bundle = nullptr;
                        for (int32 index = bundle.priority - 1; index >= 0; index--)
                        {
                            if (bundles[index]->has(path))
                            {
                                desc->actual_bundle = bundles[index];
                                desc->version++;
                                need_hot_reload = true;
                                break;
                            }
                        }
                        if (!desc->actual_bundle && !desc->is_loaded())
                        {
                            asset_descs.remove(path);
                        }
                    }
                    break;
                }
            }

            bundle.entries.remove_if([](auto& pair) { return pair.second == AssetStatus::Removing; });

            bundle.synced = true;
        }
    }

    void mount_folder(Path folder_path)
    {
        auto folder = new AssetFolder(folder_path);
        bundles.append(folder);
        refresh_bundle_priorities();
    }

    void unmount_folder(Path folder_path)
    {
        bundles.remove_if([folder_path](auto& p_bundle)
            {
                auto asset_folder = dynamic_cast<AssetFolder*>(p_bundle);
                return asset_folder && asset_folder->get_folder_path() == folder_path;;
            });
        refresh_bundle_priorities();
    }

    bool exists(AssetPath path)
    {
        return asset_descs.contains(path);
    }

    bool is_loaded(AssetPath path)
    {
        auto it = asset_descs.find_value(path);
        if (it)
        {
            return it->is_loaded();
        }
        return false;
    }

    Ptr<Asset> get_asset(AssetPath path)
    {
        auto it = asset_descs.find_value(path);
        if (it)
        {
            return it->get_asset();
        }
        return nullptr;
    }

    template <typename T>
    Ptr<T> get(AssetPath path)
    {
        return std::dynamic_pointer_cast<T, Asset>(get_asset(path));
    }

private:
    void refresh_bundle_priorities()
    {
        int32 index = 0;
        for (auto& bundle : bundles)
        {
            bundle->priority = index++;
        }
    }

private:
    HashMap<AssetPath, AssetDesc> asset_descs;
    Array<AssetBundle*> bundles;

    Path AssetDirectory;
    bool need_hot_reload = false;
};
