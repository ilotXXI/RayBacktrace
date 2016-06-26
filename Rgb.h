#ifndef RGB_H
#define RGB_H

class Rgb
{
public:
    Rgb(float r, float g, float b);

    float red() const;
    void setRed(float value);
    float green() const;
    void setGreen(float value);
    float blue() const;
    void setBlue(float value);

private:
    float _red;
    float _green;
    float _blue;
};

inline Rgb::Rgb(float r, float g, float b)
    : _red(r)
    , _green(g)
    , _blue(b)
{
}

inline float Rgb::red() const
{
    return _red;
}

inline void Rgb::setRed(float value)
{
    _red = value;
}

inline float Rgb::green() const
{
    return _green;
}

inline void Rgb::setGreen(float value)
{
    _green = value;
}

inline float Rgb::blue() const
{
    return _blue;
}

inline void Rgb::setBlue(float value)
{
    _blue = value;
}

#endif // RGB_H
