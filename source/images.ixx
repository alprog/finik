export module images;

import core;
import texel;
import byte_blob;

export class Image
{
public:
    Image(int width, int height);
    ~Image();

    int width;
    int height;
    Texel* data;

    Texel& getTexel(int x, int y);

    void generateChessboard();

    size_t getByteSize() { return width * height * 4; };
};

export namespace Images
{
    std::shared_ptr<Image> loadPng(Path path);
    std::shared_ptr<Image> loadPng(ByteBlob& blob);
}
