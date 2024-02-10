#include "Image.h"
#include "stb_image.h"

int Image::Clamp(int x, int low, int high)
{
    if (x < low)
    {
        return low;
    }

    if (x < high)
    {
        return x;
    }

    return high - 1;
}

Image::Image(const char *image_filename)
{
    std::string filename(image_filename);

    if (Load(filename))
        return;
    if (Load("images" + filename))
        return;
    if (Load("../images" + filename))
        return;
    if (Load("../../images" + filename))
        return;
    if (Load("../../../images" + filename))
        return;
    if (Load("../../../../images" + filename))
        return;
    if (Load("../../../../../images" + filename))
        return;
    if (Load("../../../../../../images" + filename))
        return;

    std::cerr << "ERROR: Could not load image file '" << image_filename << "'.\n";
}

bool Image::Load(const std::string filename)
{
    auto n = bytes_per_pixel;
    data = stbi_load(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
    bytes_per_scanline = image_width * bytes_per_pixel;
    return data != nullptr;
}

int Image::Width() const
{
    return (data == nullptr) ? 0 : image_width;
}

int Image::Height() const
{
    return (data == nullptr) ? 0 : image_height;
}

const unsigned char *Image::PixelData(int x, int y) const
{
    static unsigned char empty_data_result[] = {255, 0, 255};

    if (data == nullptr)
    {
        return empty_data_result;
    }

    x = Clamp(x, 0, image_width);
    y = Clamp(y, 0, image_height);

    return data + y * bytes_per_scanline + x * bytes_per_pixel;
}

bool Image::IsEmpty() const
{
    return data == nullptr;
}

Image::~Image()
{
    STBI_FREE(data);
}
