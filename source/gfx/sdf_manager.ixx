export module sdf_manager;

import core;
import assets;
import asset_path;
import texture;
import sdf;

export class SDFManager : public Singleton<SDFManager>
{
public:
    std::shared_ptr<Texture> get(AssetPath path)
    {
        auto ptr = sdfs.find_value(path);
        if (ptr)
        {
            return ptr->get()->texture;
        }

        std::shared_ptr asset = Assets::GetInstance().get_asset(path); 
        if (!asset)
        {
            return nullptr;
        }

        auto sdf = std::make_shared<SDF>(asset);
        sdfs[path] = sdf;
        return sdf->texture;
    }

    void update()
    {
        if (hotReloadNeeded)
        {
            hotReloadOutdated();
            hotReloadNeeded = false;
        }
    }

    void onTextureFileChanged()
    {
        hotReloadNeeded = true;
    }

private:
    int32 hotReloadOutdated()
    {
        int32 count = 0;
        for (auto& [_, sdf] : sdfs)
        {
            if (sdf->hotreloadDependencies.isOutdated())
            {
                sdf->rebuild();
                count++;
            }
        }
        return count;
    }

private:
    HashMap<AssetPath, std::shared_ptr<SDF>> sdfs;
    bool hotReloadNeeded = false;
};