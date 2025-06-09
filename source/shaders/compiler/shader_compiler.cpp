module;
#include "gfx/dx.h"
#include "dxcapi.h"
module shader_compiler;

import assets;
import shader_source_file;
import byte_blob;
import asset_dependencies;
import include_handler;
import dxc_args;

// for intellisense

const char* getShaderVersion(ShaderType type)
{
    switch (type)
    {
    case Vertex:
        return "vs_6_6";
    case Pixel:
        return "ps_6_6";
    case Compute:
        return "cs_6_6";
    default:
        throw "invalid shader type";
    }
}

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

    IncludeHandler includeHandler(*this, output.sourceAssets);

    std::wstring wideEntryPoint = std::wstring(entryPoint.begin(), entryPoint.end());

    MyPtr<IDxcCompiler3> dxc;
    MyPtr<IDxcUtils> utils;
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));
    DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc));
    MyPtr<IDxcBlobEncoding> sourceBlob;
    utils->CreateBlob(sourceText.c_str(), sourceText.size(), DXC_CP_UTF8, &sourceBlob);
    DxcBuffer sourceBuffer;
    sourceBuffer.Ptr = sourceBlob->GetBufferPointer();
    sourceBuffer.Size = sourceBlob->GetBufferSize();
    sourceBuffer.Encoding = DXC_CP_UTF8;
    MyPtr<IDxcResult> result;

    DxcArgs args;
    args << "-E" << entryPoint;
    args << "-T" << getShaderVersion(type);
    args << "-Wno-conversion";

    dxc->Compile(&sourceBuffer, args.getPtrToPtrs(), args.count(), &includeHandler, IID_PPV_ARGS(&result));

    MyPtr<IDxcBlobUtf8> errors;
    result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);

    result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&output.bytecode), nullptr);

    if (errors && errors->GetStringLength() > 0)
    {
        output.errorMessage = errors->GetStringPointer();
    }

    return output;
}