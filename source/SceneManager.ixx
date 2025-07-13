export module scene_manager;

import Finik.Core;

import scene_system_fwd;

export class SceneManager
{
    friend class App;

    SceneManager();

public:
    void update(float deltaTime);
    Scene& create_scene();

public:
    Array<Scene*> scenes;
};