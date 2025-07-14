export module TileMap;

import Finik.Core;
import Texture;
import Images;

export class TileMap
{
public:
    TileMap();

    std::shared_ptr<Image> image;
    std::shared_ptr<Texture> texture;
    bool changed;
};
