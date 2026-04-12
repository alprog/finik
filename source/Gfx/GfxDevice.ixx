export module GfxDevice;

import Finik.Core;
export import std;
export import DX;

import OneshotAllocator;
import Descriptors;

export class GfxDevice
{
public:
    void init();

    ID3D12Device* getInternal();
    DescriptorHeap* getRtvHeap();
    DescriptorHeap* getDsvHeap();
    DescriptorHeap* getCommonHeap();
    finik::gpumem::OneshotAllocator& getOneshotAllocator();

    ID3D12Device* operator->() const { return device.Get(); }

private:
    void enableDebugLayer();
    void createInternalDevice();
    void setupDebug();
    void createDescriptorHeaps();
    void createOneshotAllocator();

private:
    MyPtr<ID3D12Device> device;

    UniquePtr<DescriptorHeap> rtvHeap; // Render Target View
    UniquePtr<DescriptorHeap> dsvHeap; // Depth Stencil View
    UniquePtr<DescriptorHeap> srvCbvUavHeap;

    UniquePtr<finik::gpumem::OneshotAllocator> oneshotAllocator;
};