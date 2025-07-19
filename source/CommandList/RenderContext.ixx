export module RenderContext;

import Finik.Core;
import DX;
import RenderSystemFwd;
import Material;
import ConstantBuffer;
import RenderPass;
import Effect;
import VertexBuffer;

// for intellisense

export class RenderContext
{
public:
    RenderContext(RenderSystem& renderSystem, ID3D12GraphicsCommandList& commandList);

    void setupRoot();
    void setFrameConstants(D3D12_GPU_VIRTUAL_ADDRESS gpuAddress);
    void setGBufferConstants(D3D12_GPU_VIRTUAL_ADDRESS gpuAddress);
    void setModelMatrix(const Matrix& matrix);
    void setModelMatrix(const Matrix& currentMatrix, const Matrix& prevMatrix);
    void setMaterial(const Material& material, RenderPass pass);
    void setEffect(Effect& effect);
    void drawMesh(Mesh* mesh);
    void drawLines(LineVertexBuffer* linesVertexBuffer);

    void drawFullScreenQuad();

    RenderSystem& renderSystem;
    ID3D12GraphicsCommandList& commandList;
};
