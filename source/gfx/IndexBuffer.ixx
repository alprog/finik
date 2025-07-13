export module index_buffer;

import Finik.Core;
import dx;

export class IndexBuffer
{
public:
    void Load();

    Array<unsigned int> indices;
    MyPtr<ID3D12Resource> indexBufferResource;
    D3D12_INDEX_BUFFER_VIEW indexBufferView;
};
