export module shader_source_file;

import Finik.Core;
import asset;
import shader_manager;

// for intellisence

export class ShaderSourceFile : public Asset
{
public:
    using Asset::Asset;

    const std::string& GetSourceText() const
    {
        return sourceText;
    }

protected:
    void hot_reload(ByteBlob& blob) override
    {
        sourceText = blob.asString();
        ShaderManager::GetInstance().onSourceFileChanged();
    }

private:
    std::string sourceText;
};