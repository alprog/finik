module;
class IDxcCompiler3;
class IDxcUtils;
export module shader_compiler;

import Finik.Core;
import asset_dependencies;
import asset_path;
import shader;

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
