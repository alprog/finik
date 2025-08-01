export module RootSignature;

import Finik.Core;
import DX;
import RenderSystemFwd;

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
        MaterialsConstantBufferView, // b0

        // per frame
        FrameConstantBufferView, // b1 Camera?
        GBufferConstantBufferView, // b2

        // per draw
        MeshConstantBufferView, // b3
        MaterialInlineConstants, // b4

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
