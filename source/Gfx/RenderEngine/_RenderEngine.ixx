export module RenderEngine;

export import :Shared;
export import :CommandList;
export import :CommandListPool;
export import :Fence;
export import :CommandQueue;
export import :GpuProfiler;

import RootSignature;

export class RenderEngine
{
public:
    void init();

    GfxDevice& getDevice();
    CommandQueue& get_command_queue();
    DescriptorHeap* getRtvHeap();
    DescriptorHeap* getDsvHeap();
    DescriptorHeap* getCommonHeap();
    GpuProfiler* getProfiler();
    MainRootSignature& getRootSignature();
    ComputeRootSignature& getComputeRootSignature();
    CommandList& getFreeCommandList();
    finik::gpumem::OneshotAllocator& getOneshotAllocator();

    int32 getCompletedFrameIndex();

    void scheduleQueryResolving();
    void signalEndFrame();

private:
    void createDevice();
    void createCommandQueue();
    void createProfiler();
    void createCommandListPool();
    void createRootSignature();

private:
    GfxDevice device;
    CommandQueue* commandQueue = nullptr;
    UniquePtr<CommandListPool> commandListPool;
    GpuProfiler* gpuProfiler = nullptr;

    UniquePtr<MainRootSignature> mainRootSignature;
    UniquePtr<ComputeRootSignature> computeRootSignature;
};

