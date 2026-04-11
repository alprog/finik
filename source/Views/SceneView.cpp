module;
#include "gfx/dx.h"
module SceneView;

import Imgui;
import RenderSystem;
import DescriptorHandle;
import Log;
import Scene;
import EffectManager;
import RenderPass;
import QualityManager;
import SurfaceResolution;
import Mrt;
import FrameBuffer;

const char* bufferNames[] = { 
    "Default", "GBuffer", "LightBuffer", "ShadowMap", "ResolveBuffer", "DebugBuffer"
};

enum BufferType
{
    Default,
    GBuffer,
    LightBuffer,
    ShadowMap,
    ResolveBuffer,
    DebugBuffer
};

SceneView::SceneView(const char* name, Scene& scene)
    : View{name}
    , scene{scene}
    , camera{}
    , cameraContoller{camera}
{
    auto& settings = QualityManager::GetInstance().getCurrent();
    SurfaceResolution surfaceResolution = SurfaceResolution{1024, 800, getSampleCount(settings.msaa)};
    
    renderLane = MakePtr<SceneRenderLane>(scene, camera, surfaceResolution);
}

const CameraController& SceneView::getCameraController() const
{
    return cameraContoller;
}

static float DeltaTime = 0;

void SceneView::update(float deltaTime)
{
    DeltaTime = deltaTime;
}

FrameBuffer& getBuffer(SceneRenderLane& renderLane, Scene& scene, BufferType bufferType)
{
    switch (bufferType)
    {
        case BufferType::GBuffer:
            return renderLane.getGBuffer();

        case BufferType::LightBuffer:
            return renderLane.getLightBuffer();

        case BufferType::ShadowMap:
            return *scene.light.shadowMap;

        case BufferType::ResolveBuffer:
            return renderLane.getResolveBuffer();

        case BufferType::DebugBuffer:
            return renderLane.getDebugBuffer();

        default:
            const bool isTaa = QualityManager::GetInstance().getCurrent().taa;
            bufferType = isTaa ? BufferType::ResolveBuffer : BufferType::LightBuffer;
            return getBuffer(renderLane, scene, bufferType);
    }
}

void SceneView::draw_content()
{
    static BufferType bufferType = BufferType::Default;
    static MRT selectedType = MRT::RT0;

    BufferType* bufferTypePtr = &bufferType;

    ImGui::SetNextItemWidth(100);
    ImGui::Combo("Buffer", reinterpret_cast<int*>(&bufferType), bufferNames, 5);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    static const char* rtNames[]{"RT0", "RT1", "RT2", "RT3", "DS"};
    ImGui::Combo("Surface", reinterpret_cast<int*>(&selectedType), rtNames, 5);

    static bool Depth;
    Depth = selectedType == MRT::DS;

    auto imSize = ImGui::GetContentRegionAvail();
    auto msaa = QualityManager::GetInstance().getCurrent().msaa;

    renderLane->resize({
        static_cast<int>(imSize.x),
        static_cast<int>(imSize.y),
        getSampleCount(msaa)
    });

    auto& buffer = getBuffer(*renderLane, scene, bufferType);
    auto surface = buffer.getRenderSurface(selectedType);
    if (surface)
    {
        D3D12_GPU_DESCRIPTOR_HANDLE handle = surface->textureHandle.getGPU();
        ImTextureID textureId = (void*)handle.ptr;

        auto imageStartPos = ImGui::GetCursorScreenPos();
        bool AdvancedShader = true;
        if (AdvancedShader)
        {
            auto Callback = [](const ImDrawList* parent_list, const ImDrawCmd* cmd) //
            {
                ID3D12GraphicsCommandList* commandList = ImGuiGetCurrentCommandList();

                String effectName = Depth ? "imgui_ms_depth" : "imgui_ms";
                Ptr effect = EffectManager::GetInstance().get(effectName);
                commandList->SetPipelineState(effect->getPipelineState()->getInternalObject());
                effect->getPipelineState()->use();
            
                //int32 values[3] = {Size.width, Size.height, sampleCount};
                //commandList->SetGraphicsRoot32BitConstants(2, 3, &values, 0);    
            };
            GImGui->CurrentWindow->DrawList->AddCallback(Callback, nullptr);
        }
        ImGui::Image(textureId, imSize, ImVec2(0, 1), ImVec2(1, 0));

        if (AdvancedShader)
        {
            GImGui->CurrentWindow->DrawList->AddCallback((ImDrawCallback)(-8), nullptr);
        }

        if (ImGui::IsItemHovered())
        {
            auto mousePos = ImGui::GetMousePos();

            auto dx = (static_cast<float>(mousePos.x) - imageStartPos.x) / imSize.x;
            auto dy = (static_cast<float>(mousePos.y) - imageStartPos.y) / imSize.y;

            auto ndcPos = Vector2(dx, 1 - dy) * 2.0f - Vector2::One;

            log("ndc {} {}\n", ndcPos.x, ndcPos.y);

            cameraContoller.HandleInput(DeltaTime);
            cameraContoller.RefreshCameraPosition();

            auto ray = cameraContoller.Camera.castRay(ndcPos);

            log("ray {} {} {} | {} {} {}\n", ray.Origin.x, ray.Origin.y, ray.Origin.z, ray.Direction.x, ray.Direction.y,
                ray.Direction.z);

            if (ray.Direction.z != 0)
            {
                auto distance = ray.Origin.z / -ray.Direction.z;
                auto position = ray.Origin + ray.Direction * distance;
                log("pos {} {}\n", position.x, position.y);

                scene.castedPos = position;
            }
        }
    }

    renderLane->render();
}
