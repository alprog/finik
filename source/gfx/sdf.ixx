export module sdf;

import Finik.Core;
import texture;
import asset;
import asset_dependencies;
import images;

export class SDF
{
public:
    SDF(std::shared_ptr<Asset> asset);
    void rebuild();

private:
    void calcSDF(std::shared_ptr<Image> image);

public:
    std::shared_ptr<Texture> texture;
    AssetDependencies hotreloadDependencies;
};

