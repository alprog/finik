export module RenderEngine:CommandList;

import :Shared;
import :GpuProfiler;

export class CommandList
{
public:
    explicit CommandList(CommandListPool& pool, int frameIndex);

    CommandList(const CommandList&) = delete;
    CommandList& operator=(const CommandList&) = delete;

    void reset(int frameIndex);
    void returnToPool();

    void startRecording();
    void endRecording();

    int getFrameIndex() const;

    void transition(GpuResource& resource, D3D12_RESOURCE_STATES newState);

private:
    int addTimestampQuery();

public:
    MyPtr<ID3D12GraphicsCommandList> listImpl;
    MyPtr<ID3D12CommandAllocator> commandAllocator;

private:
    CommandListPool& pool;
    int frameIndex;

    int32 gpuTimeboxIndex;
};
