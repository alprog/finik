export module CommandListPool;

import Finik.Core;
import RenderSystemFwd;
import CommandList;

// for intellisense

export class CommandListPool
{
    friend class CommandList;

public:
    explicit CommandListPool(RenderSystem& renderSystem);

    CommandList& retrieveOne();
    void putBack(CommandList& commandList);

private:
    RenderSystem& renderSystem;

    Array<UniquePtr<CommandList>> lists;
    Array<CommandList*> freeLists;
};
