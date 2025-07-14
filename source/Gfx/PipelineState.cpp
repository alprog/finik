module PipelineState;

import App;
import Cleaner;

PipelineState::PipelineState(ID3D12PipelineState* internalObject)
    : internalObject{internalObject}
{
}

PipelineState::~PipelineState()
{
    Cleaner::GetInstance().add(internalObject, lastUseFrameIndex);
    internalObject = nullptr;
}

void PipelineState::use()
{
    lastUseFrameIndex = App::GetInstance().getFrameIndex();
}
