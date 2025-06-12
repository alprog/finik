module;
#include "gfx/dx.h"
module scene_view;

import imgui;
import render_system;
import descriptor_handle;
import log;
import scene;
import effect_manager;
import render_pass;
import quality_manager;
import surface_size;
import mrt;

// for intellisense

SceneView::SceneView(const char* name, Scene& scene)
    : View{name}
    , scene{scene}
    , camera{}
    , cameraContoller{camera}
{
    auto& settings = QualityManager::GetInstance().getCurrent();
    SurfaceSize surfaceSize = SurfaceSize{1024, 800, getSampleCount(settings.msaa)};
    
    renderLane = std::make_shared<SceneRenderLane>(scene, camera, surfaceSize);
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

void SceneView::draw_content()
{
    static int BufferType = 0;
    static int SelectedType = 0;

    static const char* items[]{"GBuffer", "LightBuffer"};
    ImGui::SetNextItemWidth(100);
    ImGui::Combo("Buffer", &BufferType, items, 2);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    static const char* items2[]{"RT0", "RT1", "RT2", "RT3", "DS"};
    ImGui::Combo("Surface", &SelectedType, items2, 5);

    bool Depth = SelectedType == MRT::DS;

    auto imSize = ImGui::GetContentRegionAvail();
    auto msaa = QualityManager::GetInstance().getCurrent().msaa;

    renderLane->resize({
        static_cast<int>(imSize.x),
        static_cast<int>(imSize.y),
        getSampleCount(msaa)
    });

    auto& buffer = BufferType ? renderLane->getLightBuffer() : renderLane->getGBuffer();
    auto surface = buffer.gerRenderSurface(static_cast<MRT>(SelectedType));
    if (surface)
    {
        D3D12_GPU_DESCRIPTOR_HANDLE handle = surface->textureHandle.getGPU();
        ImTextureID textureId = (void*)handle.ptr;

        auto imageStartPos = ImGui::GetCursorScreenPos();
        Depth = true;
        if (Depth)
        {
            auto Callback = [](const ImDrawList* parent_list, const ImDrawCmd* cmd) //
            {
                ID3D12GraphicsCommandList* commandList = ImGuiGetCurrentCommandList();

                std::shared_ptr effect = EffectManager::GetInstance().get("imgui_ms");
                commandList->SetPipelineState(effect->getPipelineState()->getInternalObject());
                effect->getPipelineState()->use();
            
                //int32 values[3] = {Size.width, Size.height, sampleCount};
                //commandList->SetGraphicsRoot32BitConstants(2, 3, &values, 0);    
            };
            GImGui->CurrentWindow->DrawList->AddCallback(Callback, nullptr);
        }
        ImGui::Image(textureId, imSize);

        if (Depth)
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
