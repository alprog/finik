export module VertexBuffer;

import Finik.Core;
import DX;
import Vertex;

export class BaseVertexBuffer
{
public:
    void Load();
    
    virtual int GetVertexSize() const = 0;
    virtual int GetCount() const = 0;
    virtual void* GetData() = 0;
        
    MyPtr<ID3D12Resource> vertexBufferResource;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
};

export template <typename VertexT = StandardVertex> 
class VertexBuffer : public BaseVertexBuffer
{
public:
    int GetVertexSize() const override { return sizeof(VertexT); }
    int GetCount() const override { return vertices.count(); }
    void* GetData() override { return &vertices[0]; }

    Array<VertexT> vertices;
};

export using StandardVertexBuffer = VertexBuffer<StandardVertex>;
export using LineVertexBuffer = VertexBuffer<LineVertex>;