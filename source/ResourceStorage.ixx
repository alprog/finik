export module ResourceStorage;

import Finik.Core;
import GpuResource;
import DescriptorHeap;

export class ResourceStorage
{

private:
    Array<GpuResource*> Resources;

    DescriptorHeap Heap;
};
