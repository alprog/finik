export module Sdf;

import Finik.Core;
import Texture;
import Asset;
import AssetDependencies;
import Images;

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

