module;
#include <SDL3/SDL.h>
module App;

import RenderSystem;
import SwapChain;
import RenderLane;
import Imgui;
import TimeboxTracker;
import DesktopWindow;
import Assets;
import ShaderManager;
import Gui;
import Cleaner;
import SpriteManager;

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
            // Main Finik Engine Windows (separate gui contexts)
            window->processEvent(event);
        }
        else
        {
            // ImGui Undocked Windows (part of some main context)
            for (auto window : desktop_system.windows)
            {
                window->gui->set_context();
                ImGui_ImplSDL3_ProcessEvent(&event);
            }
        }
    }
}

void App::run_game_loop()
{
    auto& engine = Single::Get<RenderSystem>().engine;

    profiler.start();

    while (true)
    {
        Profile _("frame");

        float deltaTime = profiler.getDeltaTime();

        Assets::GetInstance().update();
        ShaderManager::GetInstance().update();
        SpriteManager::GetInstance().update();
        Cleaner::GetInstance().update();

        auto completedValue = (int32)engine.get_command_queue().fence->GetCompletedValue();
        engine.get_command_queue().freeCompletedLists();
        engine.getOneshotAllocator().FreePages();
        engine.getProfiler()->grabReadyStamps(completedValue);

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
            Profile _("render windows");
            for (auto window : desktop_system.windows)
            {
                window->gui->set_context();

                auto& list = engine.getFreeCommandList();
                window->swap_chain->start_frame(list);
                window->gui->render(list);
                window->swap_chain->finish_frame(list);
                window->swap_chain->execute(list);
                {
                    Profile _("present");                    
                    window->swap_chain->present();
                }
                
            }
        }

        for (auto window : desktop_system.windows)
        {
            Profile _("render platform windows");
            window->gui->set_context();
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        }

        engine.scheduleQueryResolving();
        engine.signalEndFrame();

        auto signaled = engine.get_command_queue().frameFence->SignalNext();
        if (signaled != profiler.getFrameIndex())
            throw;

        profiler.endFrame();
    }

    //render_system.WaitForLastSubmittedFrame();
}

int App::getFrameIndex()
{
    return profiler.getFrameIndex();
}
