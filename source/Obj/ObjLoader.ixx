export module ObjLoader;

import AssetPath;
import Finik.Core;

export struct VertexIndices
{
    int pi;
    int ti;
    int ni;
};

export using Face = Array<VertexIndices>;

export struct Mtl
{
    Mtl(String name)
        : name{name}
    {
    }

    String name;
    Color diffuse;

    Array<Face> faces;
};

export class ObjLoader
{
public:
    void load(const String& text, const std::function<String(String)>& mtllibReader)
    {
        Array<String> lines = text.split("\n");

        positions.append({});
        normals.append({});
        tex_coords.append({});

        int32 CurrentMaterialIndex = -1;

        Array<Face>* activeFaces = &this->faces;

        for (auto& line : lines)
        {
            auto arr = line.get_trimmed().split(" ");
            if (arr.empty())
            {
                continue;
            }

            if (arr[0] == "v")
            {
                auto x = std::stof(arr[1]);
                auto y = std::stof(arr[2]);
                auto z = std::stof(arr[3]);
                positions.emplace_back(x, y, z);
            }

            if (arr[0] == "vn")
            {
                auto x = std::stof(arr[1]);
                auto y = std::stof(arr[2]);
                auto z = std::stof(arr[3]);
                normals.emplace_back(x, y, z);
            }

            if (arr[0] == "vt")
            {
                auto u = std::stof(arr[1]);
                auto v = std::stof(arr[2]);
                tex_coords.emplace_back(u, v);
            }

            if (arr[0] == "f")
            {
                auto& face = activeFaces->emplace_back();
                for (int i = 1; i < arr.count(); i++)
                {
                    auto components = arr[i].split("/");
                    auto extractIndex = [&components](int32 index) {
                        if (components.count() < index || components[index].empty())
                        {
                            return 0;
                        }
                        return std::stoi(components[index]);
                    };

                    face.emplace_back(extractIndex(0), extractIndex(1), extractIndex(2));
                }
            }

            if (arr[0] == "mtllib")
            {
                const String& text = mtllibReader(arr[1]);
                loadMtllib(text);
            }

            if (arr[0] == "usemtl")
            {
                activeFaces = &this->faces;
                for (auto& mtl : mtls)
                {
                    if (mtl.name == arr[1])
                    {
                        activeFaces = &mtl.faces;
                        break;
                    }
                }
            }

            if (arr[0] == "#meta")
            {
                if (arr.count() > 2)
                {
                    meta[arr[1]] = arr[2];
                }
            }
        }
    }

    void loadMtllib(const String& text)
    {
        Array<String> lines = text.split("\n");

        for (auto& line : lines)
        {
            auto arr = line.get_trimmed().split(" ");
            if (arr.empty())
            {
                continue;
            }

            if (arr[0] == "newmtl")
            {
                mtls.emplace_back(arr[1]);
            }

            if (arr[0] == "Kd")
            {
                auto r = std::stof(arr[1]);
                auto g = std::stof(arr[2]);
                auto b = std::stof(arr[3]);
                mtls.last().diffuse = {r, g, b};
            }
        }
    }

    String getMeta(String key, String fallback)
    {
        auto ptr = meta.find_value(key);
        return ptr ? *ptr : fallback;
    }

public:
    Array<Vector3> positions;
    Array<Vector3> normals;
    Array<Vector2> tex_coords;
    Array<Mtl> mtls;
    Array<Face> faces;
    HashMap<String, String> meta;
};
