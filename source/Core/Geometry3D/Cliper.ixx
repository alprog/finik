export module Finik.Core.Geometry3D:Clipper;

export import :Triangle;
import Finik.Core.PrimitiveTypes;
import Finik.Core.Containers;
import Finik.Core.Math;
import Finik.Core.Utils;

export class Clipper
{
public:
    void clipX(float minX, float maxX) { clipAxis(0, minX, maxX); }
    void clipY(float minY, float maxY) { clipAxis(1, minY, maxY); }
    void clipZ(float minZ, float maxZ) { clipAxis(2, minZ, maxZ); }

    void addBoundBox(BoundBox<Vector3> boundBox, Matrix matrix)
    {
        auto points = boundBox.getPoints(); 
        for (auto& point : points) 	  
        {
            point = matrix.MultiplyPoint(point);
        }
        
        triangles.reserve(triangles.count() + 12);

        // 0bZYX

        addFace(points[0b000], points[0b100], points[0b110], points[0b010]); // minX
        addFace(points[0b001], points[0b101], points[0b111], points[0b011]); // maxX

        addFace(points[0b000], points[0b100], points[0b101], points[0b001]); // minY
        addFace(points[0b010], points[0b110], points[0b111], points[0b011]); // maxY
        
        addFace(points[0b000], points[0b010], points[0b011], points[0b001]); // minZ
        addFace(points[0b100], points[0b110], points[0b111], points[0b101]); // maxZ
    }

    void addFace(Vector3 a, Vector3 b, Vector3 c, Vector3 d)
    {
        triangles.emplace_back(a, b, c);
        triangles.emplace_back(a, c, d);
    }

private:
    void clipAxis(int axisIndex, float min, float max)
    {
        clipByAxisPlane(axisIndex, std::greater<float> {}, min);
        clipByAxisPlane(axisIndex, std::less<float>{}, max);
    }

    void clipByAxisPlane(int axisIndex, std::function<bool(float, float)> comparator, float refValue)
    {
        auto clipTest = [=](Vector3& point) { 
            return comparator(point.components[axisIndex], refValue);
        };

        for (int32 triIndex = triangles.count() - 1; triIndex >= 0; triIndex--)
        {
            auto& triangle = triangles[triIndex];

            int32 passMask = 0;
            for (int32 i = 0; i < 3; i++)
            {
                if (clipTest(triangle.points[i]))
                {
                    passMask |= 1 << i;
                }
            }

            switch (passMask)
            {
                case 0b111: // all points passed
                    break;

                case 0b000: // all points failed
                    triangles.remove_at(triIndex);
                    break;
                     
                case 0b001: // A passed
                    clipTriangle(triangle, axisIndex, refValue);
                    break;

                case 0b010: // B passed
                    std::swap(triangle.B, triangle.A);
                    clipTriangle(triangle, axisIndex, refValue);
                    break;

                case 0b100: // C passed
                    std::swap(triangle.C, triangle.A);
                    clipTriangle(triangle, axisIndex, refValue);
                    break;

                case 0b011: // AB passed
                    sliceTriangle(triangle, axisIndex, refValue);
                    break;

                case 0b101: // AC passed
                    std::swap(triangle.C, triangle.B);
                    sliceTriangle(triangle, axisIndex, refValue);
                    break;

                case 0b110: // BC passed
                    std::swap(triangle.C, triangle.A);
                    sliceTriangle(triangle, axisIndex, refValue);
                    break;
            }
        }
    }

    Vector3 findAxisIntersectPoint(Vector3 PointA, Vector3 PointB, int axisIndex, float axisIntersectValue)
    {
        float axisValueA = PointA.components[axisIndex];
        float axisValueB = PointB.components[axisIndex];
        float k = (axisIntersectValue - axisValueA) / (axisValueB - axisValueA);
        return lerp(PointA, PointB, k);
    } 
    
    // assume A passed clip test, BC failed
    // 
    //          |  /\ B
    //        B'|/   \
    //         /|     \
    //       /  |      \
    //     /    |       \
    //   /______|________\
    //  A     C'|         C
    //          
    void clipTriangle(Triangle& triangle, int axisIndex, float clipValue)
    {
        triangle.B = findAxisIntersectPoint(triangle.A, triangle.B, axisIndex, clipValue);
        triangle.C = findAxisIntersectPoint(triangle.A, triangle.C, axisIndex, clipValue);
    }

    // assume AB passed clip test, C failed
    //            
    //       B /\ |
    //        /  \| 
    //       /    \ E
    //      /    /|\
    //     /   /  | \
    //    /  /    |  \
    //   /_/______|___\
    //  A         | D   C
    //
    void sliceTriangle(Triangle& triangle, int axisIndex, float clipValue)
    {
        Vector3 D = findAxisIntersectPoint(triangle.A, triangle.C, axisIndex, clipValue);
        Vector3 E = findAxisIntersectPoint(triangle.B, triangle.C, axisIndex, clipValue);
        triangle.C = E; // current triangle becomes ABE
        triangles.append({triangle.A, D, E}); // add new triangle ADE
    }

public:
    Array<Triangle> triangles;
};