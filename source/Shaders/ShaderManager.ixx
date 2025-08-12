module;
#include "asserts.h"
export module ShaderManager;

import Finik.Core;
import Shader;
import AssetPath;
import EffectManager;
import ShaderCompiler;

export class ShaderManager : public Singleton<ShaderManager>
{
public:
    ShaderManager()
    {
        ShaderCompiler& compiler = Single::Get<ShaderCompiler>();
        fallbackVertexByteCode = compiler.Compile(getFallbackVertexShaderText(), ShaderType::Vertex, "VSMain").bytecode;
        fallbackPixelByteCode = compiler.Compile(getFallbackPixelShaderText(), ShaderType::Pixel, "PSMain").bytecode;
        fallbackComputeByteCode = compiler.Compile(getFallbackComputeShaderText(), ShaderType::Compute, "CSMain").bytecode;
        ASSERT(fallbackVertexByteCode.Get());
        ASSERT(fallbackPixelByteCode.Get());
        ASSERT(fallbackComputeByteCode.Get());
    }

    void update()
    {
        if (hotReloadNeeded)
        {
            hotReloadOutdated();
            hotReloadNeeded = false;
        }
    }

    Ptr<Shader> getVertexShader(AssetPath assetPath, const std::string& entryName)
    {
        return getShader({assetPath, ShaderType::Vertex, entryName});
    }

    Ptr<Shader> getPixelShader(AssetPath assetPath, const std::string& entryName)
    {
        return getShader({assetPath, ShaderType::Pixel, entryName});
    }

    Ptr<Shader> getComputeShader(AssetPath assetPath, const std::string& entryName)
    {
        return getShader({assetPath, ShaderType::Compute, entryName});
    }

    Ptr<Shader> getShader(const ShaderKey& key)
    {
        auto it = Shaders.find_value(key);
        if (it)
        {
            return *it;
        }

        auto shader = MakePtr<Shader>(key);
        Shaders[key] = shader;
        return shader;
    }

    void onSourceFileChanged()
    {
        hotReloadNeeded = true;
    }

    ShaderByteCode getFallbackByteCode(ShaderType type)
    {
        switch (type)
        {
        case ShaderType::Vertex:
            return fallbackVertexByteCode;

        case ShaderType::Pixel:
            return fallbackPixelByteCode;

        case ShaderType::Compute:
            return fallbackComputeByteCode;
        }
    }

    auto getAllShaders() const
    {
        return Shaders;
    }

private:
    const char* getFallbackVertexShaderText();
    const char* getFallbackPixelShaderText();
    const char* getFallbackComputeShaderText();

    int32 hotReloadOutdated()
    {
        int32 count = 0;
        for (auto& [_, shader] : Shaders)
        {
            if (shader->hotreloadDependencies.isOutdated())
            {
                shader->hotRecompile();
                EffectManager::GetInstance().onShaderChanged(shader);
                count++;
            }
        }
        return count;
    }

private:
    ShaderByteCode fallbackVertexByteCode;
    ShaderByteCode fallbackPixelByteCode;
    ShaderByteCode fallbackComputeByteCode;

    HashMap<ShaderKey, Ptr<Shader>> Shaders;
    bool hotReloadNeeded = false;
};
