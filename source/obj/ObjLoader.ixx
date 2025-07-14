export module ObjLoader;

import Finik.Core;

struct VertexIndices
{
    int pi;
    int ti;
    int ni;
};

using Face = Array<VertexIndices>;

export class ObjLoader
{
public:
    void load(String text)
    {
        Array<String> lines = text.split("\n");

        positions.append({});
        normals.append({});
        tex_coords.append({});

        for (auto& line : lines)
        {
            auto arr = line.get_trimmed().split(" ");
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
                auto& face = faces.emplace_back();
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

            // if (arr[0] == "mtllib")
            //{
            //     var directoryPath = new FileInfo(objPath).Directory.FullName;
            //     var mtlPath = Path.Combine(directoryPath, arr[1]);
            //     LoadMaterialLibrary(mtlPath);
            // }

            // if (arr[0] == "usemtl")
            //{
            //     CurrentMaterial = Materials[arr[1]];
            // }
        }

        return;
    }

public:
    Array<Vector3> positions;
    Array<Vector3> normals;
    Array<Vector2> tex_coords;
    Array<Face> faces;
};
