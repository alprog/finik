module;
class Window;
export module RenderSystem;

export import RenderSystemFwd;

export import Finik.Core.Singleton;

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

    UniquePtr<MainRootSignature> mainRootSignature;
    UniquePtr<ComputeRootSignature> computeRootSignature;

    GpuProfiler* gpuProfiler = nullptr;

    UniquePtr<RenderContext> renderContext;

    UniquePtr<DescriptorHeap> rtvHeap; // Render Target View
    UniquePtr<DescriptorHeap> dsvHeap; // Depth Stencil View
    UniquePtr<DescriptorHeap> srvCbvUavHeap;

    MyPtr<ID3D12Device> device;

    Ptr<PipelineState> pipelineState;

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

    UniquePtr<CommandListPool> commandListPool;
    UniquePtr<finik::gpumem::OneshotAllocator> oneshotAllocator;
};
