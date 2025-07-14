export module TextureId;

import Finik.Core;

class Texture;

export struct TextureId
{
    Texture* Get();

    uint32 Index;
};
