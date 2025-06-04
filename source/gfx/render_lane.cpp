module render_lane;

import render_system;
import camera;
import timebox_tracker;
import command_list;
import scene;

RenderLane::RenderLane(Scene& scene, RenderPass pass, Camera& camera, IntSize resolution)
    : scene{scene}
    , pass{pass}
    , camera{camera}
{
    frameBuffer.init(resolution, pass != RenderPass::Shadow, true);
}

void RenderLane::resize(IntSize resolution)
{
    if (frameBuffer.resolution != resolution)
    {
        RenderSystem& render_system = Single::Get<RenderSystem>();
        render_system.get_command_queue().Flush();

        frameBuffer.resize(resolution);
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
