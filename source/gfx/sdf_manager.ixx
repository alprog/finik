export module sdf_manager;

import core;
import asset_path;
import texture;
import sdf;

export class SDFManager : public Singleton<SDFManager>
{
public:
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