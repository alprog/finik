module render_lane;

import render_system;
import camera;
import timebox_tracker;
import command_list;
import scene;
import mrt;
import enum_bits;
import root_signature_params;
import effect_manager;
import app;
import jitter;

SceneRenderLane::SceneRenderLane(Scene& scene, Camera& camera, SurfaceResolution resolution)
    : scene{scene}
    , camera{camera}
    , resolution{resolution}
    , gBuffer{resolution}
    , lightBuffer{resolution, {TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM}, true}
    , prevViewProjection{Matrix::Identity}
    , historyBuffer{resolution, {TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM}, false}
    , resolvedBuffer{resolution, {TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM}, false}
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
        lightBuffer.resize(resolution);
        historyBuffer.resize(resolution);
        resolvedBuffer.resize(resolution);

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

void SceneRenderLane::render()
{
    std::swap(resolvedBuffer, historyBuffer);

    auto frameIndex = App::GetInstance().getFrameIndex();
    camera.Jitter = GetJitter(resolution.resolution(), frameIndex);
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
        scene.render(context, camera, prevViewProjection, RenderPass::Geometry);
        gBuffer.endRendering(commandList);

        prevViewProjection = camera.viewMatrix * camera.projectionMatrix;
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

    {
        auto taaConstants = renderSystem.getOneshotAllocator().Allocate<TAAConstants>();
        taaConstants->LightBufferId = lightBuffer.getRenderSurface(MRT::RT0)->textureHandle.getIndex();
        taaConstants->HistoryBufferId = historyBuffer.getRenderSurface(MRT::RT0)->textureHandle.getIndex();
        commandList.getRenderContext().commandList.SetGraphicsRootConstantBufferView(MainRootSignature::Params::MeshConstantBufferView, taaConstants.GpuAddress);
        
        resolvedBuffer.startRendering(commandList);
        context.setEffect(*EffectManager::GetInstance().get("taa_resolve"));
        context.drawMesh(fullscreenQuad);
        resolvedBuffer.endRendering(commandList);
    }

    commandList.endRecording();
    commandQueue.execute(commandList);

    fenceValue = commandQueue.fence->SignalNext();
}
