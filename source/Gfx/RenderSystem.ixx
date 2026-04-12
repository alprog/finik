export module RenderSystem;

export import RenderSystemFwd;

export import Finik.Core.Singleton;

import Finik.Core;
import DX;

import RenderContext;
import Execution;
import Myptr;
import OneshotAllocator;
import RootSignature;
import Descriptors;
import PipelineState;
import GfxDevice;

export class RenderSystem : public Singleton<RenderSystem>
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
    void createCommandListPool();
    void createCommandAllocators();
    void createCommandList();
    void createRenderContext();
    void createProfiler();
    void createRootSignature();

    GfxDevice device; 

    CommandQueue* commandQueue = nullptr;
    MyPtr<ID3D12CommandAllocator> commandAllocators[3];
    MyPtr<ID3D12GraphicsCommandList> commandList;

    UniquePtr<MainRootSignature> mainRootSignature;
    UniquePtr<ComputeRootSignature> computeRootSignature;

    GpuProfiler* gpuProfiler = nullptr;

    UniquePtr<RenderContext> renderContext;

    Ptr<PipelineState> pipelineState;

    UniquePtr<CommandListPool> commandListPool;
};