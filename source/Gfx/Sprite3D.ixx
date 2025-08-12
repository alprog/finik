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
    Sprite3D(Ptr<Asset> asset);
    void rebuild();

private:
    void createMesh(Ptr<Image> image);

public:
    Ptr<Mesh> mesh;
    Ptr<Texture> texture;

    AssetDependencies hotreloadDependencies;
};

