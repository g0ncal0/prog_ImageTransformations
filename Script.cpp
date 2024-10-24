#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <algorithm>

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            
            if (command == "invert") {
                invert();
                continue;
            }

            if (command == "to_gray_scale") {
                to_gray_scale();
                continue;
            }

            if (command == "replace") {
                replace();
                continue;
            }

            if (command == "fill") {
                fill();
                continue;
            }

            if (command == "h_mirror") {
                h_mirror();
                continue;
            }

            if (command == "v_mirror") {
                v_mirror();
                continue;
            }

            if (command == "add") {
                add();
                continue;
            } 

            if (command == "crop") {
                crop();
                continue;
            } 

            if (command == "rotate_left") {
                rotate_left();
                continue;
            } 

            if (command == "rotate_right") {
                rotate_right();
                continue;
            } 

            if (command == "median_filter") {
                median_filter();
                continue;
            }

            if (command == "xpm2_open") {
                xpm2_open();
                continue;
            }

            if (command == "xpm2_save") {
                xpm2_save();
                continue;
            }

        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }

    void Script::invert() {
        // Invert the colors of the currennt image
        for (int row = 0; row < image->height(); row++) {
            for (int column = 0; column < image->width(); column++) { // NOTE: We used these nested for loops with few changes in almost all functions to iterate all the pixels of the image
                Color& aux = image->at(column, row);

                aux.red() = 255-aux.red();
                aux.green() = 255-aux.green();
                aux.blue() = 255-aux.blue();
            }
        }
    }

    void Script::to_gray_scale() {
        // Transforms the current image to grayscale
        for (int row = 0; row < image->height(); row++) {
            for (int column = 0; column < image->width(); column++) {
                Color& aux = image->at(column, row);
                int average = (aux.red() + aux.green() + aux.blue())/3;
                rgb_value v = (rgb_value)average;
                aux = {v, v, v};
            }
        }
    }

    void Script::replace() {
        // Replaces all occurrences of one color in current image by another
        Color old, update;
        input >> old >> update;

        for (int row = 0; row < image->height(); row++) {
            for (int column = 0; column < image->width(); column++) {
                Color& aux = image->at(column, row);
                if (aux == old)  aux = update;
            }
        }
    }

    void Script::fill() {
        // Fill a rectangle in the current image with a given color
        int x, y, w, h;
        Color update;
        input >> x >> y >> w >> h >> update;

        for (int row = y; row < y+h; row++) {
            for (int column = x; column < x+w; column++) {
                Color& aux = image->at(column, row);
                aux = update;
            }
        }
    }

    void Script::h_mirror() {
        // Mirrors the current image horizontally
        for (int row = 0; row < image->height(); row++) {
            for (int column = 0; column < image->width()/2; column++) {
                Color& left = image->at(column, row);
                Color& right = image->at(image->width()-column-1, row);
                swap(left, right);
            }
        }
    }

    void Script::v_mirror() {
        // Mirrors the current image vertically
        for (int row = 0; row < image->height()/2; row++) {
            for (int column = 0; column < image->width(); column++) {
                Color& up = image->at(column, row);
                Color& down = image->at(column, image->height()-row-1);
                swap(up, down);
            }
        }
    }

    void Script::add() {
        // Pastes a new image into the current image (except the pixels of a given neuttral color)
        Image* copy = new Image(*image); // Creates a copy of the current image with copy constructor

        open();
        Color neutral;
        int x, y;
        input >> neutral >> x >> y;

        for (int i = 0; i < image->height(); i++) {     ///
            for (int j = 0; j < image->width(); j++) {  ///
                Color small = image->at(j, i);

                if (small == neutral) continue;

                Color& big = copy->at(x+j, y+i);
                big = small;
            }
        } 

        delete image;
        image = copy;
    }
    
    void Script::crop() {
        // Replace the current image with a zoom of the current image
        int x, y, w, h;
        input >> x >> y >> w >> h;
        Image* zoom = new Image(w, h);

        for (int row = 0; row < h; row++) {
            for (int column = 0; column < w; column++) {
                zoom->at(column, row) = image->at(column+x, row+y);
            }
        }

        delete image;
        image = zoom;
    }

    void Script::rotate_left() {
        // Rotates image left by 90 degrees
        Image* rotate = new Image(image->height(), image->width());

        for (int row = 0; row < image->height(); row++) {
            for (int column = 0; column < image->width(); column++) {
                rotate->at(row, rotate->height()-1-column) = image->at(column, row);
            }
        }

        delete image;
        image = rotate;
    }

    void Script::rotate_right() {
        // Rotates image right by 90 degrees
        Image* rotate = new Image(image->height(), image->width());

        for (int row = 0; row < image->height(); row++) {
            for (int column = 0; column < image->width(); column++) {
                rotate->at(rotate->width()-1-row, column) = image->at(column, row);
            }
        }

        delete image;
        image = rotate;
    }

    void Script::median_filter() {
        // Apply a median filter to the current image
        int ws;
        input >> ws;

        Image* copy = new Image(*image);
        
        vector<rgb_value> red_values, green_values, blue_values;

        int height = image->height();
        int width = image->width();

        for (int row = 0; row < height; row++) {
            for (int column = 0; column < width; column++) {

                for (int i = max(0, row-(ws/2)); i <= min(height-1, row+(ws/2)); i++) {
                    for (int j = max(0, column-(ws/2)); j <= min(width-1, column+(ws/2)); j++) { // Nested loop to catch all the neighboring pixels
                        red_values.push_back(copy->at(j, i).red());
                        green_values.push_back(copy->at(j, i).green());
                        blue_values.push_back(copy->at(j, i).blue());
                    }
                }

                // Calculate medians:

                sort(red_values.begin(), red_values.end());
                sort(green_values.begin(), green_values.end());
                sort(blue_values.begin(), blue_values.end());
                
                if(red_values.size()%2) {
                    int median_index = red_values.size() / 2;
                    image->at(column, row) = {red_values[median_index], green_values[median_index], blue_values[median_index]};
                }

                else {
                    int median_index = red_values.size() / 2;
                    int median_index2 = median_index-1;
                    rgb_value median_red, median_green, median_blue;
                    median_red = (red_values[median_index] + red_values[median_index2]) / 2;
                    median_green = (green_values[median_index] + green_values[median_index2]) / 2;
                    median_blue = (blue_values[median_index] + blue_values[median_index2]) / 2;
                    image->at(column, row) = {median_red, median_green, median_blue};
                }

                red_values.clear();
                green_values.clear();
                blue_values.clear(); 
            }
            
        }
        
        delete copy;
    }

    void Script::xpm2_open() {
        // Replace current image (if any) with image read from XPM file
        clear_image_if_any();

        string filename;
        input >> filename;
        image = loadFromXPM2(filename);
    }

    void Script::xpm2_save() {
        // Save current image to XPM file
        string filename;
        input >> filename;
        saveToXPM2(filename, image);
    }

}
