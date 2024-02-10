#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG

#include <cstdlib>
#include <iostream>

class Image
{
public:
    static int Clamp(int x, int low, int high);

public:
    Image(const char *image_filename);

    bool Load(const std::string filename);

    int Width() const;
    int Height() const;

    const unsigned char *PixelData(int x, int y) const;

    bool IsEmpty() const;

    ~Image();

private:
    const int bytes_per_pixel = 3;
    unsigned char *data = nullptr;
    int image_width = 0, image_height = 0;
    int bytes_per_scanline = 0;
};
