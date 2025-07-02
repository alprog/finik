module sdf;

import assets;
import images;

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

void SDF::calcSDF(std::shared_ptr<Image> image)
{
    // not implemented
}
