export module Scene;

import Effect;

import Finik.Core;
import Actor;
import Grid;
import Texture;
import Light;
import RenderSystem;
import RenderPass;
import Character;
import RenderContext;
import Camera;

export class Scene
{
public:
    Scene();

    void update(float deltaTime);
    void renderShadowMaps(CommandList& commandList, RenderContext& context, Camera& camera);
    void render(RenderContext& context, const Camera& camera, const Matrix& prevViewProjection, const Vector2& prevJitter, RenderPass pass);
    void debugRender(RenderContext& context, const Camera& camera, const Matrix& prevViewProjection, const Vector2& prevJitter);

    Grid* grid;
    Array<Actor*> actors;
    Array<Character*> characters;

    Effect renderState;
    Effect renderState2;

    Ray hoverRay;
    Vector3 castedPos;

    Light light;
};
