export module Model;

import Finik.Core;
import Asset;
import ObjLoader;
import Mesh;
import MeshBuilder;
import Vertex;
import Assets;
import Material;
import EffectManager;
import TextFile;

export class Model : public Asset
{
public:
    using Asset::Asset;

    void hot_reload(ByteBlob& blob) override
    {
        auto mtllibReader = [&](String relPath) -> String {
            AssetPath path = Path::combine(assetPath.getParentPath(), relPath);
            auto mtllib = Assets::GetInstance().get<TextFile>(path);
            return mtllib ? mtllib->GetFileText() : "";
        };
        
        ObjLoader loader;
        loader.load(blob.asString(), mtllibReader);

        String coordinates = loader.getMeta("coordinates", "RUB");

        Matrix3x3 axesChangeMatrix =
            coordinates == "unchanged" ? Matrix3x3::Identity :            
            CoordinateSystem::World.getConversionMatrixFrom(coordinates.c_str()); 

        bool sameHand = axesChangeMatrix.determinant() > 0;

        for (auto& mtl : loader.mtls)
        {
            if (mtl.faces.empty())
            {
                continue;
            }

            auto material = new Material(mtl.name);
            material->Colors.append(mtl.diffuse);
            material->RefreshBuffer();
            material->Effect = EffectManager::GetInstance().get("standard");
            material->ShadowEffect = EffectManager::GetInstance().getShadowEffect(*material->Effect);
            materials.append(material);
        
            MeshBuilder builder;
            for (auto& face : mtl.faces)
            {
                Array<StandardVertex> vertices;
                vertices.resize(face.count());
                int32 index = sameHand ? 0 : face.count();
                for (auto& desc : face)
                {
                    StandardVertex& vertex = vertices[sameHand ? index++ : --index];
                    vertex.position = loader.positions[desc.pi] * axesChangeMatrix;
                    vertex.normal = loader.normals[desc.ni] * axesChangeMatrix;
                    vertex.texCoord = loader.tex_coords[desc.ti];
                }
                builder.addTriangleFan(vertices);
            }
            meshes.emplace_back(builder.Build());
        }
    }

    Array<Mesh*> meshes;
    Array<Material*> materials;
};
