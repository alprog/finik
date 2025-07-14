module Sdf;

import Assets;
import Images;

SDF::SDF(std::shared_ptr<Asset> asset)
{
    hotreloadDependencies.add(asset);
    rebuild();
}

void SDF::rebuild()
{
    auto path = hotreloadDependencies.getDependency(0).first->getPath();
    std::shared_ptr image = Images::loadPng(path);
    calcSDF(image);
    texture = std::make_shared<Texture>(*image);
}

void scan(Texel* data, int32 startIndex, int32 step, int32 count, int32 channel)
{
    for (int i = 0; i < count; i++)
    {

    }
}

void SDF::calcSDF(std::shared_ptr<Image> image)
{
    int32 width = image->width;
    int32 height = image->height;

    // rows
    for (int32 y = 0; y < height; y++)
    {
        scan(image->data, y * width, 1, width, 0);
    }

    // cols
    for (int32 x = 0; x < width; x++)
    {
        scan(image->data, x, width, height, 1);
    }
}
