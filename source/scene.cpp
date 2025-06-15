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
import surface_size;

// for intellisense

Scene::Scene()
{
    grid = new Grid();

    actors.append(new Actor());
    actors.append(new Actor());

    actors[1]->mesh = Assets::GetInstance().get<Model>("models/airplane.obj")->mesh;

    light.shadowMap = std::make_unique<FrameBuffer>(SurfaceSize(2048, 2048, 1), 0, true);
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

void Scene::renderShadowMaps(CommandList& commandList, RenderContext& context, Camera& camera)
{
    light.shadowMap->startRendering(commandList);

    light.shadowCamera.OrthoSize = 30;
    light.shadowCamera.FieldOfView = 0;
    light.shadowCamera.lookAt = camera.lookAt;
    light.shadowCamera.position = light.shadowCamera.lookAt - light.direction.xyz() * 30;
    light.shadowCamera.calcViewMatrix();
    light.shadowCamera.calcProjectionMatrix();

    render(context, light.shadowCamera, RenderPass::Shadow);

    light.shadowMap->endRendering(commandList);
}

void Scene::render(RenderContext& renderContext, Camera& camera, RenderPass pass)
{
    renderContext.setupRoot();

    RenderSystem& renderSystem = renderContext.renderSystem;
    auto frameConstants = renderSystem.getOneshotAllocator().Allocate<FrameConstants>();
    auto V = camera.viewMatrix;
    auto P = camera.projectionMatrix;
    frameConstants->View = V;
    frameConstants->Projection = P;
    frameConstants->ViewProjection = V * P;
    frameConstants->InverseViewProjection = frameConstants->ViewProjection.getInverse();
    frameConstants->NearFar = {camera.NearPlane, camera.FarPlane};

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