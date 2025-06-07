module scene;

import render_system;
import effect;
import mesh;
import texture;
import actor;
import shader;
import grid;
import camera;
import oneshot_allocator;
import allocation;
import effect_manager;
import root_signature_params;
import render_context;
import assets;
import model;

// for intellisense

Scene::Scene()
    //: shadowMaps{*this}
{
    grid = new Grid();

    actors.append(new Actor());
    actors.append(new Actor());

    actors[1]->mesh = Assets::GetInstance().get<Model>("models/airplane.obj")->mesh;

    light.direction = Vector4(-1, -1, -1, 0).getNormalized();
}

void Scene::update(float deltaTime)
{
    static float angle = 0;
    angle += deltaTime;

    light.direction = light.direction * Matrix::RotationZ(deltaTime);

    actors[0]->transformMatrix = Matrix::Translation(Vector3(castedPos.x, castedPos.y, 0.0f));
    actors[1]->transformMatrix = Matrix::Translation(Vector3(32, 32, 5));
}

void Scene::render(RenderContext& renderContext, Camera* camera, RenderPass pass)
{
    renderContext.setupRoot();

    RenderSystem& renderSystem = renderContext.renderSystem;
    auto frameConstants = renderSystem.getOneshotAllocator().Allocate<FrameConstants>();
    auto V = camera->viewMatrix;
    auto P = camera->projectionMatrix;
    frameConstants->ViewProjection = V * P;
    frameConstants->LightDirection = light.direction;

    if (pass == RenderPass::Geometry)
    {
        // temp code, replace it
        Camera shadowCamera;
        shadowCamera.FieldOfView = std::numbers::pi / 4.0f;
        shadowCamera.lookAt = camera->lookAt;
        shadowCamera.position = shadowCamera.lookAt - light.direction.xyz() * 100;
        shadowCamera.calcViewMatrix();
        shadowCamera.calcProjectionMatrix();
        frameConstants->ShadowViewProjection = shadowCamera.viewMatrix * shadowCamera.projectionMatrix;
        frameConstants->ShadowTextureId = this->shadowTextureId;
    }

    renderContext.setFrameConstants(frameConstants.GpuAddress);

    //----------------------

    for (auto& actor : actors)
    {
        renderContext.setModelMatrix(actor->transformMatrix);
        renderContext.setMaterial(*actor->material, pass);
        renderContext.drawMesh(actor->mesh);
    }

    //----------------------

    renderContext.setModelMatrix(Matrix::Identity);
    renderContext.setMaterial(*grid->material, pass);
    renderContext.drawMesh(grid->mesh);
}