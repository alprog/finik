module asset_desc;

import texture;
import shader_source_file;
import model;

void AssetDesc::create_asset()
{
    auto extension = virtual_path.getExtension();

    if (extension == ".png")
    {
        loaded_asset = std::make_shared<Texture>(virtual_path);
    }
    else if (extension == ".inc" || extension == ".hlsl")
    {
        loaded_asset = std::make_shared<ShaderSourceFile>(virtual_path);
    }
    else if (extension == ".obj")
    {
        loaded_asset = std::make_shared<Model>(virtual_path);
    }
    else
    {
        throw "unknown asset extension";
    }
}