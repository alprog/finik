module;
#include "CDT.h"
export module Finik.Core.Geometry:Triangulator;

import Finik.Core;
import :Polygon;

export class Triangulator
{
public:
    void addPolygon(const Polygon& polygon)
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

    void run()
    {
        CDT::Triangulation<float> cdt;

        cdt.insertVertices(
            vertices.begin(), 
            vertices.end(),
            [](const Vector2& p) { return p.x; },
            [](const Vector2& p) { return p.y; }
        );

        cdt.insertEdges(
            edges.begin(),
            edges.end(),
            [](const CDT::Edge& e) { return e.v1(); },
            [](const CDT::Edge& e) { return e.v2(); }
        );

        cdt.eraseOuterTrianglesAndHoles();
        
        triangles.reserve(cdt.triangles.size());
        for (auto& t : cdt.triangles)
        {
            triangles.append(t);
        }
    }

    auto& getVertices() const { return vertices; }
    auto& getEdges() const { return edges; }
    auto& getTriangles() const { return triangles; }

private:
    Array<Vector2> vertices;
    Array<CDT::Edge> edges;
    Array<CDT::Triangle> triangles;
};