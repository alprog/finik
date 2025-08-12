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
    UniquePtr<Fence> fence;

    UniquePtr<Fence> frameFence;

    std::queue<CommandList*> executionQueue;
};
