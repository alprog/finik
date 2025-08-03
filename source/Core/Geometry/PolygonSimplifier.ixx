export module Finik.Core.Geometry:PolygonSimplifier;

import Finik.Core.Containers;
import Finik.Core.Math;
import Finik.Core.Utils;
import :Polygon;

struct Point
{
    Vector2 position;
    Point* prev;
    Point* next;
    float area;
    bool dirty;

    void calcArea()
    {
        area = triangleArea(position, prev->position, next->position);
        dirty = false;
    }

    void exclude()
    {
        prev->next = next;
        next->prev = prev;
        prev->calcArea();
        next->calcArea();
        prev = nullptr;
        next = nullptr;
    }

    bool isExcluded()
    {
        return next == nullptr;
    }
};

struct PointComparator
{
    bool operator()(const Point* a, const Point* b) const
    {
        return a->area > b->area;
    }
};

 // Visvalingam–Whyatt
export class PolygonSimplifier
{
public:
    PolygonSimplifier(Polygon& polygon)
    {
        int count = polygon.pointCount();
        points.resize(count);

        for (int i = 0; i < count; i++)
        {
            auto& point = points[i];
            point.position = polygon.points[i];
            point.prev = &points[wrapIndex(i - 1)];
            point.next = &points[wrapIndex(i + 1)];
        }

        for (auto& p : points)
        {
            p.calcArea();
        }
    }

    void run()
    {
        BinaryHeap<Point*, PointComparator> heap;
        for (auto& p : points)
        {
            heap.pushWithoutHeapify(&p);
        }
        heap.makeHeap();

        while (heap.count() > 30)
        {
            float area = heap.peek()->area;
            //if (heap.peek()->dirty)
            //{
            //    heap.peek()->calcArea();
            //    heap.heapifyDown(0);
            //}
            //else
            {
                heap.pop()->exclude();
                heap.makeHeap();
            }
        }
    }

    Polygon getResult()
    {
        Polygon result;
        for (auto& point : points)
        {
            if (!point.isExcluded())
            {
                result.points.append(point.position);
            }
        }
        return result;
    }

private:
    int wrapIndex(int index)
    {
        int count = points.count();
        return (index + count) % count;
    }

private:
    Array<Point> points;
};