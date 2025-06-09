export module material;

import core;
import texture;
import texture_id;
import effect;

// for intellisense

export struct MaterialData
{
    Color ColorA;
    Color ColorB;
    Color ColorC;
    Color ColorD;
    int32 TextureA;
    int32 TextureB;
    int32 TextureC;
    int32 TextureD;
};

export struct MaterialsConstantBuffer
{
    MaterialData Materials[80];
};

export class Material
{
public:
    Material(std::string name);
    ~Material();

    void RefreshBuffer();

    std::string Name;
    Array<Color> Colors;
    Array<std::shared_ptr<Texture>> Textures;

    int32 Index;

    std::shared_ptr<Effect> ShadowEffect;
    std::shared_ptr<Effect> Effect;
};

static_assert(sizeof(MaterialData) == 80);