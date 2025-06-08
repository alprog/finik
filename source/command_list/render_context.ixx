export module render_context;

import core;
import dx;
import render_system_fwd;
import material;
import constant_buffer;
import render_pass;

export class RenderContext
{
public:
    RenderContext(RenderSystem& renderSystem, ID3D12GraphicsCommandList& commandList);

    void setupRoot();
    void setFrameConstants(D3D12_GPU_VIRTUAL_ADDRESS gpuAddress);
    void setModelMatrix(const Matrix& matrix);
    void setMaterial(const Material& material, RenderPass pass);
    void drawMesh(Mesh* mesh);

    void drawFullScreenQuad();

    RenderSystem& renderSystem;
    ID3D12GraphicsCommandList& commandList;
};