module Execution:CommandListPool;

import Finik.Core;
import App;

CommandListPool::CommandListPool(GfxDevice& device)
    : device{device}
{
}

CommandList& CommandListPool::retrieveOne()
{
    auto frameIndex = App::GetInstance().getFrameIndex();
    
    if (freeLists.empty())
    {
        return *lists.emplace_back(MakeUnique<CommandList>(device, *this, frameIndex));
    }

    auto& list = *freeLists.last();
    freeLists.remove_last();
    list.reset(frameIndex);
    return list;
}

void CommandListPool::putBack(CommandList& commandList)
{
    freeLists.append(&commandList);
}
