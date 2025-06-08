export module mesh;

import core;
import vertex;
import vertex_buffer;
import index_buffer;

export class Mesh
{
public:
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
};

export Mesh* createFullScreenQuad();
export Mesh* createCubeMesh();
