export module ConstantBuffer;

import Finik.Core;
import DescriptorHandle;
import RenderSystemFwd;
import RootSignatureParams;

export class GpuDataBuffer
{
public:
    GpuDataBuffer(int32 size, RenderSystem& renderSystem);
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
    GpuBuffer(RenderSystem& renderSystem)
        : GpuDataBuffer(sizeof(T), renderSystem)
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

export using FrameConstantBuffer = GpuBuffer<FrameConstants>;
