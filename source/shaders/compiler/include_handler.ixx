module;
#include "gfx/dx.h"
#include "dxcapi.h"
export module include_handler;

import core;
import dx;
import assets;
import shader_source_file;
import byte_blob;
import asset_dependencies;

// for intellisense

export class ShaderCompiler;

export class IncludeHandler : public IDxcIncludeHandler
{
public:
    IncludeHandler(ShaderCompiler& compiler, AssetDependencies& sourceAssets)
        : compiler{compiler}
        , sourceAssets{sourceAssets}
    {
    }

    // IUnknown
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
        if (riid == __uuidof(IDxcIncludeHandler) || riid == IID_IUnknown)
        {
            *ppvObject = this;
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    HRESULT STDMETHODCALLTYPE LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource) override
    {
        auto ws = std::wstring(pFilename);
        const std::string utf8Filename = std::string(ws.begin(), ws.end());
        std::shared_ptr<ShaderSourceFile> sourceFile = Assets::GetInstance().get<ShaderSourceFile>(utf8Filename);
        sourceAssets.add(sourceFile);

        MyPtr<IDxcBlobEncoding> blob;
        MyPtr<IDxcUtils> utils;
        DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));

        utils->CreateBlob(
            sourceFile->GetSourceText().c_str(),
            sourceFile->GetSourceText().size(),
            DXC_CP_UTF8,
            &blob);

        *ppIncludeSource = blob.Detach();

        return S_OK;
    }

private:
    ShaderCompiler& compiler;
    AssetDependencies& sourceAssets;
};