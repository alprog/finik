module;
class DesktopWindow;
export module SwapChain;

import Finik.Core;
import GfxDevice;
import RenderSystem;
import RenderSurface;

export constexpr int NUM_BACK_BUFFER = 3;
export constexpr int NUM_FRAMES_IN_FLIGHT = 3;

export struct SwapChainRenderTarget
{
    MyPtr<ID3D12Resource> resource;
    DescriptorHandle handle;
};

export class SwapChain
{
public:
    SwapChain(DesktopWindow& window);
    ~SwapChain();

    void CreateRenderTargets();
    void WaitForNextFrameResources();

    void start_frame(CommandList& list);
    void finish_frame(CommandList& list);
    void execute(CommandList& list);
    void present();

public:
    DesktopWindow& window;

    MyPtr<IDXGISwapChain3> swapChain;

    HANDLE hSwapChainWaitableObject;
    Array<Ptr<SwapChainRenderTarget>> renderTargets;

    MyPtr<ID3D12Resource> depthStencil;
    DescriptorHandle depthStencilHandle;

    bool swapChainOccluded = false;

    std::queue<uint64> presentFenceValues;
}; 
