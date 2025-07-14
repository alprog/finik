module;
class Window;
export module RenderSystem;

export import RenderSystemFwd;

export import Singleton;

import Finik.Core;
import DX;
import CommandQueue;
import CommandListPool;
import RenderLane;
import RenderContext;
import GpuProfiler;
import Myptr;
import OneshotAllocator;
import RootSignature;
import DescriptorHeap;

// for intellisense

export class RenderSystem
{
    friend class Single;

    RenderSystem();

public:
    void init();

    ID3D12Device* get_device();
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
    void enableDebugLayer();
    void createDevice();
    void setupDebug();
    void createCommandQueue();
    void createDescriptorHeap();
    void createCommandListPool();
    void createCommandAllocators();
    void createOneshotAllocator();
    void createCommandList();
    void createRenderContext();
    void createProfiler();
    void createRootSignature();

    CommandQueue* commandQueue = nullptr;
    MyPtr<ID3D12CommandAllocator> commandAllocators[3];
    MyPtr<ID3D12GraphicsCommandList> commandList;

    std::unique_ptr<MainRootSignature> mainRootSignature;
    std::unique_ptr<ComputeRootSignature> computeRootSignature;

    GpuProfiler* gpuProfiler = nullptr;

    std::unique_ptr<RenderContext> renderContext;

    std::unique_ptr<DescriptorHeap> rtvHeap; // Render Target View
    std::unique_ptr<DescriptorHeap> dsvHeap; // Depth Stencil View
    std::unique_ptr<DescriptorHeap> srvCbvUavHeap;

    MyPtr<ID3D12Device> device;

    std::shared_ptr<PipelineState> pipelineState;

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

    std::unique_ptr<CommandListPool> commandListPool;
    std::unique_ptr<finik::gpumem::OneshotAllocator> oneshotAllocator;
};
