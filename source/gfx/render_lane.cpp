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
    surface.init(resolution, pass != RenderPass::Shadow, true);
}

void RenderLane::resize(IntSize resolution)
{
    if (surface.resolution != resolution)
    {
        RenderSystem& render_system = Single::Get<RenderSystem>();
        render_system.get_command_queue().Flush();

        surface.resize(resolution);
        camera.AspectRatio = static_cast<float>(resolution.width) / resolution.height;
        camera.calcProjectionMatrix();
    }
}

RenderSurface& RenderLane::getSurface()
{
    return surface;
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

    surface.startRendering(commandList);

    RenderContext context(render_system, *commandList.listImpl.Get());
    scene.render(context, &camera, pass);
    surface.endRendering(commandList);

    commandList.endRecording();
    commandQueue.execute(commandList);

    fenceValue = commandQueue.fence->SignalNext();
}
