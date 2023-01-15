#include "color.hpp"

Color::Color(float red, float green, float blue, float alpha) 
    : red(util::clamp(red, 0.0f, 1.0f)), green(util::clamp(green, 0.0f, 1.0f)), blue(util::clamp(blue, 0.0f, 1.0f)), alpha(util::clamp(alpha, 0.0f, 1.0f))
{}

Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
    : red(red / 255.0f), green(green / 255.0f), blue(blue / 255.0f), alpha(alpha / 255.0f)
{}

float &Color::operator[](size_t i)
{
    assert(i < 4 && "index out of bounds");
    switch(i)
    {
        case(0) : return red; break;
        case(1) : return green; break;
        case(2) : return blue; break;
        case(3) : return alpha; break;
        default : return red;
    }
}

const float &Color::operator[](size_t i) const
{
    assert(i < 4 && "index out of bounds");
    switch(i)
    {
        case(0) : return red; break;
        case(1) : return green; break;
        case(2) : return blue; break;
        case(3) : return alpha; break;
        default : return red;
    }
}

Color mix(const Color &color1, const Color &color2, float t)
{
    float t_clamped = util::clamp(t, 0.0f, 1.0f);
    return color1 * (1 - t_clamped) + color2 * t_clamped;
}

Color operator*(const Color &color, float value)
{
    return Color(color.red * value, color.green * value, color.blue * value);
}

Color operator-(const Color &color, float value)
{
    return Color(color.red - value, color.green - value, color.blue - value);
}

Color operator+(const Color &color1, const Color &color2)
{
    return Color(color1.red + color2.red, color1.green + color2.green, color1.blue + color2.blue, color1.alpha + color2.alpha);
}

Color operator-(const Color &color1, const Color &color2)
{
    return Color(color1.red - color2.red, color1.green - color2.green, color1.blue - color2.blue, color1.alpha - color2.alpha);
}

Color operator*(const Color &color1, const Color &color2)
{
    return Color(color1.red * color2.red, color1.green * color2.green, color1.blue * color2.blue, color1.alpha * color2.alpha);
}

std::ostream &operator<<(std::ostream &os, const Color &color)
{
    os << color.red << " " << color.green << " " << color.blue << " " << color.alpha;
    return os;
}

Color Red()
{
    return Color(1.0f, 0.0f, 0.0f);
}

Color Green()
{
    return Color(0.0f, 1.0f, 0.0f);
}

Color Blue()
{
    return Color(0.0f, 0.0f, 1.0f);
}

Color Yellow()
{
    return Color(1.0f, 1.0f, 0.0f);
}

Color Black()
{
    return Color(0.0f, 0.0f, 0.0f);
}

Color White()
{
    return Color(1.0f, 1.0f, 1.0f);
}