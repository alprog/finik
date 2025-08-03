module;
#include "CDT.h"
export module Finik.Core.Geometry:Triangulator;

import Finik.Core;
import :Polygon;

export class Triangulator
{
public:
    void addPolygon(const Polygon& polygon);
    void run();

    auto& getVertices() const { return vertices; }
    auto& getEdges() const { return edges; }
    auto& getTriangles() const { return triangles; }

private:
    Array<Vector2> vertices;
    Array<CDT::Edge> edges;
    Array<CDT::Triangle> triangles;
};