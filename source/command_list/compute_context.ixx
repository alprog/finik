export module compute_context;

import Finik.Core;
import dx;
import render_system_fwd;
import constant_buffer;

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