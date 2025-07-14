module;
#include "asserts.h"
module Cleaner;

import App;

void Cleaner::add(IUnknown* object, int32 lastUseFrameIndex)
{
    records.emplace_back(object, lastUseFrameIndex);
}

void Cleaner::update()
{
    int32 currentFrame = App::GetInstance().getFrameIndex();
    for (int32 i = records.count() - 1; i >= 0; i--)
    {
        auto& record = records[i];
        if (currentFrame > record.lastUseFrameIndex + 1)
        {
            int32 refCounter = record.pointer->Release();
            ASSERT(refCounter == 0);
            records.swap_remove_at(i);
        }
    }
}
