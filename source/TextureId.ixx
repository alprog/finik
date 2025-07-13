export module texture_id;

import Finik.Core;

class Texture;

export struct TextureId
{
    Texture* Get();

    uint32 Index;
};