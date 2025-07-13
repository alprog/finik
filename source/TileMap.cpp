module tile_map;

import texture;
import images;

TileMap::TileMap()
{
    image = std::make_shared<Image>(256, 256);
    image->generateChessboard();

    texture = std::make_shared<Texture>(*image);
}