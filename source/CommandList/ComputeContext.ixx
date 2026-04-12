export module ComputeContext;

import Finik.Core;
import DX;
import RenderEngine;

export class ComputeContext
{
public:
    ComputeContext(RenderEngine& renderEngine, ID3D12GraphicsCommandList& commandList);

    void setupRoot();

    void setConstants(D3D12_GPU_VIRTUAL_ADDRESS gpuAddress);
    void setTexture(D3D12_GPU_DESCRIPTOR_HANDLE handle);
    void setUnorderedAccessView(D3D12_GPU_DESCRIPTOR_HANDLE handle);

    RenderEngine& engine;
    ID3D12GraphicsCommandList& commandList;
};
