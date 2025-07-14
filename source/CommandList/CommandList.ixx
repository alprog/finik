export module CommandList;

import DX;
import RenderSystemFwd;
import RenderContext;
import ComputeContext;

// for intellisense

export class CommandList
{
public:
    explicit CommandList(RenderSystem& renderSystem, CommandListPool& pool, int frameIndex);
    void reset(int frameIndex);
    void returnToPool();

    void startRecording();
    void endRecording();

    int getFrameIndex() const;

    RenderContext getRenderContext()
    {
        return RenderContext(renderSystem, *listImpl.Get());
    }

    ComputeContext getComputeContext()
    {
        return ComputeContext(renderSystem, *listImpl.Get());
    }

    void transition(GpuResource& resource, D3D12_RESOURCE_STATES newState);

private:
    int addTimestampQuery();

public:
    MyPtr<ID3D12GraphicsCommandList> listImpl;
    MyPtr<ID3D12CommandAllocator> commandAllocator;

private:
    RenderSystem& renderSystem;
    CommandListPool& pool;
    int frameIndex;

    int startTimestampIndex;
    int endTimestampIndex;
};
