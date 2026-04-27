#include <Windows.h>

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

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    initTimer();

    App& app = App::GetInstance();

    DesktopWindow* window = app.desktop_system.create_window(1024, 800);

    Scene& scene = app.scene_manager.create_scene();
    window->scene = &scene;

    window->gui->views.append(MakeUnique<ConsoleView>("consoleView"));
    window->gui->views.append(MakeUnique<SceneView>("sceneView1", scene));
    //window->gui->views.append(MakeUnique<SceneView>("sceneView2", scene));
    window->gui->views.append(MakeUnique<TextureView>("textureView", scene));

    SceneView* sceneView = (SceneView*)window->gui->views[1].get();
    window->gui->views.append(MakeUnique<StatsView>("statsView", *sceneView));

    window->gui->views.append(MakeUnique<ProfilerView>("profilerView"));

    window->gui->views.append(MakeUnique<AssetsView>("assetsView"));
    window->gui->views.append(MakeUnique<ShadersView>("shadersView"));

    window->gui->views.append(MakeUnique<QualityView>("qualityView"));

    //auto secondWindow = app.desktop_system.create_window(800, 600);
    //secondWindow->scene = &scene;

    app.run_game_loop();

    return 0;
}
