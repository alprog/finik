module;
class DesktopWindow;
export module SwapChain;

import Finik.Core;
import GfxDevice;
import RenderSystem;
import RenderSurface;
import GpuResource;
import SwapChainBackBuffer;

export constexpr int NUM_BACK_BUFFER = 3;
export constexpr int NUM_FRAMES_IN_FLIGHT = 3;

export class SwapChain
{
public:
    SwapChain(DesktopWindow& window);
    ~SwapChain();

    void resize();

    void createBackBuffers();
    void WaitForNextFrameResources();

    void start_frame(CommandList& commandList);
    void finish_frame(CommandList& commandList);
    void execute(CommandList& commandList);
    void present();

public:
    DesktopWindow& window;
    
    MyPtr<IDXGISwapChain3> swapChain;
    IntSize resolution;

    HANDLE hSwapChainWaitableObject;
    Array<Ptr<SwapChainBackBuffer>> backBuffers;

    MyPtr<ID3D12Resource> depthStencil;
    DescriptorHandle depthStencilHandle;

    bool swapChainOccluded = false;

    std::queue<uint64> presentFenceValues;
}; 
