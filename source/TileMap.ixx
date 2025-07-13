export module tile_map;

import Finik.Core;
import texture;
import images;

export class TileMap
{
public:
    TileMap();

    std::shared_ptr<Image> image;
    std::shared_ptr<Texture> texture;
    bool changed;
};