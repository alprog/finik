export module mesh;

import Finik.Core;
import vertex;
import vertex_buffer;
import index_buffer;

export class Mesh
{
public:
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
};

export Mesh* createCubeMesh();
export Mesh* createFullScreenQuad();
export Mesh* createBodyQuad();
export Mesh* createBodyMesh();
