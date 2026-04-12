export module ConstantBuffer;

import Finik.Core;
import Descriptors;
import GfxDevice;
import UploadBuffer;

export class GpuDataBuffer
{
public:
    GpuDataBuffer(int32 size, GfxDevice& device);
    virtual ~GpuDataBuffer();

    UploadBuffer* uploadBuffer;
    DescriptorHandle descriptorHandle;
    uint8* data;
    int32 size;
};

export template <typename T>
class GpuBuffer : public GpuDataBuffer
{
    using Base = GpuDataBuffer;

public:
    GpuBuffer(GfxDevice& device)
        : GpuDataBuffer(sizeof(T), device)
    {
        data = reinterpret_cast<T*>(Base::data);
    }

    T* data;

    T* GetData()
    {
        return data;
    }

    T* operator->()
    {
        return data;
    }
};
