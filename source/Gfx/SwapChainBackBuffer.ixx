export module SwapChainBackBuffer;

import Finik.Core;
import DX;
export import GpuResource;
export import Descriptors;

export class SwapChainBackBuffer : public GpuResource
{
public:
    SwapChainBackBuffer();
    virtual ~SwapChainBackBuffer();

    void set(IDXGISwapChain3& swapChain, int32 bufferIndex);

    DescriptorHandle descriptorHandle;
};