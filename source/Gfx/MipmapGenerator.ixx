export module MipmapGenerator;

import Finik.Core;

import DX;
import Execution;
import Texture;

export class MipMapGenerator : public Singleton<MipMapGenerator>
{
public:
    MipMapGenerator();

    void Generate(Texture& texture, CommandList& commandList);

    MyPtr<ID3D12PipelineState> pso;
};
