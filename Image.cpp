#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    image_ = std::vector<std::vector<Color>>(h, std::vector<Color>(w, fill));
  }
  Image::~Image()
  {
  }
  int Image::width() const
  {
    return image_[0].size();
  }
  int Image::height() const
  {
    return image_.size();
  }

  Color& Image::at(int x, int y)
  {
    return image_[y][x];
  }

  const Color& Image::at(int x, int y) const
  {
    return image_[y][x];
  }

  Image::Image(const Image& original) {
    image_ = std::vector<std::vector<Color>>(original.height(), std::vector<Color>(original.width()));

    for (int row = 0; row < original.height(); row++) {
      for (int column = 0; column < original.width(); column++) {
        Color& color_copy = image_[row][column];
        Color color_ori = original.at(column, row);
        color_copy.red() = color_ori.red();
        color_copy.green() = color_ori.green();
        color_copy.blue() = color_ori.blue();
      }
    }
  }

}
