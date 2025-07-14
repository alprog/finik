module;
#include "gfx/dx.h"
module TextureView;

import Texture;
import Imgui;
import Grid;
import TileMap;
import Scene;

TextureView::TextureView(const char* name, Scene& scene)
    : View {name}
    , scene {scene}
{
}

void TextureView::draw_content()
{
    auto texture = scene.grid->tileMap->texture;
    D3D12_GPU_DESCRIPTOR_HANDLE handle = texture->descriptorHandle.getGPU();

    ImTextureID textureId = (void*)handle.ptr;
    
    auto size = ImVec2(texture->Width, texture->Height);
    ImGui::Image(textureId, size);
}
