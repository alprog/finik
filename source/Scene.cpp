module Scene;

import TextureFormat;
import RenderSystem;
import Effect;
import Mesh;
import Texture;
import Actor;
import Shader;
import Grid;
import Camera;
import OneshotAllocator;
import Allocation;
import EffectManager;
import RootSignatureParams;
import RenderContext;
import Assets;
import Model;
import SurfaceResolution;
import EffectManager;

// for intellisense

Scene::Scene()
{
    grid = new Grid();

    actors.append(new Actor());
    actors.append(new Actor());

    characters.append(new Character());

    characters[0]->transformMatrix = Matrix::RotationZ(std::numbers::pi) * Matrix::Translation({14.5f, 14.5f, 0});

    actors[1]->mesh = Assets::GetInstance().get<Model>("models/airplane.obj")->mesh;

    light.shadowMap = MakeUnique<FrameBuffer>(SurfaceResolution(2048, 2048, 1), Array<TextureFormat>{TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM}, true);
    light.direction = Vector4(-1, -1, -1, 0).getNormalized();
}

void Scene::update(float deltaTime)
{
    light.direction = light.direction * Matrix::RotationZ(deltaTime / 2);

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


    auto size = boundBox.size();
    size.y = 0;

    light.shadowCamera.OrthoSize = size.length();
    light.shadowCamera.FieldOfView = 0;
    light.shadowCamera.NearPlane = boundBox.min.y;
    light.shadowCamera.FarPlane = boundBox.max.y;   
    light.shadowCamera.calcProjectionMatrix();

    render(context, light.shadowCamera, Matrix::Identity, Vector2::Zero, RenderPass::Shadow);

    light.shadowMap->endRendering(commandList);
}

void Scene::render(RenderContext& renderContext, const Camera& camera, const Matrix& prevViewProjection, const Vector2& prevJitter, RenderPass pass)
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
    frameConstants->PrevViewProjection = prevViewProjection;
    frameConstants->NearFar = {camera.NearPlane, camera.FarPlane};
    frameConstants->Jitter = camera.Jitter;
    frameConstants->PrevJitter = prevJitter;

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
        renderContext.drawMesh(character->sprite->mesh.get());
    }

    //----------------------

    renderContext.setModelMatrix(Matrix::Identity);
    renderContext.setMaterial(*grid->material, pass);
    renderContext.drawMesh(grid->mesh);
}

void Scene::debugRender(RenderContext& renderContext, const Camera& camera, const Matrix& prevViewProjection, const Vector2& prevJitter)
{
    RenderSystem& renderSystem = renderContext.renderSystem;
    auto frameConstants = renderSystem.getOneshotAllocator().Allocate<FrameConstants>();
    auto V = camera.viewMatrix;
    auto P = camera.projectionMatrix;
    frameConstants->View = V;
    frameConstants->Projection = P;
    frameConstants->ViewProjection = V * P;
    renderContext.setFrameConstants(frameConstants.GpuAddress);

    renderContext.setEffect(*EffectManager::GetInstance().get("lines"));
    for (auto& character : characters)
    {
        if (character->debugLines)
        {
            renderContext.setModelMatrix(character->transformMatrix);
            renderContext.drawLines(character->debugLines);
        }
    }
}

