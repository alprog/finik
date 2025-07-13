export module byte_blob;

import Finik.Core;

export class ByteBlob
{
public:
    ByteBlob(Path path);

    void* data() { return &str[0]; }
    size_t size() { return str.size(); };

    bool empty() { return size() == 0; }

    const std::string& asString() const { return str; }

private:
    std::string str;
};
