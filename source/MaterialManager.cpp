module material_manager;

import app;
import render_system;

static int32 Counter = 0;

MaterialManager::MaterialManager()
{
    ConstantBuffer = new GpuMaterialsConstantBuffer(Single::Get<RenderSystem>());
}

int32 MaterialManager::Register(Material* material)
{
    const int32 Index = Counter++;

    Materials.resize(Counter, nullptr);
    Materials[Index] = material;

    return Index;
}

void MaterialManager::Unregister(Material* material)
{
    Materials[material->Index] = nullptr;
}

void MaterialManager::WriteToBuffer(Material* material)
{
    auto& Dst = ConstantBuffer->GetData()->Materials[material->Index];
    Dst.ColorA = Color(0, 0, 0);
    Dst.ColorB = Color(0, 0, 0);
    Dst.ColorC = Color(0, 0, 0);
    Dst.ColorD = Color(0, 0, 0);

    auto Count = material->Textures.count();
    Dst.TextureA = Count > 0 ? material->Textures[0]->descriptorHandle.getIndex() : 0;
    Dst.TextureB = Count > 1 ? material->Textures[1]->descriptorHandle.getIndex() : 0;
    Dst.TextureC = Count > 2 ? material->Textures[2]->descriptorHandle.getIndex() : 0;
    Dst.TextureD = Count > 3 ? material->Textures[3]->descriptorHandle.getIndex() : 0;
}