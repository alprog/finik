export module ShaderSourceFile;

import Finik.Core;
import Asset;
import ShaderManager;

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
