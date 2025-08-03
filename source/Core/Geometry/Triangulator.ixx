module;
#include "CDT.h"
export module Finik.Core.Geometry:Triangulator;

import Finik.Core;
import :Polygon;

export class Triangulator
{
public:


    void run()
    {
        std::vector<CDT::V2d<float>> vertices;
        std::vector<CDT::Edge> edges;
 
        int totalCount = getPointCount();
        vertices.reserve(totalCount);
        edges.reserve(totalCount);
        
        for (auto& polygon : polygons)
        {
            unsigned int startIndex = vertices.size();
            for (auto& point : polygon.points)
            {
                vertices.push_back({point.x, point.y});
            }

            unsigned int lastIndex = vertices.size() - 1;
            for (unsigned int i = startIndex; i < lastIndex; i++)
            {
                edges.push_back({i, i + 1});
            }
            edges.push_back({lastIndex, startIndex});
        }

        CDT::Triangulation<float> cdt;
        cdt.insertVertices(vertices);
        cdt.insertEdges(edges);
        cdt.eraseOuterTrianglesAndHoles();


    }

private:
    int getPointCount()
    {
        int count = 0;
        for (auto& polygon : polygons)
        {
            count += polygon.pointCount();
        }
        return count;
    }

private:
    Array<Polygon> polygons;
};