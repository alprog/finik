export module PsoManager;

import Finik.Core;
import DX;
import PipelineSettings;
import PipelineState;
import WindingOrder;

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
