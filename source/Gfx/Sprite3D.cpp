module Sprite3D;

import Assets;
import Images;
import Finik.Core.Geometry;
import MeshBuilder;

Sprite3D::Sprite3D(std::shared_ptr<Asset> asset)
{
    hotreloadDependencies.add(asset);
    rebuild();
}

void Sprite3D::rebuild()
{
    auto path = hotreloadDependencies.getDependency(0).first->getPath();
    std::shared_ptr image = Images::loadPng(Path::combine("C:/finik/assets", path));
    createMesh(image);
}

void Sprite3D::createMesh(std::shared_ptr<Image> image)
{
    // FindContours
    constexpr int alphaThreashold = 128;
    MarchingSquares squares(*image, alphaThreashold);
    squares.run();
    auto& contours = squares.getContours();

    // Simplify
    PolygonSimplifier simplifier;
    simplifier.simplify(contours, 30);

    // Triangulate
    Triangulator triangulator;
    for (auto& contour : contours)
    {
        triangulator.addPolygon(contour);
    }
    triangulator.run();

    // BuildMesh
    MeshBuilder builder;

    auto vertexCount = triangulator.getVertices().count();
    builder.Vertices.reserve(vertexCount);

    auto mult = [](Vector2 p) {
        p = p / 64.0f;
        return Vector3(0.5 - p.x, 0, p.y * 2);
    };

    for (auto& vertex2D : triangulator.getVertices())
    {
        StandardVertex vertex;
        vertex.position = mult(vertex2D);
        vertex.texCoord = vertex2D / 64.0f;
        vertex.normal = Vector3::Forward;        
        builder.addVertex(vertex);
    }

    auto triangleCount = triangulator.getTriangles().count();
    builder.Indices.reserve(triangleCount * 3);
    for (auto& triangle : triangulator.getTriangles())
    {
        for (auto i : triangle.vertices)
        {
            builder.addIndex(i);
        }
    }

    mesh = std::shared_ptr<Mesh>( builder.Build() );
}
