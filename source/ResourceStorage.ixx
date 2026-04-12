export module ResourceStorage;

import Finik.Core;
import GpuResource;
import Descriptors;

export class ResourceStorage
{

private:
    Array<GpuResource*> Resources;

    DescriptorHeap Heap;
};
