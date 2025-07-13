export module pso_manager;

import Finik.Core;
import dx;
import pipeline_settings;
import pipeline_state;

// for intellisense

export class PSOManager
{
public:
    std::shared_ptr<PipelineState> get_pso(const PipelineSettings& settings);

private:
    std::shared_ptr<PipelineState> standardCompile(const PipelineSettings& settings);
    std::shared_ptr<PipelineState> imguiCompile(const PipelineSettings& settings);

private:
    HashMap<PipelineSettings, std::shared_ptr<PipelineState>> states;
};