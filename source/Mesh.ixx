export module Mesh;

import Finik.Core;
import Vertex;
import VertexBuffer;
import IndexBuffer;

export class Mesh
{
public:
    StandardVertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
};

export Mesh* createCubeMesh();
export Mesh* createFullScreenQuad();
export Mesh* createBodyQuad();
export Mesh* createBodyMesh();
