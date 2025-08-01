export module Images;

import Finik.Core;
import Texel;
import ByteBlob;

export class Image
{
public:
    Image(int width, int height);
    ~Image();

    int width;
    int height;
    Texel* data;

    Texel& getTexel(int32 index);
    Texel& getTexel(int x, int y);
    
    void generateChessboard();

    int getTexelCount() const { return width * height; }
    size_t getByteSize() const { return getTexelCount() * sizeof(Texel); };
};

export namespace Images
{
    std::shared_ptr<Image> loadPng(Path path);
    std::shared_ptr<Image> loadPng(ByteBlob& blob);
}
