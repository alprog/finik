module;
#include "dx.h"
module texture;

import render_system;
import upload_buffer;
import images;
import command_list;
import mipmap_generator;

static const uint32 TexturePixelSize = 4;

Texture::Texture(AssetPath asset_path)
    : Asset{asset_path}
    , Width{0}
    , Height{0}
{
    auto& renderSystem = Single::Get<RenderSystem>();
    DescriptorHeap* heap = renderSystem.getCommonHeap();
    descriptorHandle = heap->getNextHandle();
}

int32 Texture::calcMipMapCount()
{
    int32 maxSide = max(Width, Height);
    float count = std::log2(static_cast<float>(maxSide));
    return static_cast<int32>(std::ceil(count));
}

void Texture::resize(int32 width, int32 height)
{
    if (this->Width == width && this->Height == height)
    {
        return;
    }

    this->Width = width;
    this->Height = height;

    auto& renderSystem = Single::Get<RenderSystem>();
    auto* device = renderSystem.get_device();

    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.MipLevels = calcMipMapCount();
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    reinit(textureDesc, D3D12_RESOURCE_STATE_COPY_DEST);

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;

    device->CreateShaderResourceView(InternalResource, &srvDesc, descriptorHandle.getCPU());
}

Texture::Texture(int32 width, int32 height)
    : Texture("")
{
    resize(width, height);
}

Texture::Texture(Image& image)
    : Texture(image.width, image.height)
{
    setData(image);
}

void Texture::hot_reload(ByteBlob& blob)
{
    std::shared_ptr image = Images::loadPng(blob);
    resize(image->width, image->height);
    setData(*image);
}

void Texture::setData(Image& image)
{
    auto& renderSystem = Single::Get<RenderSystem>();

    auto device = renderSystem.get_device();
    CommandList& commandList = renderSystem.getFreeCommandList();
    commandList.startRecording();

    commandList.transition(*this, D3D12_RESOURCE_STATE_COPY_DEST);

    const uint64 uploadBufferSize = GetRequiredIntermediateSize(InternalResource, 0, 1);
    UploadBuffer uploadBuffer(renderSystem, uploadBufferSize);

    if (uploadBufferSize == image.getByteSize())
    {
        memcpy(uploadBuffer.GetData(), image.data, uploadBufferSize);
    }
    else
    {
        D3D12_SUBRESOURCE_DATA textureData = {};
        textureData.pData = image.data;
        textureData.RowPitch = image.width * TexturePixelSize;
        textureData.SlicePitch = textureData.RowPitch * image.height;
        UpdateSubresources(commandList.listImpl.Get(), InternalResource, uploadBuffer.GetResource(), 0, 0, 1, &textureData);
    }

    D3D12_PLACED_SUBRESOURCE_FOOTPRINT srcFootprint;
    uint32 numRows;
    uint64 rowSizeInBytes, totalBytes;
    device->GetCopyableFootprints(&InternalResource->GetDesc(), 0, 1, 0, &srcFootprint, &numRows, &rowSizeInBytes, &totalBytes);

    const CD3DX12_TEXTURE_COPY_LOCATION Src(uploadBuffer.GetResource(), srcFootprint);
    const CD3DX12_TEXTURE_COPY_LOCATION Dst(InternalResource, 0);
    commandList.listImpl->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);

    commandList.transition(*this, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    MipMapGenerator::GetInstance().Generate(*this, commandList);

    commandList.endRecording();

    auto& commandQueue = renderSystem.get_command_queue();
    commandQueue.execute(commandList);
    commandQueue.Flush();

    state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
}