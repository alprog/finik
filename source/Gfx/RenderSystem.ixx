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

    RenderContext* getRenderContext();
    MainRootSignature& getRootSignature();
    ComputeRootSignature& getComputeRootSignature();

    void ImguiInitHelper();

private:
    void createRenderContext();

public:
    RenderEngine engine;
    UniquePtr<RenderContext> renderContext;  
};