export module SceneManager;

import Finik.Core;

import Scene;

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
