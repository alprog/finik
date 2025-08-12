export module Material;

import Finik.Core;
import Texture;
import TextureId;
import Effect;

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
    Material(String name);
    ~Material();

    void RefreshBuffer();

    String Name;
    Array<Color> Colors;
    Array<Ptr<Texture>> Textures;

    int32 Index;

    Ptr<Effect> ShadowEffect;
    Ptr<Effect> Effect;
};

static_assert(sizeof(MaterialData) == 80);
