export module RenderContext;

import Finik.Core;
import DX;
import Material;
import ConstantBuffer;
import RenderPass;
import Effect;
import VertexBuffer;
import Mesh;
import RenderEngine;

export class RenderContext
{
public:
    RenderContext(RenderEngine& engine, ID3D12GraphicsCommandList& commandList);

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

    RenderEngine& engine;
    ID3D12GraphicsCommandList& commandList;
};
