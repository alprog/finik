module RenderLane;

import RenderSystem;
import Camera;
import TimeboxTracker;
import CommandList;
import Scene;
import Mrt;
import Finik.Core.EnumBits;
import RootSignatureParams;
import EffectManager;
import App;
import Jitter;
import QualityManager;

SceneRenderLane::SceneRenderLane(Scene& scene, Camera& camera, SurfaceResolution resolution)
    : scene{scene}
    , camera{camera}
    , resolution{resolution}
    , gBuffer{resolution}
    , lightBuffer{resolution, {TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM}, true}
    , prevViewProjection{Matrix::Identity}
    , historyBuffer{resolution, {TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM}, false}
    , resolvedBuffer{resolution, {TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM}, false}
    , debugBuffer{resolution, {TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM}, false}
{
    fullscreenQuad = createFullScreenQuad();
}

void SceneRenderLane::resize(SurfaceResolution resolution)
{
    if (this->resolution != resolution)
    {
        RenderSystem& render_system = Single::Get<RenderSystem>();
        render_system.get_command_queue().Flush();

        gBuffer.resize(resolution);

        SurfaceResolution colorResolution{resolution.width, resolution.height, 1};
        lightBuffer.resize(colorResolution);
        historyBuffer.resize(colorResolution);
        resolvedBuffer.resize(colorResolution);
        debugBuffer.resize(colorResolution);

        camera.AspectRatio = static_cast<float>(resolution.width) / resolution.height;
        camera.calcProjectionMatrix();

        this->resolution = resolution;
    }
}

FrameBuffer& SceneRenderLane::getGBuffer()
{
    return gBuffer;
}

FrameBuffer& SceneRenderLane::getLightBuffer()
{
    return lightBuffer;
}

FrameBuffer& SceneRenderLane::getHistoryBuffer()
{
    return historyBuffer;
}

FrameBuffer& SceneRenderLane::getResolveBuffer()
{
    return resolvedBuffer;
}

FrameBuffer& SceneRenderLane::getDebugBuffer()
{
    return debugBuffer;
}

void SceneRenderLane::render()
{
    std::swap(resolvedBuffer, historyBuffer);

    auto frameIndex = App::GetInstance().getFrameIndex();
    bool taaEnabled = QualityManager::GetInstance().getCurrent().taa;
    camera.Jitter = taaEnabled ? GetJitter(resolution.resolution(), frameIndex) : Vector2::Zero;
    camera.calcProjectionMatrix();

    RenderSystem& renderSystem = Single::Get<RenderSystem>();
    auto& commandQueue = renderSystem.get_command_queue();
    {
        Profile _("wait");
        commandQueue.fence->WaitForValue(fenceValue);
    }

    CommandList& commandList = renderSystem.getFreeCommandList();
    commandList.startRecording();

    RenderContext context(renderSystem, *commandList.listImpl.Get());

    scene.renderShadowMaps(commandList, context, camera);

    {
        gBuffer.startRendering(commandList);
        scene.render(context, camera, prevViewProjection, prevJitter, RenderPass::Geometry);
        gBuffer.endRendering(commandList);

        prevViewProjection = camera.viewMatrix * camera.projectionMatrix;
        prevJitter = camera.Jitter;
    }

    {
        auto gBufferConstants = renderSystem.getOneshotAllocator().Allocate<GBufferConstants>();
        gBufferConstants->Resolution = gBuffer.resolution;
        gBufferConstants->RT0Id = gBuffer.getRenderSurface(MRT::RT0)->textureHandle.getIndex();
        gBufferConstants->RT1Id = gBuffer.getRenderSurface(MRT::RT1)->textureHandle.getIndex();
        gBufferConstants->RT2Id = gBuffer.getRenderSurface(MRT::RT2)->textureHandle.getIndex();
        gBufferConstants->RT3Id = gBuffer.getRenderSurface(MRT::RT3)->textureHandle.getIndex();
        gBufferConstants->DSId = gBuffer.getRenderSurface(MRT::DS)->textureHandle.getIndex();
        context.setGBufferConstants(gBufferConstants.GpuAddress);

        lightBuffer.startRendering(commandList);

        auto& light = scene.light;
        auto lightConstants = renderSystem.getOneshotAllocator().Allocate<LightConstants>();
        lightConstants->LightDirection = light.direction;
        lightConstants->ShadowViewProjection = light.shadowCamera.viewMatrix * light.shadowCamera.projectionMatrix;
        lightConstants->ShadowTextureId = light.shadowMap->depthStencil->textureHandle.getIndex();

        commandList.getRenderContext().commandList.SetGraphicsRootConstantBufferView(MainRootSignature::Params::MeshConstantBufferView, lightConstants.GpuAddress);
        
        context.setEffect(*EffectManager::GetInstance().get("directional_light"));
        context.drawMesh(fullscreenQuad);
        lightBuffer.endRendering(commandList);
    }

    if (QualityManager::GetInstance().getCurrent().taa)
    {
        auto taaConstants = renderSystem.getOneshotAllocator().Allocate<TAAConstants>();
        taaConstants->LightBufferId = lightBuffer.getRenderSurface(MRT::RT0)->textureHandle.getIndex();
        taaConstants->HistoryBufferId = historyBuffer.getRenderSurface(MRT::RT0)->textureHandle.getIndex();
        commandList.getRenderContext().commandList.SetGraphicsRootConstantBufferView(MainRootSignature::Params::MeshConstantBufferView, taaConstants.GpuAddress);
        
        resolvedBuffer.startRendering(commandList);
        context.setEffect(*EffectManager::GetInstance().get("taa_resolve"));
        context.drawMesh(fullscreenQuad);
        resolvedBuffer.endRendering(commandList);

        debugBuffer.startRendering(commandList);
        context.setEffect(*EffectManager::GetInstance().get("taa_resolve_debug"));
        context.drawMesh(fullscreenQuad);
        debugBuffer.endRendering(commandList);
    }

    commandList.endRecording();
    commandQueue.execute(commandList);

    fenceValue = commandQueue.fence->SignalNext();
}
