export module RenderEngine:CommandList;

import :Shared;

export class CommandList
{
public:
    explicit CommandList(RenderEngine& engine, CommandListPool& pool, int frameIndex);
    void reset(int frameIndex);
    void returnToPool();

    void startRecording();
    void endRecording();

    int getFrameIndex() const;

    //RenderContext getRenderContext()
    //{
    //    return RenderContext(renderSystem, *listImpl.Get());
    //}

    //ComputeContext getComputeContext()
    //{
    //    return ComputeContext(renderSystem, *listImpl.Get());
    //}

    void transition(GpuResource& resource, D3D12_RESOURCE_STATES newState);

private:
    int addTimestampQuery();

public:
    MyPtr<ID3D12GraphicsCommandList> listImpl;
    MyPtr<ID3D12CommandAllocator> commandAllocator;

private:
    RenderEngine& engine;
    CommandListPool& pool;
    int frameIndex;

    int startTimestampIndex;
    int endTimestampIndex;
};
