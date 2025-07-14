#include "SDL.h"

import Finik.Core;
import App;
import Timer;
import SceneView;
import ConsoleView;
import TextureView;
import StatsView;
import ProfilerView;
import Scene;
import Gui;
import DesktopWindow;
import DesktopSystem;
import AssetsView;
import ShadersView;
import Assets;
import Model;
import ShaderManager;
import QualityView;

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
