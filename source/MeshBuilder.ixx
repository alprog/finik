export module MeshBuilder;

import Finik.Core;
import Vertex;
import Mesh;

export class MeshBuilder
{
public:
    Array<StandardVertex> Vertices;
    Array<uint32> Indices;

    int32 addVertex(const StandardVertex& vertex);
    void addIndex(int32 index);

    void addTriangle(const StandardVertex& a, const StandardVertex& b, const StandardVertex& c);
    void addQuad(const StandardVertex& a, const StandardVertex& b, const StandardVertex& c, const StandardVertex& d);
    void addTriangleFan(const Array<StandardVertex>& fanVertices);

    void AddTile(int x, int y);
    Mesh* Build();
};
