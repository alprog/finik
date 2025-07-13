export module resource_storage;

import Finik.Core;
import gpu_resource;
import descriptor_heap;

export class ResourceStorage
{

private:
    Array<GpuResource*> Resources;

    DescriptorHeap Heap;
};