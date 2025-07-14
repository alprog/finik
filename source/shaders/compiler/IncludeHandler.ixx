module;
#include "gfx/dx.h"
#include "dxcapi.h"
export module IncludeHandler;

import Finik.Core;

// for intellisense

template <typename T>
class DefaultQueryInterface : public T
{
    ULONG STDMETHODCALLTYPE AddRef() override
    {
        return 1;
    };
    ULONG STDMETHODCALLTYPE Release() override
    {
        return 1;
    }
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override
    {
        if (riid == __uuidof(T) || riid == IID_IUnknown)
        {
            *ppvObject = this;
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
};

export class IncludeHandler : public DefaultQueryInterface<IDxcIncludeHandler>
{
public:
    using TCallback = std::function<IDxcBlob*(const String&)>;

    IncludeHandler(TCallback callback)
        : callback{callback}
    {
    }

    HRESULT STDMETHODCALLTYPE LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource) override
    {
        const std::wstring wideFilename = pFilename;
        const std::string filename = std::string(wideFilename.begin(), wideFilename.end());
        *ppIncludeSource = callback(filename);
        return S_OK;
    }

private:
    TCallback callback;
};
