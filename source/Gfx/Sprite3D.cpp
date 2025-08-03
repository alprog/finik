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
    std::shared_ptr image = Images::loadPng(path);
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
}
