#include "SDL.h"

import core;
import app;
import timer;
import scene_view;
import console_view;
import texture_view;
import stats_view;
import profiler_view;
import scene;
import gui;
import desktop_window;
import desktop_system;
import assets_view;
import shaders_view;
import assets;
import model;
import shader_manager;
import quality_view;

int main(int argc, char* argv[])
{
    initTimer();

    App& app = App::GetInstance();

    Scene& scene = app.scene_manager.create_scene();
    DesktopWindow* window = app.desktop_system.create_window(1024, 800);

    window->scene = &scene;

    window->gui->views.append(std::make_unique<ConsoleView>("consoleView"));
    window->gui->views.append(std::make_unique<SceneView>("sceneView1", scene));
    //window->gui->views.append(std::make_unique<SceneView>("sceneView2", scene));
    window->gui->views.append(std::make_unique<TextureView>("textureView", scene));

    SceneView* sceneView = (SceneView*)window->gui->views[1].get();
    window->gui->views.append(std::make_unique<StatsView>("statsView", *sceneView));

    window->gui->views.append(std::make_unique<ProfilerView>("profilerView"));

    window->gui->views.append(std::make_unique<AssetsView>("assetsView"));
    window->gui->views.append(std::make_unique<ShadersView>("shadersView"));

    window->gui->views.append(std::make_unique<QualityView>("qualityView"));

    //auto secondWindow = app.desktop_system.create_window(800, 600);
    //secondWindow->scene = &scene;

    app.run_game_loop();

    return 0;
}