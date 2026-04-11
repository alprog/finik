export module ShaderCompiler;

import Finik.Core;
import AssetDependencies;
import AssetPath;
import Shader;

extern "C++" class IDxcCompiler3;
extern "C++" class IDxcUtils;

// for intellisense

export class ShaderCompiler
{
    friend class Single;

private:
    ShaderCompiler();
    ~ShaderCompiler();

public:
    struct Output
    {
        ShaderByteCode bytecode;
        AssetDependencies sourceAssets;
        String errorMessage;
    };

    Output Compile(ShaderKey key);
    Output Compile(const String& sourceText, ShaderType type, const String& entryPoint);

    void AddIncludeDirectory(AssetPath directory)
    {
        IncludeDirectories.append(directory);
    }

private:
    Array<AssetPath> IncludeDirectories;
    IDxcCompiler3* pDxcInstance;
    IDxcUtils* pDxcUtils;
    static int32 Counter;
};
