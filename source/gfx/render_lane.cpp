module render_lane;

import render_system;
import camera;
import timebox_tracker;
import command_list;
import scene;
import mrt;
import enum_bits;

SceneRenderLane::SceneRenderLane(Scene& scene, Camera& camera, SurfaceSize size)
    : scene{scene}
    , camera{camera}
    , size{size}
    , gBuffer{size, 4, true}
    , lightBuffer{size, 1, false}
{
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
    RenderSystem& render_system = Single::Get<RenderSystem>();
    auto& commandQueue = render_system.get_command_queue();
    {
        Profile _("wait");
        commandQueue.fence->WaitForValue(fenceValue);
    }

    CommandList& commandList = render_system.getFreeCommandList();
    commandList.startRecording();

    RenderContext context(render_system, *commandList.listImpl.Get());

    gBuffer.startRendering(commandList);
    scene.render(context, &camera, RenderPass::Geometry);
    gBuffer.endRendering(commandList);

    lightBuffer.startRendering(commandList);
    // light pass
    lightBuffer.endRendering(commandList);

    commandList.endRecording();
    commandQueue.execute(commandList);

    fenceValue = commandQueue.fence->SignalNext();
}
