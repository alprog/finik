module mesh_builder;

import mesh;
import vertex_buffer;
import index_buffer;

int32 MeshBuilder::addVertex(const StandardVertex& vertex)
{
    Vertices.append(vertex);
    return Vertices.last_index();
}

void MeshBuilder::addIndex(int32 index)
{
    Indices.append(index);
}

void MeshBuilder::addTriangle(const StandardVertex& a, const StandardVertex& b, const StandardVertex& c)
{
    addIndex(addVertex(a));
    addIndex(addVertex(b));
    addIndex(addVertex(c));
}

void MeshBuilder::addQuad(const StandardVertex& a, const StandardVertex& b, const StandardVertex& c, const StandardVertex& d)
{
    addTriangleFan({a, b, c, d});
}

void MeshBuilder::addTriangleFan(const Array<StandardVertex>& fanVertices)
{
    if (fanVertices.count() == 3)
    {
        addTriangle(fanVertices[0], fanVertices[1], fanVertices[2]);
        return;
    }

    auto startIndex = Vertices.count();
    for (auto& vertex : fanVertices)
    {
        addVertex(vertex);
    }

    for (int32 i = 2; i < fanVertices.count(); i++)
    {
        addIndex(startIndex);
        addIndex(startIndex + i - 1);
        addIndex(startIndex + i);
    }
}

void MeshBuilder::AddTile(int x, int y)
{
    auto vertexCount = Vertices.count();

    Vertices.append({Vector3(x + 0, y + 0, 0), Vector3(0, 0, 1), Vector2(0, 0), x, y});
    Vertices.append({Vector3(x + 1, y + 0, 0), Vector3(0, 0, 1), Vector2(1, 0), x, y});
    Vertices.append({Vector3(x + 0, y + 1, 0), Vector3(0, 0, 1), Vector2(0, 1), x, y});
    Vertices.append({Vector3(x + 1, y + 1, 0), Vector3(0, 0, 1), Vector2(1, 1), x, y});

    int i = vertexCount;
    Indices.append(i + 0);
    Indices.append(i + 1);
    Indices.append(i + 2);

    Indices.append(i + 2);
    Indices.append(i + 1);
    Indices.append(i + 3);
}

Mesh* MeshBuilder::Build()
{
    Mesh* mesh = new Mesh();

    mesh->vertexBuffer = new VertexBuffer();
    mesh->vertexBuffer->vertices = Vertices;
    mesh->vertexBuffer->Load();

    mesh->indexBuffer = new IndexBuffer();
    mesh->indexBuffer->indices = Indices;
    mesh->indexBuffer->Load();

    return mesh;
}