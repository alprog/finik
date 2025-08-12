module TileMap;

import Texture;
import Images;

TileMap::TileMap()
{
    image = MakePtr<Image>(256, 256);
    image->generateChessboard();

    texture = MakePtr<Texture>(*image);
}
