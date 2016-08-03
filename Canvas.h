#ifndef CANVAS_H
#define CANVAS_H

#include <vector>

#include "Rgb.h"

// Provides a canvas (a 2D array of pixels).
class Canvas
{
public:
    Canvas(int width, int height, const Rgb &fillColor = Rgb());

    const Rgb & pixel(int x, int y) const;
    void setPixel(int x, int y, const Rgb &color);

    void clear();
    bool isEmpty() const;

    size_t width() const;
    size_t height() const;

private:
    std::vector<Rgb>    _pixels;
    size_t              _width;
    size_t              _height;
};


inline Canvas::Canvas(int width, int height, const Rgb &fillColor)
    : _pixels(width * height, fillColor)
    , _width(width)
    , _height(height)
{
}

inline const Rgb & Canvas::pixel(int x, int y) const
{
    return _pixels.at(x * _height + y);
}

inline void Canvas::setPixel(int x, int y, const Rgb &color)
{
    Q_ASSERT(0 <= x && x < int(_width) && 0 <= y && y < int(_height));
    _pixels[x * _height + y] = color;
}

inline void Canvas::clear()
{
    std::fill(_pixels.begin(), _pixels.end(), Rgb());
}

inline bool Canvas::isEmpty() const
{
    return _pixels.empty();
}

inline size_t Canvas::width() const
{
    return _width;
}

inline size_t Canvas::height() const
{
    return _height;
}

#endif // CANVAS_H
