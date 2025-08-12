module AssetDesc;

import Texture;
import ShaderSourceFile;
import Model;

void AssetDesc::create_asset()
{
    auto extension = virtual_path.getExtension();

    if (extension == ".png")
    {
        loaded_asset = MakePtr<Texture>(virtual_path);
    }
    else if (extension == ".inc" || extension == ".hlsl")
    {
        loaded_asset = MakePtr<ShaderSourceFile>(virtual_path);
    }
    else if (extension == ".obj")
    {
        loaded_asset = MakePtr<Model>(virtual_path);
    }
    else
    {
        throw "unknown asset extension";
    }
}
