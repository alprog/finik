module SceneManager;

import Scene;

SceneManager::SceneManager()
{
}

void SceneManager::update(float deltaTime)
{
    for (auto scene : scenes)
    {
        scene->update(deltaTime);
    }
}

Scene& SceneManager::create_scene()
{
    auto scene = new Scene();
    scenes.append(scene);
    return *scene;
}
