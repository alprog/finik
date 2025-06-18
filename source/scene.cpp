module scene;

import texture_format;
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
import surface_resolution;

// for intellisense

Scene::Scene()
{
    grid = new Grid();

    actors.append(new Actor());
    actors.append(new Actor());

    characters.append(new Character());

    characters[0]->transformMatrix = Matrix::RotationZ(std::numbers::pi / 2) * Matrix::Translation({14.5f, 14.5f, 0});

    actors[1]->mesh = Assets::GetInstance().get<Model>("models/airplane.obj")->mesh;

    light.shadowMap = std::make_unique<FrameBuffer>(SurfaceResolution(2048, 2048, 1), Array<TextureFormat>{}, true);
    light.direction = Vector4(-1, -1, -1, 0).getNormalized();
}

void Scene::update(float deltaTime)
{
    light.direction = light.direction * Matrix::RotationZ(deltaTime / 50);

    actors[0]->transformMatrix = Matrix::Translation(Vector3(castedPos.x, castedPos.y, 0.0f));
    actors[1]->transformMatrix = Matrix::Translation(Vector3(32, 32, 5));
}

void Scene::renderShadowMaps(CommandList& commandList, RenderContext& context, Camera& camera)
{
    light.shadowMap->startRendering(commandList);

    light.shadowCamera.lookAt = camera.lookAt;
    light.shadowCamera.position = light.shadowCamera.lookAt - light.direction.xyz() * 1000;
    light.shadowCamera.calcViewMatrix();

    auto minHeight = -1;
    auto maxHeight = +15;
    
    auto calcLightPos = [this, &camera](Vector2 ndcPos, float height) {

        Vector3 worldPos = camera.castRay(ndcPos).castToHorizontalPlane(height);
        return light.shadowCamera.viewMatrix.MultiplyPoint(worldPos);
    };

    auto a = calcLightPos({-1, +1}, minHeight);
    auto b = calcLightPos({+1, +1}, minHeight);
    auto c = calcLightPos({-1, -1}, minHeight);
    auto d = calcLightPos({+1, -1}, minHeight);
    auto e = calcLightPos({-1, +1}, maxHeight);
    auto f = calcLightPos({+1, +1}, maxHeight);
    auto g = calcLightPos({-1, -1}, maxHeight);
    auto h = calcLightPos({+1, -1}, maxHeight);
    BoundBox<Vector3> boundBox(a, b, c, d, e, f, g, h);

    auto diagonalLength = boundBox.size().length();


    light.shadowCamera.OrthoSize = boundBox.size().xy().length();
    light.shadowCamera.FieldOfView = 0;
    light.shadowCamera.NearPlane = boundBox.min.z;
    light.shadowCamera.FarPlane = boundBox.max.z;    
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

    for (auto& character : characters)
    {
        renderContext.setModelMatrix(character->transformMatrix);
        renderContext.setMaterial(*character->material, pass);
        renderContext.drawMesh(character->bodyMesh);
    }

    //----------------------

    renderContext.setModelMatrix(Matrix::Identity);
    renderContext.setMaterial(*grid->material, pass);
    renderContext.drawMesh(grid->mesh);
}