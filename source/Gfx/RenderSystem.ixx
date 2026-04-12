export module RenderSystem;

export import Finik.Core.Singleton;

import GfxDevice;
import RenderContext;
export import RenderEngine;
import RootSignature;

export class RenderSystem : public Singleton<RenderSystem>
{
public:
    RenderSystem();

public:
    void init();

    GfxDevice& getDevice();
    ID3D12Device* getInternalDevice();

    RenderContext* getRenderContext();
    GpuProfiler* getProfiler();
    MainRootSignature& getRootSignature();
    ComputeRootSignature& getComputeRootSignature();

    void ImguiInitHelper();

    void scheduleQueryResolving();

private:
    void createRenderContext();

public:
    RenderEngine engine;
    UniquePtr<RenderContext> renderContext;  
};