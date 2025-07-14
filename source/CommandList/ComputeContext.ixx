export module ComputeContext;

import Finik.Core;
import DX;
import RenderSystemFwd;
import ConstantBuffer;

export class ComputeContext
{
public:
    ComputeContext(RenderSystem& renderSystem, ID3D12GraphicsCommandList& commandList);

    void setupRoot();

    void setConstants(D3D12_GPU_VIRTUAL_ADDRESS gpuAddress);
    void setTexture(D3D12_GPU_DESCRIPTOR_HANDLE handle);
    void setUnorderedAccessView(D3D12_GPU_DESCRIPTOR_HANDLE handle);

    RenderSystem& renderSystem;
    ID3D12GraphicsCommandList& commandList;
};
