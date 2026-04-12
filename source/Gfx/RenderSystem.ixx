export module RenderSystem;

export import RenderSystemFwd;

export import Finik.Core.Singleton;

import GfxDevice;
import RenderContext;
export import RenderEngine;
import RootSignature;

export extern "C++" class RenderSystem : public Singleton<RenderSystem>
{
public:
    RenderSystem();

public:
    void init();

    GfxDevice& getDevice();
    ID3D12Device* getInternalDevice();
    CommandQueue& get_command_queue();
    ID3D12GraphicsCommandList* get_command_list();
    DescriptorHeap* getRtvHeap();
    DescriptorHeap* getDsvHeap();
    DescriptorHeap* getCommonHeap();
    RenderContext* getRenderContext();
    GpuProfiler* getProfiler();
    MainRootSignature& getRootSignature();
    ComputeRootSignature& getComputeRootSignature();

    CommandList& getFreeCommandList();
    finik::gpumem::OneshotAllocator& getOneshotAllocator();

    void ImguiInitHelper();

    void scheduleQueryResolving();

private:
    void createDevice();
    void createCommandQueue();
    void createProfiler();
    void createCommandListPool();
    void createCommandAllocators();
    void createCommandList();
    void createRenderContext();
    void createRootSignature();

    RenderEngine engine; 

    CommandQueue* commandQueue = nullptr;
    MyPtr<ID3D12CommandAllocator> commandAllocators[3];
    MyPtr<ID3D12GraphicsCommandList> commandList;

    UniquePtr<MainRootSignature> mainRootSignature;
    UniquePtr<ComputeRootSignature> computeRootSignature;

    GpuProfiler* gpuProfiler = nullptr;

    UniquePtr<RenderContext> renderContext;

    UniquePtr<CommandListPool> commandListPool;
};