export module CommandListPool;

import Finik.Core;
import RenderSystemFwd;
import CommandList;

export class CommandListPool
{
    friend class CommandList;

public:
    explicit CommandListPool(RenderSystem& renderSystem);

    CommandList& retrieveOne();
    void putBack(CommandList& commandList);

private:
    RenderSystem& renderSystem;

    Array<std::unique_ptr<CommandList>> lists;
    Array<CommandList*> freeLists;
};
