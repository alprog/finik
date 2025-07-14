module TileMap;

import Texture;
import Images;

TileMap::TileMap()
{
    image = std::make_shared<Image>(256, 256);
    image->generateChessboard();

    texture = std::make_shared<Texture>(*image);
}
