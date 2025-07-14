module;
#include "png.h"
module Images;

import ByteBlob;

#define PNGSIGSIZE 8

Image::Image(int width, int height)
    : width{width}
    , height{height}
{
    data = new Texel[width * height];
}

Image::~Image()
{
    delete[] data;
}

Texel& Image::getTexel(int32 index)
{
    return data[index];
}

Texel& Image::getTexel(int x, int y)
{
    return data[y * width + x];
}

void Image::generateChessboard()
{
    int cellWidth = width / 8;
    int cellHeight = height / 8;

    int texelCount = width * height;

    int index = 0;
    for (int y = 0; y < height; y++)
    {
        bool oddColumn = (y / cellHeight) % 2;
        for (int x = 0; x < width; x++)
        {
            bool oddRow = (x / cellWidth) % 2;
            data[index++] = (oddColumn == oddRow) ? Texel::Green : Texel::Blue;
        }
    }
}

bool validate(std::istream& source)
{
    png_byte pngsig[PNGSIGSIZE];

    source.read((char*)pngsig, PNGSIGSIZE);

    if (source.good())
    {
        auto cmp = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
        if (cmp == 0)
        {
            return true;
        }
    }

    return false;
}

void readData(png_structp pngPtr, png_bytep data, png_size_t length)
{
    auto stream = (std::istream*)png_get_io_ptr(pngPtr);
    stream->read((char*)data, length);
}

std::shared_ptr<Image> Images::loadPng(Path path)
{
    ByteBlob blob(path);
    return loadPng(blob);
}

std::shared_ptr<Image> Images::loadPng(ByteBlob& blob)
{
    std::istringstream inputStream(blob.asString());
    if (validate(inputStream))
    {
        auto pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        auto infoPtr = png_create_info_struct(pngPtr);

        if (setjmp(png_jmpbuf(pngPtr)))
        {
            return nullptr;
        }

        png_set_read_fn(pngPtr, (png_voidp)&inputStream, readData);

        png_set_sig_bytes(pngPtr, PNGSIGSIZE);
        png_read_info(pngPtr, infoPtr);

        auto imgWidth = png_get_image_width(pngPtr, infoPtr);
        auto imgHeight = png_get_image_height(pngPtr, infoPtr);
        auto bitdepth = png_get_bit_depth(pngPtr, infoPtr);
        auto channels = png_get_channels(pngPtr, infoPtr);
        auto color_type = png_get_color_type(pngPtr, infoPtr);

        printf("width %i, heigth %i, bitdepth %i, channels %i, type %i", imgWidth, imgHeight, bitdepth, channels,
               color_type);
        fflush(stdout);

        const unsigned int stride = imgWidth * bitdepth * channels / 8;

        auto image = std::make_shared<Image>(imgWidth, imgHeight);

        auto rowPtrs = new png_bytep[imgHeight];
        for (size_t i = 0; i < imgHeight; i++)
        {
            rowPtrs[i] = (png_bytep)image->data + i * stride;
        }

        png_read_image(pngPtr, rowPtrs);

        delete[] (png_bytep)rowPtrs;
        png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);

        return image;
    }

    return nullptr;
}
