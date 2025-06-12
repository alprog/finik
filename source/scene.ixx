export module scene;

import effect;
import scene_system_fwd;

import core;
import actor;
import grid;
import texture;
import light;
import render_system_fwd;
import render_pass;

// for intellisense

export class Scene
{
public:
    Scene();

    void update(float deltaTime);
    void renderShadowMaps(CommandList& commandList, RenderContext& context, Camera& camera);
    void render(RenderContext& context, Camera& camera, RenderPass pass);

    Grid* grid;
    Array<Actor*> actors;

    Effect renderState;
    Effect renderState2;

    Ray hoverRay;
    Vector3 castedPos;

    Light light;
};