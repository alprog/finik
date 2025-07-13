export module upload_buffer;

import Finik.Core;
import dx;
import render_system_fwd;

export class UploadBuffer
{
public:
    UploadBuffer(RenderSystem& renderSystem, int size);
    ~UploadBuffer();

    int GetSize() const;
    void* GetData() const;
    ID3D12Resource* GetResource() const;
    D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const;
   
private:
    int Size;
    ID3D12Resource* InternalResource;
    void* Data;
};