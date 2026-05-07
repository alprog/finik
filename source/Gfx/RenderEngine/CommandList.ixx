export module RenderEngine:CommandList;

import :Shared;
import :GpuProfiler;

export class CommandList
{
public:
    explicit CommandList(CommandListPool& pool, int32 frameIndex);

    CommandList(const CommandList&) = delete;
    CommandList& operator=(const CommandList&) = delete;

    void reset(int32 frameIndex);
    void returnToPool();

    void startRecording(const char* label);
    void endRecording();

    int getFrameIndex() const;

    void transition(GpuResource& resource, D3D12_RESOURCE_STATES newState);

private:
    void startTimebox(const char* label);
    void endTimebox();

    int addTimestampQuery();

public:
    MyPtr<ID3D12GraphicsCommandList> listImpl;
    MyPtr<ID3D12CommandAllocator> commandAllocator;

private:
    CommandListPool& pool;
    int32 frameIndex;

    Array<int32> gpuTimeboxIndices;
};
