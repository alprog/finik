export module Sprite3D;

import Finik.Core;
import Texture;
import Asset;
import AssetDependencies;
import Images;
import Mesh;

export class Sprite3D
{
public:
    Sprite3D(std::shared_ptr<Asset> asset);
    void rebuild();

private:
    void createMesh(std::shared_ptr<Image> image);

public:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> texture;

    AssetDependencies hotreloadDependencies;
};

