export module Effect;

import Finik.Core;
import RenderSystemFwd;
import PipelineSettings;
import ConstantBuffer;
import Myptr;
import DX;
import PipelineState;

// for intellisense

export class Effect
{
public:
    Effect() = default;
    Effect(String name);
 
    String name;

    void setPipelineType(PipelineType type);
    void setVertexShader(Ptr<Shader> shader);
    void setPixelShader(Ptr<Shader> shader);
    
    PipelineType getPipelineType() const
    {
        return type;
    }

    Ptr<Shader> getVertexShader() const
    {
        return vertexShader;
    }

    Ptr<Shader> getPixelShader() const
    {
        return pixelShader;
    }

    void resetPso()
    {
        pipelineState = nullptr;
    }

    Ptr<PipelineState> getPipelineState();

private:
    PipelineType type = PipelineType::Geometry;
    Ptr<Shader> vertexShader;
    Ptr<Shader> pixelShader;

    Ptr<PipelineState> pipelineState;
};
