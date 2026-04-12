export module UploadBuffer;

import Finik.Core;
import GfxDevice;

export class UploadBuffer
{
public:
    UploadBuffer(GfxDevice& device, int size);
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
