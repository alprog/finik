export module effect;

import Finik.Core;
import render_system_fwd;
import pipeline_settings;
import constant_buffer;
import myptr;
import dx;
import pipeline_state;

// for intellisense

export class Effect
{
public:
    Effect() = default;
    Effect(String name);
 
    String name;

    void setPipelineType(PipelineType type);
    void setVertexShader(std::shared_ptr<Shader> shader);
    void setPixelShader(std::shared_ptr<Shader> shader);
    
    PipelineType getPipelineType() const
    {
        return type;
    }

    std::shared_ptr<Shader> getVertexShader() const
    {
        return vertexShader;
    }

    std::shared_ptr<Shader> getPixelShader() const
    {
        return pixelShader;
    }

    void resetPso()
    {
        pipelineState = nullptr;
    }

    std::shared_ptr<PipelineState> getPipelineState();

private:
    PipelineType type = PipelineType::Geometry;
    std::shared_ptr<Shader> vertexShader;
    std::shared_ptr<Shader> pixelShader;

    std::shared_ptr<PipelineState> pipelineState;
};
