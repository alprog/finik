export module ShaderSourceFile;

import Finik.Core;
import TextFile;
import ShaderManager;

export class ShaderSourceFile : public TextFile
{
public:
    using TextFile::TextFile;

    const String& GetSourceText() const
    {
        return fileText;
    }

protected:
    void hot_reload(ByteBlob& blob) override
    {
        TextFile::hot_reload(blob);
        ShaderManager::GetInstance().onSourceFileChanged();
    }
};
