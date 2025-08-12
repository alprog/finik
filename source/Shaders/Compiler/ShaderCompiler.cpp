module;
#include "gfx/dx.h"
#include "dxcapi.h"
module ShaderCompiler;

import Assets;
import ShaderSourceFile;
import ByteBlob;
import AssetDependencies;
import IncludeHandler;
import DxcArgs;

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

ShaderCompiler::ShaderCompiler()
{
    DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pDxcInstance));
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pDxcUtils));
}

ShaderCompiler::~ShaderCompiler()
{
    if (pDxcInstance)
    {
        pDxcInstance->Release();
        pDxcInstance = nullptr;
    }
    if (pDxcUtils)
    {
        pDxcUtils->Release();
        pDxcUtils = nullptr;
    }
}

ShaderCompiler::Output ShaderCompiler::Compile(ShaderKey key)
{
    Ptr<ShaderSourceFile> source_file = Assets::GetInstance().get<ShaderSourceFile>(key.AssetPath);
    const String& sourceText = source_file->GetSourceText();

    auto output = Compile(sourceText, key.Type, key.EntryPoint);
    output.sourceAssets.add(source_file);
    return output;
}

ShaderCompiler::Output ShaderCompiler::Compile(const String& sourceText, ShaderType type, const String& entryPoint)
{
    Output output;
    
    IncludeHandler includeHandler([this, &output](const String& filename) {
        Ptr<ShaderSourceFile> sourceFile = Assets::GetInstance().get<ShaderSourceFile>(filename.c_str());
        output.sourceAssets.add(sourceFile);

        MyPtr<IDxcBlobEncoding> includeBlob;
        auto& includeText = sourceFile->GetSourceText();
        pDxcUtils->CreateBlob(includeText.c_str(), includeText.size(), DXC_CP_UTF8, &includeBlob);
        return includeBlob.Detach();
     });
    
    DxcBuffer sourceBuffer{&sourceText[0], sourceText.size(), DXC_CP_UTF8};

    DxcArgs args;
    args << "-E" << entryPoint;
    args << "-T" << getShaderVersion(type);
    args << "-Wno-conversion";
        
    MyPtr<IDxcResult> result;
    pDxcInstance->Compile(&sourceBuffer, args.getPtrToPtrs(), args.count(), &includeHandler, IID_PPV_ARGS(&result));

    result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&output.bytecode), nullptr);

    MyPtr<IDxcBlobUtf8> errors;
    result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
    if (errors && errors->GetStringLength() > 0)
    {
        output.errorMessage = errors->GetStringPointer();
    }

    return output;
}
