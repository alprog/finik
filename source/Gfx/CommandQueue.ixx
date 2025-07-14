export module CommandQueue;

import Finik.Core;
import DX;
import Fence;
import RenderSystemFwd;

export class CommandQueue
{
public:
    CommandQueue(RenderSystem& renderSystem);

    void execute(CommandList& commandList);
    void freeCompletedLists();

    ID3D12CommandQueue* operator->();
    
    void Flush();

    MyPtr<ID3D12CommandQueue> queueImpl;
    std::unique_ptr<Fence> fence;

    std::unique_ptr<Fence> frameFence;

    std::queue<CommandList*> executionQueue;
};
