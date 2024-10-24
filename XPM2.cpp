#include "XPM2.hpp"
#include <fstream>
#include <map>

#include <iostream>
using namespace std;

namespace prog {

    // Auxiliary functions to do conversions from hexadecimal to Color and vice versa
        int hexa_to_int(const string& hexa) {
            int mul = 1;
            int sum = 0;

            for (int i = hexa.length()-1; i >= 0; i--)
            {
                if ((hexa[i]>='0') && (hexa[i]<='9')) sum += (hexa[i]-'0') * mul;
                else if ((hexa[i]>='a') && (hexa[i]<='f')) sum += (10+hexa[i]-'a') * mul;
                else sum += (10+hexa[i]-'A') * mul;
                mul <<= 4;
            }
            
            return sum;
        }

        Color hexa_to_Color(const string& hexa) {
            string red_hexa, green_hexa, blue_hexa;

            red_hexa = hexa.substr(0, 2);
            green_hexa = hexa.substr(2, 2);
            blue_hexa = hexa.substr(4, 2);

            return {(rgb_value)hexa_to_int(red_hexa), (rgb_value)hexa_to_int(green_hexa), (rgb_value)hexa_to_int(blue_hexa)};
        }

        string int_to_hexa(int n) {
            string hexa;
            if (n==0) return "00";

            while(n > 0) {
                int r = n % 16;
                if (r < 10) hexa = (char)('0' + r) + hexa;
                else hexa = (char)('a' + r - 10) + hexa;

                n >>=4;
            }

            if (hexa.length() == 1) return '0' + hexa;
            else return hexa;
        }

        string Color_to_hexa(const Color& color) {
            return int_to_hexa(color.red()) + int_to_hexa(color.green()) + int_to_hexa(color.blue());
        }
    // End of auxiliary functions

    Image* loadFromXPM2(const std::string& file) {
        // Returns a pointer to an image read from XPM file
        ifstream in(file);
        string str_trash;
        in >> str_trash >> str_trash;
        int w, h, n, c;
        in >> w >> h >> n >> c;

        map<char, Color> colors;

        for (int i = 0; i < n; i++) { //loop to find all the colors in the image
            char key, chr_trash;
            string hexa;
            in >> key >> chr_trash >> chr_trash >> hexa;
            colors.insert(pair<char, Color>(key, hexa_to_Color(hexa)));
        }
        
        Image* image = new Image(w, h);

        for (int row = 0; row < h; row++) { //loop to give the right colors to the image
            for (int column = 0; column < w; column++)
            {
                char symbol;
                in >> symbol;
                image->at(column, row) = colors[symbol];
            }
        }
    
        return image;
    }

    void saveToXPM2(const std::string& file, const Image* image) {
        // Save an image to XPM file
        map<Color, char> colors;

        for (int row = 0; row < image->height(); row++) {
            for (int column = 0; column < image->width(); column++) {
                auto it = colors.find(image->at(column, row));
                if (it == colors.end()) colors[image->at(column, row)] = 'a' + colors.size();
            }
        }

        ofstream out(file);
        out << "! XPM2\n" << image->width() << ' ' << image->height() << ' ' << colors.size() << " 1\n";

        for (auto pair : colors) {
            out << pair.second << " c #" << Color_to_hexa(pair.first) << '\n';
        }

        for (int row = 0; row < image->height(); row++) {
            for (int column = 0; column < image->width(); column++) {
                out << colors[image->at(column, row)];
            }
            out << '\n';
        }
    }
}
