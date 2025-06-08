export module root_signature;

import core;
import dx;
import render_system_fwd;

export class RootSignature
{
protected:
    void init(RenderSystem& renderSystem, Array<CD3DX12_ROOT_PARAMETER>& parameters);

public:
    MyPtr<ID3D12RootSignature> signatureImpl;
};

export class MainRootSignature : public RootSignature
{
public:
    enum Params
    {
        // permanent
        UnboundTextureTable,
        UnboundTextureMSTable,
        MaterialsConstantBufferView,

        // per frame
        FrameConstantBufferView,

        // per draw
        MeshConstantBufferView,
        MaterialInlineConstants,

        Count
    };

    MainRootSignature(RenderSystem& renderSystem);
};

export class ComputeRootSignature : public RootSignature
{
public:
    enum Params
    {
        ShaderResourceView,
        UnorderedAccessView,
        ConstantBufferView,

        Count
    };

    ComputeRootSignature(RenderSystem& renderSystem);
};