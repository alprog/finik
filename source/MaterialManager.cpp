module MaterialManager;

import App;
import RenderSystem;

static int32 Counter = 0;

MaterialManager::MaterialManager()
{
    ConstantBuffer = new GpuMaterialsConstantBuffer(Single::Get<RenderSystem>().engine.getDevice());
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
    
    auto Count = material->Colors.count();
    Dst.ColorA = Count > 0 ? material->Colors[0] : Color::Black;
    Dst.ColorB = Count > 1 ? material->Colors[1] : Color::Black;
    Dst.ColorC = Count > 2 ? material->Colors[2] : Color::Black;
    Dst.ColorD = Count > 3 ? material->Colors[3] : Color::Black;

    Count = material->Textures.count();
    Dst.TextureA = Count > 0 ? material->Textures[0]->descriptorHandle.getIndex() : 0;
    Dst.TextureB = Count > 1 ? material->Textures[1]->descriptorHandle.getIndex() : 0;
    Dst.TextureC = Count > 2 ? material->Textures[2]->descriptorHandle.getIndex() : 0;
    Dst.TextureD = Count > 3 ? material->Textures[3]->descriptorHandle.getIndex() : 0;
}
