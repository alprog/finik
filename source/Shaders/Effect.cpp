module Effect;

import RenderSystem;
import Shader;
import PsoManager;
import ShaderManager;
import QualityManager;

Effect::Effect(String name)
    : name{name}
{
}

void Effect::setPipelineType(PipelineType type)
{
    if (this->type != type)
    {
        this->type = type;
        resetPso();
    }
}

void Effect::setVertexShader(Ptr<Shader> shader)
{
    if (vertexShader != shader)
    {
        vertexShader = shader;
        resetPso();
    }
}

void Effect::setPixelShader(Ptr<Shader> shader)
{
    if (pixelShader != shader)
    {
        pixelShader = shader;
        resetPso();
    }
}

Ptr<PipelineState> Effect::getPipelineState()
{
    if (pipelineState == nullptr)
    {
        ShaderByteCode pixelBytecode = pixelShader ? pixelShader->bytecode : ShaderByteCode();
        PipelineSettings settings(vertexShader->bytecode, pixelBytecode);
        settings.type = type;
        if (type == PipelineType::Geometry)
        {
            settings.msaa = QualityManager::GetInstance().getCurrent().msaa;
        }
        
        pipelineState = Single::Get<PSOManager>().get_pso(settings);
    }

    return pipelineState;
}
