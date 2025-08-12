export module TileMap;

import Finik.Core;
import Texture;
import Images;

export class TileMap
{
public:
    TileMap();

    Ptr<Image> image;
    Ptr<Texture> texture;
    bool changed;
};
