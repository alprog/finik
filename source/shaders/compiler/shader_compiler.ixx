module;
class IDxcCompiler3;
class IDxcUtils;
export module shader_compiler;

import core;
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
        InlcudeDirectories.append(directory);
    }

private:
    Array<AssetPath> InlcudeDirectories;
    IDxcCompiler3* pDxcInstance;
    IDxcUtils* pDxcUtils;
    static int32 Counter;
};
