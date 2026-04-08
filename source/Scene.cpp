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
import QualityManager;

import Finik.Core.Geometry3D;

// for intellisense

Scene::Scene()
{
    grid = new Grid();

    actors.append(new Actor());
    actors.append(new Actor());
    actors.append(new Actor());
    actors.append(new Actor());
    actors.append(new Actor());

    characters.append(new Character());

    characters[0]->transformMatrix = Matrix::RotationZ(PI) * Matrix::Translation({14.5f, 14.5f, 0});

    actors[1]->mesh = Assets::GetInstance().get<Model>("models/airplane.obj")->mesh;
    actors[2]->mesh = Assets::GetInstance().get<Model>("models/Axis.obj")->mesh;
    actors[3]->mesh = Assets::GetInstance().get<Model>("models/littleman.obj")->mesh;
    actors[4]->mesh = Assets::GetInstance().get<Model>("models/wooden watch tower2.obj")->mesh;

    auto size = QualityManager::GetInstance().getCurrent().shadowMapResolution;
    light.shadowMap = MakeUnique<FrameBuffer>(SurfaceResolution(size, size, 1), Array<TextureFormat>{TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM}, true);
    light.direction = Vector4(-1, -1, -1, 0).getNormalized();
}

void Scene::update(float deltaTime)
{
    //light.direction = light.direction * Matrix::RotationZ(deltaTime / 2);

    actors[0]->transformMatrix = Matrix::Translation(Vector3(castedPos.x, castedPos.y, 0.5f));
    actors[1]->transformMatrix = Matrix::Translation(Vector3(32, 32, 5));

    actors[2]->transformMatrix = Matrix::Translation(Vector3(0, 0, 0));
    actors[3]->transformMatrix = Matrix::Translation(Vector3(64, 64, 0.5));
    actors[4]->transformMatrix = Matrix::Translation(Vector3(32, 64, -1));
}

void Scene::renderShadowMaps(CommandList& commandList, RenderContext& context, Camera& camera)
{
    light.shadowMap->startRendering(commandList);

    light.shadowCamera.lookAt = Vector3::Zero;
    light.shadowCamera.position = -light.direction.xyz() * 1000;
    light.shadowCamera.calcViewMatrix();

    auto minHeight = -1.f;
    auto maxHeight = +10.f;
    
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
    BoundBox<Vector3> viewFrustrumBoundBox(a, b, c, d, e, f, g, h);

    auto minSize = std::max(viewFrustrumBoundBox.size().x, viewFrustrumBoundBox.size().z);
    auto size = std::pow(2.0f, std::ceil(std::log2(minSize)));

    auto center = viewFrustrumBoundBox.center();

    auto qualitySettings = QualityManager::GetInstance().getCurrent();
    if (qualitySettings.shadowSnapping)
    {
        float texelWorldSize = size / light.shadowMap->resolution.width;

        center.x = std::floor(center.x / texelWorldSize) * texelWorldSize;
        center.z = std::floor(center.z / texelWorldSize) * texelWorldSize;
    }

    BoundBox<Vector3> sceneBoundBox(
        Vector3{0, 0, minHeight},
        Vector3{256, 256, maxHeight}
    );

    Clipper clipper;
    clipper.addBoundBox(sceneBoundBox, light.shadowCamera.viewMatrix);
    clipper.clipX(center.x - size / 2, center.x + size / 2);
    clipper.clipZ(center.z - size / 2, center.z + size / 2);

    float nearPlane = std::numeric_limits<float>::max();
    float farPlane = std::numeric_limits<float>::lowest();
    for (auto& triangle : clipper.triangles)
    {
        for (int i = 0; i < 3; i++)
        {
            nearPlane = std::min(nearPlane, triangle.points[i].y);
            farPlane = std::max(farPlane, triangle.points[i].y);
        }
    }

    light.shadowCamera.OrthoSize.y = size;
    light.shadowCamera.OrthoOffset = -Vector2(center.x, center.z);

    light.shadowCamera.FieldOfView = 0;
    light.shadowCamera.NearPlane = nearPlane;
    light.shadowCamera.FarPlane = farPlane;   
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

