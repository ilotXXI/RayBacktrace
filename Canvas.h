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

    void clear(const Rgb &fillColor = Rgb());
    bool isEmpty() const;

    size_t width() const;
    size_t height() const;

private:
    std::vector<std::vector<Rgb>>   _pixels;
    size_t                          _width;
    size_t                          _height;
};


inline Canvas::Canvas(int width, int height, const Rgb &fillColor)
    : _pixels(width, std::vector<Rgb>(height, fillColor))
    , _width(width)
    , _height(height)
{
}

inline const Rgb & Canvas::pixel(int x, int y) const
{
    return _pixels.at(x).at(y);
}

inline void Canvas::setPixel(int x, int y, const Rgb &color)
{
    _ASSERT(0 <= x && x < int(_width) && 0 <= y && y < int(_height));
    _pixels[x][y] = color;
}

inline void Canvas::clear(const Rgb &fillColor)
{
    for (auto &row: _pixels)
        std::fill(row.begin(), row.end(), fillColor);
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
