export module SceneManager;

import Finik.Core;

import SceneSystemFwd;

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
