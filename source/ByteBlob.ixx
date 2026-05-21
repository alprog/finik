export module ByteBlob;

import Finik.Core;

export class ByteBlob
{
public:
    ByteBlob(Path path);

    void* data() { return &str[0]; }
    size_t size() { return str.size(); };

    bool empty() { return size() == 0; }

    const String& asString() const { return str; }

private:
    String str;
};
