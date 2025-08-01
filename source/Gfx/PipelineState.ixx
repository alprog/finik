export module PipelineState;

import Finik.Core;
import DX;

export class PipelineState
{
public:
    PipelineState(ID3D12PipelineState* internalObject);
    virtual ~PipelineState();

    ID3D12PipelineState* getInternalObject() const
    {
        return internalObject;
    }

    void use();

private:
    ID3D12PipelineState* internalObject = nullptr;
    int32 lastUseFrameIndex = 0;
};
