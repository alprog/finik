export module Execution:CommandListPool;

import Finik.Core;
import RenderSystemFwd;
import :CommandList;
import GfxDevice;

export class CommandListPool
{
public:
    explicit CommandListPool(GfxDevice& device);

    CommandList& retrieveOne();
    void putBack(CommandList& commandList);

private:
    GfxDevice& device;

    Array<UniquePtr<CommandList>> lists;
    Array<CommandList*> freeLists;
};
