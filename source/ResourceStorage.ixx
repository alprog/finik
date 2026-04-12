export module ResourceStorage;

import Finik.Core;
import GfxDevice;
import GpuResource;

export class ResourceStorage
{

private:
    Array<GpuResource*> Resources;

    DescriptorHeap Heap;
};
