export module TextFile;

import Finik.Core;
import Asset;

export class TextFile : public Asset
{
public:
    using Asset::Asset;

    const String& GetFileText() const
    {
        return fileText;
    }

protected:
    void hot_reload(ByteBlob& blob) override
    {
        fileText = blob.asString();
    }

protected:
    String fileText;
};
