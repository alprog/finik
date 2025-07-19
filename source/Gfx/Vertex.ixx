export module Vertex;

import Finik.Core;

export struct StandardVertex
{
    Vector3 position;
    Vector3 normal;
    Vector2 texCoord;
    int x;
    int y;
};

export struct LineVertex
{
    Vector3 position;
    Color color;
};