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

SceneRenderLane::SceneRenderLane(Scene& scene, Camera& camera, SurfaceSize size)
    : scene{scene}
    , camera{camera}
    , size{size}
    , gBuffer{size, 4, true}
    , lightBuffer{size, 1, true}
{
    fullscreenQuad = createFullScreenQuad();
}

void SceneRenderLane::resize(SurfaceSize size)
{
    if (this->size != size)
    {
        RenderSystem& render_system = Single::Get<RenderSystem>();
        render_system.get_command_queue().Flush();

        gBuffer.resize(size);
        lightBuffer.resize(size);

        camera.AspectRatio = static_cast<float>(size.width) / size.height;
        camera.calcProjectionMatrix();

        this->size = size;
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

void SceneRenderLane::render()
{
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

    gBuffer.startRendering(commandList);
    scene.render(context, camera, RenderPass::Geometry);
    gBuffer.endRendering(commandList);

    auto gBufferConstants = renderSystem.getOneshotAllocator().Allocate<GBufferConstants>();
    gBufferConstants->RT0Id = gBuffer.gerRenderSurface(MRT::RT0)->textureHandle.getIndex();
    gBufferConstants->RT1Id = gBuffer.gerRenderSurface(MRT::RT1)->textureHandle.getIndex();
    gBufferConstants->RT2Id = gBuffer.gerRenderSurface(MRT::RT2)->textureHandle.getIndex();
    gBufferConstants->RT3Id = gBuffer.gerRenderSurface(MRT::RT3)->textureHandle.getIndex();
    gBufferConstants->DSId = gBuffer.gerRenderSurface(MRT::DS)->textureHandle.getIndex();
    context.setGBufferConstants(gBufferConstants.GpuAddress);

    lightBuffer.startRendering(commandList);
    context.setEffect(*EffectManager::GetInstance().get("directional_light"));
    context.drawMesh(fullscreenQuad);
    lightBuffer.endRendering(commandList);

    commandList.endRecording();
    commandQueue.execute(commandList);

    fenceValue = commandQueue.fence->SignalNext();
}
