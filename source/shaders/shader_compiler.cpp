module;
#include "gfx/dx.h"
#include <d3dcompiler.h>
#include "dxcapi.h"
module shader_compiler;

import assets;
import shader_source_file;
import byte_blob;
import asset_dependencies;

std::string WideToUtf8(LPCWSTR wideStr)
{
    if (!wideStr)
        return {};

    int sizeNeeded = WideCharToMultiByte(
        CP_UTF8, // кодировка UTF-8
        0,
        wideStr,
        -1, // до нуля-терминатора
        nullptr,
        0,
        nullptr,
        nullptr);

    std::string result(sizeNeeded, 0);

    WideCharToMultiByte(
        CP_UTF8,
        0,
        wideStr,
        -1,
        &result[0],
        sizeNeeded,
        nullptr,
        nullptr);

    return result;
}

std::wstring Utf8ToWide(LPCSTR utf8Str)
{
    if (!utf8Str)
        return {};

    int sizeNeeded = MultiByteToWideChar(
        CP_UTF8, // Исходная кодировка (можно CP_ACP для ANSI)
        0,
        utf8Str,
        -1, // до нуля-терминатора
        nullptr,
        0);

    std::wstring result(sizeNeeded, 0);

    MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8Str,
        -1,
        &result[0],
        sizeNeeded);

    return result;
}

class IncludeHandler : public IDxcIncludeHandler
{
public:
    IncludeHandler(AssetDependencies& sourceAssets)
        : sourceAssets{sourceAssets}
    {
    }

    ULONG STDMETHODCALLTYPE AddRef() override
    {
        return 1;
    }
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


    HRESULT STDMETHODCALLTYPE LoadSource(
        _In_z_ LPCWSTR pFilename,                                // Candidate filename.
        _COM_Outptr_result_maybenull_ IDxcBlob** ppIncludeSource // Resultant source object for included file, nullptr if not found.
    ) override
    {
        auto ws = WideToUtf8(pFilename);
        const char* utf8Filename = ws.c_str();
        std::shared_ptr<ShaderSourceFile> sourceFile = Assets::GetInstance().get<ShaderSourceFile>(utf8Filename);
        sourceAssets.add(sourceFile);

        MyPtr<IDxcBlobEncoding> blob;
        MyPtr<IDxcUtils> utils;
        DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));
        utils->CreateBlob(
            sourceFile->GetSourceText().c_str(), 
            sourceFile->GetSourceText().size(), 
            DXC_CP_UTF8, 
            &blob
        );

        *ppIncludeSource = blob.Detach();

        return S_OK;
    }

    //HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData,
    //                       UINT* pBytes) override
    //{
    //    std::shared_ptr<ShaderSourceFile> sourceFile = Assets::GetInstance().get<ShaderSourceFile>(pFileName);
    //    sourceAssets.add(sourceFile);

    //    const std::string& sourceText = sourceFile->GetSourceText();

    //    *ppData = &sourceText[0];
    //    *pBytes = static_cast<UINT>(sourceText.size());
    //    return S_OK;
    //}

    //HRESULT __stdcall Close(LPCVOID pData) override
    //{
    //    return S_OK;
    //}

private:
    AssetDependencies& sourceAssets;
};

ShaderCompiler::Output ShaderCompiler::Compile(ShaderKey key)
{
    std::shared_ptr<ShaderSourceFile> source_file = Assets::GetInstance().get<ShaderSourceFile>(key.AssetPath);
    const String& sourceText = source_file->GetSourceText();

    auto output = Compile(sourceText, key.Type, key.EntryPoint);
    output.sourceAssets.add(source_file);
    return output;
}

ShaderCompiler::Output ShaderCompiler::Compile(const String& sourceText, ShaderType type, const String& entryPoint)
{
    Output output;

    IncludeHandler includeHandler(output.sourceAssets);


    std::wstring wideEntryPoint = Utf8ToWide(entryPoint.c_str());

    auto ttt = type == ShaderType::Compute ? L"cs_6_0" : ((type == ShaderType::Pixel ? L"ps_6_0" : L"vs_6_0"));

    {
        MyPtr<IDxcCompiler3> compiler;
        MyPtr<IDxcUtils> utils;
        DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));
        DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
        MyPtr<IDxcBlobEncoding> sourceBlob;
        utils->CreateBlob(sourceText.c_str(), sourceText.size(), DXC_CP_UTF8, &sourceBlob);
        DxcBuffer sourceBuffer;
        sourceBuffer.Ptr = sourceBlob->GetBufferPointer();
        sourceBuffer.Size = sourceBlob->GetBufferSize();
        sourceBuffer.Encoding = DXC_CP_UTF8;
        MyPtr<IDxcResult> result;
        LPCWSTR args[] = {
            L"-E", wideEntryPoint.c_str(),
            L"-T", ttt,
            L"-Wno-conversion"};

        compiler->Compile(
            &sourceBuffer,
            args, _countof(args),
            &includeHandler, // include handler
            IID_PPV_ARGS(&result));

        // Проверка на ошибки
        MyPtr<IDxcBlobUtf8> errors;
        result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);

        result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&output.bytecode), nullptr);

        if (errors && errors->GetStringLength() > 0)
        {
            output.errorMessage = errors->GetStringPointer();
        }
    }


    uint32 compileFlags = D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;

    ID3DBlob* errorBlob = nullptr;

    /*IncludeHandler includeHandler(output.sourceAssets);
    auto result = D3DCompile(&sourceText[0], sourceText.length(), entryPoint.c_str(), nullptr, &includeHandler,
                             entryPoint.c_str(), targets[type], compileFlags, 0, &output.bytecode, &errorBlob);

    if (FAILED(result))
    {
        if (errorBlob)
        {
            output.errorMessage = static_cast<char*>(errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        else
        {
            output.errorMessage = "unknown error";
        }
    }*/

    return output;
}