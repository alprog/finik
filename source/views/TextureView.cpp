module;
#include "gfx/dx.h"
module texture_view;

import texture;
import imgui;
import grid;
import tile_map;
import scene;

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