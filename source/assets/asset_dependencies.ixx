export module asset_dependencies;

import core;
import asset;

export struct AssetDependencies
{
    void add(std::shared_ptr<Asset> asset)
    {
        dependencies.emplace_back(asset, asset->get_version());
    }

    bool empty() const
    {
        return dependencies.empty();
    }

    bool isOutdated()
    {
        for (auto& [asset, version] : dependencies)
        {
            if (asset->get_version() != version)
            {
                return true;
            }
        }
        return false;
    }

    void actualize()
    {
        for (auto& [asset, version] : dependencies)
        {
            version = asset->get_version();
        }
    }

    int32 count() const
    {
        return dependencies.count();
    }

    const auto& getDependency(int32 index) const 
    { 
        return dependencies[index];
    }

private:
    Array<std::pair<std::shared_ptr<Asset>, int32>> dependencies;
};