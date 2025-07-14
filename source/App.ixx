export module App;

import Finik.Core;
import RenderSystem;
import Profiler;
import SceneManager;
import InputSystem;
import DesktopSystem;

// for intellisense

export class App
{
public:
    static App& GetInstance();

private:
    App();

public:
    void run_game_loop();
    int getFrameIndex();

private:
    void handle_input();

public:
    DesktopSystem desktop_system;
    InputSystem input_system;
    SceneManager scene_manager;

    finik::profiler::Profiler profiler;
};
