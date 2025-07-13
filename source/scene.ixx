export module scene;

import effect;
import scene_system_fwd;

import Finik.Core;
import actor;
import grid;
import texture;
import light;
import render_system_fwd;
import render_pass;
import character;

// for intellisense

export class Scene
{
public:
    Scene();

    void update(float deltaTime);
    void renderShadowMaps(CommandList& commandList, RenderContext& context, Camera& camera);
    void render(RenderContext& context, const Camera& camera, const Matrix& prevViewProjection, const Vector2& prevJitter, RenderPass pass);

    Grid* grid;
    Array<Actor*> actors;
    Array<Character*> characters;

    Effect renderState;
    Effect renderState2;

    Ray hoverRay;
    Vector3 castedPos;

    Light light;
};