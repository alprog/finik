module;
#include "CDT.h"
module Finik.Core.Geometry2D:Triangulator;

void Triangulator::addPolygon(const Polygon& polygon)
{
    unsigned int startIndex = vertices.count();
    for (auto& point : polygon.points)
    {
        vertices.append(point);
    }

    unsigned int lastIndex = vertices.count() - 1;
    for (unsigned int i = startIndex; i < lastIndex; i++)
    {
        edges.append({i, i + 1});
    }
    edges.append({lastIndex, startIndex});
}

void Triangulator::run()
{
    CDT::Triangulation<float> cdt;

    cdt.insertVertices(
        vertices.begin(),
        vertices.end(),
        [](const Vector2& p) { return p.x; },
        [](const Vector2& p) { return p.y; });

    cdt.insertEdges(
        edges.begin(),
        edges.end(),
        [](const Edge& e) { return e.first; },
        [](const Edge& e) { return e.second; });

    cdt.eraseOuterTrianglesAndHoles();

    triangles.reserve(cdt.triangles.size());
    for (auto& t : cdt.triangles)
    {
        Triangle& triangle = triangles.emplace_back();
        triangle.vertices.reserve(t.vertices.size());
        for (auto& v : t.vertices)
        {
            triangle.vertices.append(v);
        }

        triangle.neighbors.reserve(t.neighbors.size());
        for (auto& n : t.neighbors)
        {
            triangle.neighbors.append(n);
        }
    }
}