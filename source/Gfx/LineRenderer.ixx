export module LineRenderer;

import Finik.Core;
import Vertex;

export class LineRenderer
{
public:
    void setColor(Color color)
    {
        currentColor = color;
    }

    void line(Vector3 start, Vector3 end)
    {
        vertices.append({start, currentColor});
        vertices.append({end, currentColor});
    }

private:
    Color currentColor = Color::White;
    Array<LineVertex> vertices;
};