export module command_queue;

import Finik.Core;
import dx;
import fence;
import render_system_fwd;

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