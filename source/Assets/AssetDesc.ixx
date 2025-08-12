export module AssetDesc;

import Finik.Core;
import Asset;
import AssetPath;
import AssetBundle;
import ByteBlob;

export struct AssetDesc
{
    AssetDesc() = default;

    AssetDesc(AssetPath virtual_path, AssetBundle* bundle)
        : virtual_path { virtual_path }
        , actual_bundle{ bundle }
        , version { 0 }
    {
    }

    Ptr<Asset> get_asset()
    {
        if (!is_loaded())
        {
            create_asset();
            try_reload();
        }
        return loaded_asset;
    }

    void create_asset();

    bool try_reload()
    {
        if (actual_bundle)
        {
            ByteBlob blob = actual_bundle->get(virtual_path);
            if (!blob.empty())
            {
                loaded_asset->hot_reload(blob, version);
                return true;
            }
        }
        return false;
    }
       
    bool is_loaded() const
    {
        return loaded_asset != nullptr;
    }

    bool need_reload() const
    {
        return loaded_asset && loaded_asset->get_version() != version;
    }

    std::strong_ordering operator<=>(AssetDesc& another)
    {
        return virtual_path <=> another.virtual_path;
    }

    AssetPath virtual_path;
    
    int32 version;
    AssetBundle* actual_bundle;

    Ptr<Asset> loaded_asset;
};
