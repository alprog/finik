export module Model;

import Finik.Core;
import Asset;
import ObjLoader;
import Mesh;
import MeshBuilder;
import Vertex;

export class Model : public Asset
{
public:
    using Asset::Asset;

    Vector3 rub_to_fru(const Vector3 v)
    {
        return {-v.z, v.x, v.y};
    }

    void hot_reload(ByteBlob& blob) override
    {
        ObjLoader loader;
        loader.load(blob.asString());

        MeshBuilder builder;
        for (auto& face : loader.faces)
        {
            Array<StandardVertex> vertices;
            vertices.resize(face.count());
            int32 index = face.count();
            for (auto& desc : face)
            {
                StandardVertex& vertex = vertices[--index];
                vertex.position = rub_to_fru(loader.positions[desc.pi]);
                vertex.normal = rub_to_fru(loader.normals[desc.ni]);
                vertex.texCoord = loader.tex_coords[desc.ti];
            }
            builder.addTriangleFan(vertices);
        }

        mesh = builder.Build();
    }

    Mesh* mesh = nullptr;
};
