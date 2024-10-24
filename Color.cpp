#include "Color.hpp"

namespace prog {
    Color::Color() {
        red_ = green_ = blue_ = 0;
    }
    Color::Color(const Color& other) {
        red_ = other.red_;
        green_ = other.green_;
        blue_ = other.blue_;
    }
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        red_ = red;
        green_ = green;
        blue_ = blue;
    }
    rgb_value Color::red() const {
        return red_;
    }
    rgb_value Color::green() const {
        return green_;
    }
    rgb_value Color::blue() const {
        return blue_;
    }

    rgb_value& Color::red()  {
        return red_;
    }
    rgb_value& Color::green()  {
      return green_;
    }
    rgb_value& Color::blue()  {
      return blue_;
    }

    bool Color::operator==(const Color& color_2) const {
        return ((red_*1000000+green_*1000+blue_) == (color_2.red()*1000000+color_2.green()*1000+color_2.blue()));
    }
    bool Color::operator<(const Color& color_2) const {
        return ((red_*1000000+green_*1000+blue_) < (color_2.red()*1000000+color_2.green()*1000+color_2.blue()));
    }
}
