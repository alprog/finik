module RenderEngine:CommandListPool;

import App;
import RenderSystem;

CommandListPool::CommandListPool(RenderEngine& engine)
    : engine{engine}
{
}

CommandList& CommandListPool::retrieveOne()
{
    auto frameIndex = App::GetInstance().getFrameIndex();
    
    if (freeLists.empty())
    {
        return *lists.emplace_back(MakeUnique<CommandList>(*this, frameIndex));
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