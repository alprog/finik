
export module Finik.Core.Geometry2D:Triangulator;

import Finik.Core;
import :Polygon;

export class Triangulator
{
    using Edge = std::pair<uint32, uint32>;

    struct Triangle
    {
        Array<uint32> vertices;
        Array<uint32> neighbors;
    };

public:
    void addPolygon(const Polygon& polygon);
    void run();

    auto& getVertices() const { return vertices; }
    auto& getEdges() const { return edges; }
    auto& getTriangles() const { return triangles; }

private:
    Array<Vector2> vertices;
    Array<Edge> edges;
    Array<Triangle> triangles;
};