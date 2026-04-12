export module RenderEngine:CommandListPool;

import :Shared;

export class CommandListPool
{
public:
    explicit CommandListPool(RenderEngine& engine);

    CommandList& retrieveOne();
    void putBack(CommandList& commandList);

    RenderEngine& getEngine() { return engine; }

private:
    RenderEngine& engine;

    Array<UniquePtr<CommandList>> lists;
    Array<CommandList*> freeLists;
};
