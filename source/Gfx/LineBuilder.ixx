export module LineBuilder;

import Finik.Core;
import Vertex;
import VertexBuffer;

export class LineBuilder
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

    LineVertexBuffer* build()
    {
        auto vertexBuffer = new LineVertexBuffer();
        vertexBuffer->vertices = vertices;
        vertexBuffer->Load();
        return vertexBuffer;
    }

private:
    Color currentColor = Color::White;
    Array<LineVertex> vertices;
};