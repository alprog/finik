module render_lane;

import render_system;
import camera;
import timebox_tracker;
import command_list;
import scene;
import mrt;
import enum_bits;

RenderLane::RenderLane(Scene& scene, RenderPass pass, Camera& camera, IntSize resolution, MSAA msaa)
    : scene{scene}
    , pass{pass}
    , camera{camera}
    , frameBuffer{resolution, msaa, pass == RenderPass::Shadow ? 0 : 4, true}
{
}

void RenderLane::resize(IntSize resolution, MSAA msaa)
{
    if (frameBuffer.resolution != resolution || frameBuffer.msaa != msaa)
    {
        RenderSystem& render_system = Single::Get<RenderSystem>();
        render_system.get_command_queue().Flush();

        frameBuffer.resize(resolution, msaa);
        camera.AspectRatio = static_cast<float>(resolution.width) / resolution.height;
        camera.calcProjectionMatrix();
    }
}

FrameBuffer& RenderLane::getFrameBuffer()
{
    return frameBuffer;
}

void RenderLane::render()
{
    RenderSystem& render_system = Single::Get<RenderSystem>();
    auto& commandQueue = render_system.get_command_queue();
    {
        Profile _("wait");
        commandQueue.fence->WaitForValue(fenceValue);
    }

    CommandList& commandList = render_system.getFreeCommandList();
    commandList.startRecording();

    frameBuffer.startRendering(commandList);

    RenderContext context(render_system, *commandList.listImpl.Get());
    scene.render(context, &camera, pass);
    frameBuffer.endRendering(commandList);

    commandList.endRecording();
    commandQueue.execute(commandList);

    fenceValue = commandQueue.fence->SignalNext();
}

//------------------------------------------------

SceneRenderLane::SceneRenderLane(Scene& scene, Camera& camera, IntSize resolution, MSAA msaa)
    : scene{scene}
    , camera{camera}
    , gBuffer{resolution, msaa, 4, true}
    , lightBuffer{resolution, msaa, 1, false}
{
}

void SceneRenderLane::resize(IntSize resolution, MSAA msaa)
{
    if (gBuffer.resolution != resolution || gBuffer.msaa != msaa)
    {
        RenderSystem& render_system = Single::Get<RenderSystem>();
        render_system.get_command_queue().Flush();

        gBuffer.resize(resolution, msaa);
        camera.AspectRatio = static_cast<float>(resolution.width) / resolution.height;
        camera.calcProjectionMatrix();
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
