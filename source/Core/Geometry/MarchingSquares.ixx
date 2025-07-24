export module Finik.Core.Geometry:MarchingSquares;

import Finik.Core;
import Images;
import :Polygon;

struct MSEdge
{
    Vector2 point;
    int goTo = 0;
};

struct MSCell
{
    float value;
    
    bool isFilled() const
    {
        return value >= 0;
    }
};

export class MarchingSquares
{
public:
    MarchingSquares(Image& image, unsigned char threashold)
    {
        cells.resize(image.width, image.height);
        int count = image.getTexelCount();
        for (int i = 0; i < count; i++)
        {
            float alpha = static_cast<float>(image.getTexel(i).A);
            cells.get(i).value = alpha - threashold;
        }
    }

    void run()
    {
        calcEdgePoints();
        calcLines();
        findContours();
    }

    Array<Polygon>& getContours()
    {
        return contours;
    }

private:
    void calcEdgePoints()
    {
        edges.resize(cells.width * 2 - 1, cells.height * 2); // sparse chess array
        for (int x = 0; x < cells.width; x++)
        {
            for (int y = 0; y < cells.height; y++)
            {
                if (x)
                {
                    calcEdgePoint(x, y, x - 1, y);
                }
                if (y)
                {
                    calcEdgePoint(x, y, x, y - 1);
                }
            }
        }
    }

    void calcEdgePoint(int x1, int y1, int x2, int y2)
    {
        auto& cellA = cells.get(x1, y1);
        auto& cellB = cells.get(x2, y2);
        if (cellA.isFilled() != cellB.isFilled())
        {
            float a = std::abs(cellA.value);
            float b = std::abs(cellB.value);
            float k = a / (a + b);

            auto posA = Vector2(x1, y1);
            auto posB = Vector2(x2, y2);
            auto point = lerp(posA, posB, k) + Vector2::One / 2;

            edges.get(x1 + x2, y1 + y2).point = point;
        }
    }

    void calcLines()
    {
        for (int x = 0; x < cells.width - 1; x++)
        {
            for (int y = 0; y < cells.height - 1; y++)
            {
                int n = 0;
                n += cells.get(x + 0, y + 0).isFilled() * 8;
                n += cells.get(x + 1, y + 0).isFilled() * 4;
                n += cells.get(x + 1, y + 1).isFilled() * 2;
                n += cells.get(x + 0, y + 1).isFilled() * 1;

                if (n == 0 || n == 15)
                    continue;

                int baseIndex = edges.getIndex(x * 2 + 1, y * 2 + 1);
                int left = baseIndex - 1;
                int right = baseIndex + 1;
                int top = baseIndex - edges.width;
                int bottom = baseIndex + edges.width;
              
                switch (n)
                {
                case 1:
                    edges.get(left).goTo = bottom;
                    break;

                case 2:
                    edges.get(bottom).goTo = right;
                    break;

                case 3:
                    edges.get(left).goTo = right;
                    break;

                case 4:
                    edges.get(right).goTo = top;
                    break;

                case 5:
                    edges.get(left).goTo = top;
                    edges.get(right).goTo = bottom;
                    break;

                case 6:
                    edges.get(bottom).goTo = top;
                    break;

                case 7:
                    edges.get(left).goTo = top;
                    break;

                case 8:
                    edges.get(top).goTo = left;
                    break;

                case 9:
                    edges.get(top).goTo = bottom;
                    break;

                case 10:
                    edges.get(bottom).goTo = left;
                    edges.get(top).goTo = right;
                    break;

                case 11:
                    edges.get(top).goTo = right;
                    break;

                case 12:
                    edges.get(right).goTo = left;
                    break;

                case 13:
                    edges.get(right).goTo = bottom;
                    break;

                case 14:
                    edges.get(bottom).goTo = left;
                    break;
                }
            }
        }
    }

    void findContours()
    {
        int count = cells.count();
        for (int index = 1; index < count; index += 2)
        {
            if (edges.get(index).goTo > 0)
            {
                buildContour(index);
            }
        }
    }

    void buildContour(int startIndex)
    {
        Polygon contour;

        int index = startIndex;
        MSEdge* cur = &edges.get(index);

        while (cur->goTo > 0)
        {
            contour.points.append(cur->point);
            index = cur->goTo;
            cur->goTo = -cur->goTo;
            cur = &edges.get(index);
        }

        if (index != startIndex)
        {
            throw;
        }
        
        contours.append(contour);
    }

private:
    Table<MSCell> cells;
    Table<MSEdge> edges; // sparse chess array
    Array<Polygon> contours;
};