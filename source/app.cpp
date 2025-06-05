module;
#include <SDL.h>
#include <SDL_syswm.h>
module app;

import render_system;
import swap_chain;
import render_lane;
import imgui;
import timebox_tracker;
import desktop_window;
import assets;
import shader_manager;
import gui;
import cleaner;

App& App::GetInstance()
{
    static App instance;
    return instance;
}

App::App()
    : desktop_system{}
    , input_system{}
    , scene_manager{}
    , profiler{}
{
}

void App::handle_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        auto window = desktop_system.get_window_by_id(event.window.windowID);
        if (window)
        {
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                desktop_system.close_window(window);
            }
            else
            {
                window->gui->set_context();
            }
        }

        if (ImGui::GetCurrentContext())
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
        }
    }
}

void App::run_game_loop()
{
    RenderSystem& render_system = Single::Get<RenderSystem>();

    profiler.start();

    while (true)
    {
        Profile _("frame");

        float deltaTime = profiler.getDeltaTime();

        Assets::GetInstance().update();
        ShaderManager::GetInstance().update();
        Cleaner::GetInstance().update();

        auto completedValue = render_system.get_command_queue().fence->GetCompletedValue();
        render_system.get_command_queue().freeCompletedLists();
        render_system.getOneshotAllocator().FreePages();
        render_system.getProfiler()->grabReadyStamps(completedValue);

        {
            Profile _("input");
            handle_input();
        }

        if (App::GetInstance().desktop_system.windows.empty())
        {
            break;
        }

        {
            Profile _("update");
            scene_manager.update(deltaTime);
            for (auto window : desktop_system.windows)
            {
                window->gui->set_context();
                for (auto& view : window->gui->views)
                {
                    view->update(deltaTime);
                }
            }
        }

        {
            Profile _("gui prepare");
            for (auto window : desktop_system.windows)
            {
                window->gui->set_context();
                window->gui->prepare();
            }
        }

        {
            Profile _("lanes");
            for (auto& lane : render_system.lanes)
            {
                lane->render();
            }
        }

        {
            Profile _("render windows");
            for (auto window : desktop_system.windows)
            {
                auto command_list = render_system.get_command_list();
                window->swap_chain->start_frame(command_list);
                //window->renderScene();
                window->gui->render(command_list);
                window->swap_chain->finish_frame(command_list);
                window->swap_chain->execute(command_list);
                window->swap_chain->present();
            }
        }

        {
            Profile _("render platform windows");
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        }

        render_system.scheduleQueryResolving();

        auto signaled = render_system.get_command_queue().frameFence->SignalNext();
        //assert(signaled == profiler.getFrameIndex());

        profiler.endFrame();
    }

    //render_system.WaitForLastSubmittedFrame();
}

int App::getFrameIndex()
{
    return profiler.getFrameIndex();
}