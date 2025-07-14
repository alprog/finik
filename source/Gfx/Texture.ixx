export module Texture;

import Finik.Core;
import DX;
import DescriptorHandle;
import Images;
import GpuResource;
import ByteBlob;
import Asset;
import AssetPath;

// for intellisense

export class Texture : public GpuResource, public Asset
{
public:
    Texture(AssetPath asset_path);

    void resize(int32 width, int32 height);

    Texture(int32 width, int32 height);
    Texture(Image& image);

    void setData(Image& image);

protected:
    void hot_reload(ByteBlob& blob) override;

private:
    int32 calcMipMapCount();

public:
    int Width;
    int Height;

    DescriptorHandle descriptorHandle;
};
