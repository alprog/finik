module;
class DesktopWindow;
export module swap_chain;

import core;
import dx;
import descriptor_handle;
import render_system_fwd;
import render_surface;

export constexpr int NUM_BACK_BUFFER = 3;
export constexpr int NUM_FRAMES_IN_FLIGHT = 3;

export struct FrameContext
{
    ID3D12CommandAllocator* CommandAllocator;
    uint64                  FenceValue;
};

export struct SwapChainRenderTarget
{
    MyPtr<ID3D12Resource> resource;
    DescriptorHandle targetHandle;
};

export class SwapChain
{
public:
    SwapChain(DesktopWindow& window);
    ~SwapChain();

    void CreateRenderTargets();
    void CreateDepthStencil();
    void CleanupRenderTarget();
    void WaitForLastSubmittedFrame();
    FrameContext* WaitForNextFrameResources();

    void start_frame(ID3D12GraphicsCommandList* command_list);
    void finish_frame(ID3D12GraphicsCommandList* command_list);
    void execute(ID3D12GraphicsCommandList* command_list);
    void present();

public:
    DesktopWindow& window;

    MyPtr<IDXGISwapChain3> swapChain;

    HANDLE hSwapChainWaitableObject;
    Array<std::shared_ptr<SwapChainRenderTarget>> renderTargets;

    MyPtr<ID3D12Resource> depthStencil;
    DescriptorHandle depthStencilHandle;

    bool swapChainOccluded = false;
    FrameContext frameContext[NUM_FRAMES_IN_FLIGHT] = {};
    uint32 frameIndex = 0;
    FrameContext* current_frame_ctx;
}; 