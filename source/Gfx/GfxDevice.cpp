module;
#include <SDL.h>
#include <SDL_syswm.h>

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#include "dx.h"
#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif
module GfxDevice;

void GfxDevice::init()
{
    enableDebugLayer();
    createInternalDevice();
    setupDebug();
    createDescriptorHeaps();
    createOneshotAllocator();
}

void GfxDevice::enableDebugLayer()
{
    MyPtr<ID3D12Debug> debugInterface;
    auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface));
    if (SUCCEEDED(result))
    {
        debugInterface->EnableDebugLayer();
    }
}

void GfxDevice::createInternalDevice()
{
    MyPtr<IDXGIFactory4> factory;
    auto result = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(result))
        throw;

    MyPtr<IDXGIAdapter1> adapter;
    for (uint32 adapterIndex = 0; DXGI_ERROR_NOT_FOUND != factory.Get()->EnumAdapters1(adapterIndex, &adapter); adapterIndex++)
    {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0)
        {
            auto result = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
            if (SUCCEEDED(result))
            {
                break;
            }
        }
    }

    MyPtr<IDXGIAdapter1> hardwareAdapter;
    hardwareAdapter = adapter.Detach();

    result = D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
    if (FAILED(result))
        throw;
}

void GfxDevice::setupDebug()
{
#ifdef DX12_ENABLE_DEBUG_LAYER
    MyPtr<ID3D12InfoQueue> pInfoQueue;
    if (SUCCEEDED(device.As(pInfoQueue)))
    {
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

        // Suppress whole categories of messages
        //D3D12_MESSAGE_CATEGORY Categories[] = {};

        // Suppress messages based on their severity level
        D3D12_MESSAGE_SEVERITY Severities[] =
            {
                D3D12_MESSAGE_SEVERITY_INFO};

        // Suppress individual messages by their ID
        D3D12_MESSAGE_ID DenyIds[] = {
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE, // I'm really not sure how to avoid this message.
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                     // This warning occurs when using capture frame while graphics debugging.
        };

        D3D12_INFO_QUEUE_FILTER NewFilter = {};
        //NewFilter.DenyList.NumCategories = _countof(Categories);
        //NewFilter.DenyList.pCategoryList = Categories;
        NewFilter.DenyList.NumSeverities = _countof(Severities);
        NewFilter.DenyList.pSeverityList = Severities;
        NewFilter.DenyList.NumIDs = _countof(DenyIds);
        NewFilter.DenyList.pIDList = DenyIds;

        auto result = pInfoQueue->PushStorageFilter(&NewFilter);
        if (FAILED(result))
            throw;
    }
#endif
}

void GfxDevice::createDescriptorHeaps()
{
    rtvHeap = MakeUnique<DescriptorHeap>(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 20);
    dsvHeap = MakeUnique<DescriptorHeap>(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 5);
    srvCbvUavHeap = MakeUnique<DescriptorHeap>(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1000);
}

void GfxDevice::createOneshotAllocator()
{
    oneshotAllocator = MakeUnique<finik::gpumem::OneshotAllocator>(device.Get());
}

ID3D12Device* GfxDevice::getInternal()
{
    return device.Get();
}

DescriptorHeap* GfxDevice::getRtvHeap()
{
    return rtvHeap.get();
}

DescriptorHeap* GfxDevice::getDsvHeap()
{
    return dsvHeap.get();
}

DescriptorHeap* GfxDevice::getCommonHeap()
{
    return srvCbvUavHeap.get();
}

finik::gpumem::OneshotAllocator& GfxDevice::getOneshotAllocator()
{
    return *oneshotAllocator;
}
