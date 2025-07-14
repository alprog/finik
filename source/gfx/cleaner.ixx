export module Cleaner;

import Finik.Core;
import DX;

export class Cleaner : public Singleton<Cleaner>
{
    struct Record
    {
        IUnknown* pointer;
        int32 lastUseFrameIndex;
    };

public:
    void add(IUnknown* object, int32 lastUseFrameIndex);

    void update();

private:
    Array<Record> records;
};
