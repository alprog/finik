export module Grid;

import TileMap;
import Mesh;
import Material;

export class Grid
{
public:
    Grid();

    Mesh* Grid::CreateMesh();

    TileMap* tileMap;
    Mesh* mesh;
    Material* material;
};
