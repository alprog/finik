export module RenderEngine:RenderEngine;

import :Shared;
import RootSignature;

export class RenderEngine
{
public:
    void init();

    GfxDevice& getDevice();
    ID3D12Device* getInternalDevice();
    CommandQueue& get_command_queue();
    ID3D12GraphicsCommandList* get_command_list();
    DescriptorHeap* getRtvHeap();
    DescriptorHeap* getDsvHeap();
    DescriptorHeap* getCommonHeap();
    GpuProfiler* getProfiler();
    MainRootSignature& getRootSignature();
    ComputeRootSignature& getComputeRootSignature();
    CommandList& getFreeCommandList();
    finik::gpumem::OneshotAllocator& getOneshotAllocator();

    void scheduleQueryResolving();

private:
    void createDevice();
    void createCommandQueue();
    void createProfiler();
    void createCommandListPool();
    void createCommandAllocators();
    void createCommandList();
    void createRootSignature();

private:
    GfxDevice device;
    CommandQueue* commandQueue = nullptr;
    MyPtr<ID3D12CommandAllocator> commandAllocators[3];
    MyPtr<ID3D12GraphicsCommandList> commandList;
    UniquePtr<CommandListPool> commandListPool;
    GpuProfiler* gpuProfiler = nullptr;

    UniquePtr<MainRootSignature> mainRootSignature;
    UniquePtr<ComputeRootSignature> computeRootSignature;
};