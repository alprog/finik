module;
#include "backends/imgui_impl_dx12.h"
#include "dx.h"

module PsoManager;

import RenderSystem;
import RootSignature;
import GBuffer;

Ptr<PipelineState> PSOManager::get_pso(const PipelineSettings& settings)
{
    auto it = states.find_value(settings);
    if (it)
    {
        return *it;
    }

    Ptr<PipelineState> result;
    if (settings.type == PipelineType::Geometry)
    {
        result = standardCompile(settings);
    }
    else if (settings.type == PipelineType::Shadow)
    {
        result = standardCompile(settings);
    }
    else if (settings.type == PipelineType::Lighting)
    {
        result = standardCompile(settings);
    }
    else if (settings.type == PipelineType::DebugLines)
    {
        result = standardCompile(settings);
    }
    else if (settings.type == PipelineType::ScreenSpace)
    {
        result = standardCompile(settings);
    }
    else if (settings.type == PipelineType::Imgui)
    {
        result = imguiCompile(settings);
    }
    else
    {
        throw;
    }
    states[settings] = result;
    return result;
}

Ptr<PipelineState> PSOManager::standardCompile(const PipelineSettings& settings)
{
    auto& renderSystem = Single::Get<RenderSystem>();
    auto device = renderSystem.get_device();

    Array<D3D12_INPUT_ELEMENT_DESC> inputElementDescs = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 1, DXGI_FORMAT_R32G32_UINT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}};

    if (settings.type == PipelineType::DebugLines)
    {
        inputElementDescs = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}};
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = {&inputElementDescs[0], (uint32)inputElementDescs.count()};
    psoDesc.pRootSignature = renderSystem.getRootSignature().signatureImpl.Get();

    psoDesc.VS = CD3DX12_SHADER_BYTECODE(settings.vertexByteCode.Get());
    if (settings.pixelByteCode)
    {
        psoDesc.PS = CD3DX12_SHADER_BYTECODE(settings.pixelByteCode.Get());
    }
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

    auto frontOrder = WindingOrder::CCW; // natural for right-handed. front face normals to the viewer (a^b = n)
    auto gapiFrontOrder = reverse(frontOrder); // but we use flipped viewport, so reverse it
    psoDesc.RasterizerState.FrontCounterClockwise = gapiFrontOrder == WindingOrder::CCW;

    if (settings.type == PipelineType::Geometry)
    {
        D3D12_BLEND_DESC blendDesc = {};
        blendDesc.AlphaToCoverageEnable = TRUE;
        blendDesc.IndependentBlendEnable = FALSE;
        D3D12_RENDER_TARGET_BLEND_DESC rtBlend = {};
        rtBlend.BlendEnable = FALSE;
        rtBlend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        blendDesc.RenderTarget[0] = rtBlend;
        psoDesc.BlendState = blendDesc;
    }
    else
    {
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    }

    CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc(D3D12_DEFAULT);
    depthStencilDesc.DepthEnable = settings.type != PipelineType::ScreenSpace && settings.type != PipelineType::DebugLines;
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    depthStencilDesc.StencilEnable = FALSE;
    psoDesc.DepthStencilState = depthStencilDesc;

    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    if (settings.type == PipelineType::DebugLines)
    {
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    }

    psoDesc.NumRenderTargets = 4;

    const auto& RTFormats = GBuffer::GetRTFormats();
    for (int32 i = 0; i < psoDesc.NumRenderTargets; i++)
    {
        psoDesc.RTVFormats[i] = RTFormats[i];
    }
    psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    psoDesc.SampleDesc.Count = getSampleCount(settings.msaa);

    ID3D12PipelineState* pipelineState = nullptr;
    device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState)) MUST;
    return MakePtr<PipelineState>(pipelineState);
}

Ptr<PipelineState> PSOManager::imguiCompile(const PipelineSettings& settings)
{
    auto& renderSystem = Single::Get<RenderSystem>();
    auto device = renderSystem.get_device();

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    FillImguiPsoDesc(psoDesc);

    psoDesc.VS = CD3DX12_SHADER_BYTECODE(settings.vertexByteCode.Get());
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(settings.pixelByteCode.Get());
    
    ID3D12PipelineState* pipelineState = nullptr;
    device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState)) MUST;
    return MakePtr<PipelineState>(pipelineState);
}

