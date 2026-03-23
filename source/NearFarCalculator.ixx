export module NearFarCalculator;

import Finik.Core;

export class NearFarCalculator
{
    struct DepthRange
    {
        float near;
        float far;
    };

    struct TravelRange
    {
        float min;
        float max;

        bool isValid()
        {
            return min < max;
        }

        void intersect(float a, float b)
        {
            if (a > b)
            {
                std::swap(a, b);
            }

            min = std::max(min, a);
            max = std::min(max, b);
        }
    };

    struct Segment
    {
        Vector3 start;
        Vector3 end;
    };

    void Run(BoundBox<Vector3> sceneBounds, BoundBox<Vector3> lightViewBounds, Matrix& lightViewMatrix)
    {
        // scene bounding box points
        Array<Vector3> points = sceneBounds.getPoints();
        for (auto& point : points)
        {
            point = lightViewMatrix.MultiplyPoint(point);
        }

        // scene bounding box lines in light view space
        Array<Segment> segments = {
            {points[0b000], points[0b001]},
            {points[0b000], points[0b010]},
            {points[0b000], points[0b100]},
            
            {points[0b001], points[0b011]},
            {points[0b001], points[0b101]},
            {points[0b010], points[0b011]},
            {points[0b010], points[0b110]},
            {points[0b100], points[0b101]},
            {points[0b100], points[0b110]},

            {points[0b011], points[0b111]},
            {points[0b101], points[0b111]},
            {points[0b110], points[0b111]},
        };

        for (auto& segment : segments)
        {
            Vector3 segmentDelta = segment.end - segment.start;
            float segmentLength = segmentDelta.length();

            TravelRange travelRange(0, segmentLength);

            auto origin = segment.start;
            auto direction = segmentDelta / segmentLength;

            if (direction.x != 0)
            {
                auto distA = (lightViewBounds.min.x - origin.x) / direction.x;
                auto distB = (lightViewBounds.max.x - origin.x) / direction.x;
                travelRange.intersect(distA, distB);
            }

            if (direction.z != 0)
            {
                auto distA = (lightViewBounds.min.z - origin.z) / direction.z;
                auto distB = (lightViewBounds.max.z - origin.z) / direction.z;
                travelRange.intersect(distA, distB);
            }

            if (travelRange.isValid())
            {

            }
        }
    }
};