export module asset_path;

import Finik.Core;

export class AssetPath : public Path
{
    using Path::Path;
};

export template<>
struct std::hash<AssetPath>
{
    size_t operator()(const AssetPath& path) const
    {
        return std::hash<std::string>()(path.str());
    }
};