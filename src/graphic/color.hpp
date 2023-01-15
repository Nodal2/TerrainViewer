#ifndef MESHTOOL_COLOR
#define MESHTOOL_COLOR

#include <iostream>
#include <cassert>
#include <util.hpp>

class Color
{
public:
    Color(float red, float green, float blue, float alpha = 1.0f);
    Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
    float &operator[](size_t i);
    const float &operator[](size_t i) const;

public:
    float red, green, blue, alpha;
};

Color mix(const Color &color1, const Color &color2, float t);

Color operator*(const Color &color, float value);
Color operator-(const Color &color, float value);
Color operator+(const Color &color1, const Color &color2);
Color operator-(const Color &color1, const Color &color2);
Color operator*(const Color &color1, const Color &color2);
std::ostream &operator<<(std::ostream &os, const Color &color);

Color Red();
Color Green();
Color Blue();
Color Yellow();
Color Black();
Color White();

#endif
