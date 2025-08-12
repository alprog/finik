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
    Ptr<PipelineState> get_pso(const PipelineSettings& settings);

private:
    Ptr<PipelineState> standardCompile(const PipelineSettings& settings);
    Ptr<PipelineState> imguiCompile(const PipelineSettings& settings);

private:
    HashMap<PipelineSettings, Ptr<PipelineState>> states;
};
