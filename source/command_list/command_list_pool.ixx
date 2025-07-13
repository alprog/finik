export module command_list_pool;

import Finik.Core;
import render_system_fwd;
import command_list;

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