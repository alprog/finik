export module Model;

import Finik.Core;
import Asset;
import ObjLoader;
import Mesh;
import MeshBuilder;
import Vertex;
import Assets;
import ShaderSourceFile;
import Material;
import EffectManager;

export class Model : public Asset
{
public:
    using Asset::Asset;

    Vector3 rub_to_rfu(const Vector3 v)
    {
        return {v.x, -v.z, v.y};
    }

    void hot_reload(ByteBlob& blob) override
    {
        auto mtllibReader = [&](String relPath) -> String {
            AssetPath path = Path::combine(assetPath.getParentPath(), relPath);
            auto mtllib = Assets::GetInstance().get<ShaderSourceFile>(path);
            return mtllib ? mtllib->GetSourceText() : "";
        };
        
        ObjLoader loader;
        loader.load(blob.asString(), mtllibReader);

        bool sameHand = true;

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
                    vertex.position = rub_to_rfu(loader.positions[desc.pi]);
                    vertex.normal = rub_to_rfu(loader.normals[desc.ni]);
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
