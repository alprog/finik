export module SpriteManager;

import Finik.Core;
import Assets;
import AssetPath;
import Texture;
import Sprite3D;

export class SpriteManager : public Singleton<SpriteManager>
{
public:
    std::shared_ptr<Sprite3D> get(AssetPath path)
    {
        auto ptr = sprites.find_value(path);
        if (ptr)
        {
            return *ptr;
        }

        std::shared_ptr asset = Assets::GetInstance().get_asset(path); 
        if (!asset)
        {
            return nullptr;
        }

        auto sprite = std::make_shared<Sprite3D>(asset);
        sprites[path] = sprite;
        return sprite;
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
        for (auto& [_, sdf] : sprites)
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
    HashMap<AssetPath, std::shared_ptr<Sprite3D>> sprites;
    bool hotReloadNeeded = false;
};
