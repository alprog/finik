module CommandListPool;

import Finik.Core;
import App;

CommandListPool::CommandListPool(RenderSystem& renderSystem)
    : renderSystem{ renderSystem }
{
}

CommandList& CommandListPool::retrieveOne()
{
    auto frameIndex = App::GetInstance().getFrameIndex();
    
    if (freeLists.empty())
    {
        return *lists.emplace_back(std::make_unique<CommandList>(renderSystem, *this, frameIndex));
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
