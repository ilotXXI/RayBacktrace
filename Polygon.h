#ifndef POLYGON_H
#define POLYGON_H

#include <vector>

#include "Point.h"
#include "Rgb.h"

struct Line;
class SpotLight;

class Polygon
{
private:
    std::vector<Point>  _vertices;

    //Степень косинуса для зеркальной составляющей освещённости.
    int                 _cosPow;

    // Коэффициенты рассеивания.
    Rgb                 _ka;
    // Коэффициенты отражения.
    Rgb                 _kd;

    float               _ks;

    //Коэффициенты уравнения несущей плоскости.
    float               _A;
    float               _B;
    float               _C;
    float               _D;

public:
    Polygon(const std::vector<Point> &vertices, const Rgb &_ka, const Rgb &_kd,
            float ksCoeff, int cosCoeff);

    char pointInPolygon(float x, float y, float z) const;
    char lineCross(const float &x, const float &y, const float &y0,
                   const float &x1, const float &y1, const float &x2,
                   const float &y2, const float &y3) const;
    void rotate(float alpha, short axis);
    void replace(float x1, float y1, float z1);
    void scale(float t);
    void getLine(const Line &l, Line &r) const;

    const Rgb &kaColor() const;
    void setKaColor(const Rgb &value);
    const Rgb &kdColor() const;
    void setKdColor(const Rgb &value);

    int verticesCount() const;
    const Point & vertice(int index) const;

    float getA() const;
    float getB() const;
    float getC() const;
    float getD() const;

    float getKs() const;
    void setKs(float value);
    int cosPower() const;
    void setCosPower(int value);
};


inline float Polygon::getA() const
{
    return _A;
}

inline float Polygon::getB() const
{
    return _B;
}

inline float Polygon::getC() const
{
    return _C;
}

inline float Polygon::getD() const
{
    return _D;
}

inline float Polygon::getKs() const
{
    return _ks;
}

inline int Polygon::cosPower() const
{
    return _cosPow;
}

inline void Polygon::setCosPower(int value)
{
    _cosPow = value;
}

inline void Polygon::setKs(float value)
{
    _ks = value;
}

inline const Rgb & Polygon::kaColor() const
{
    return _ka;
}

inline void Polygon::setKaColor(const Rgb &value)
{
    _ka = value;
}

inline const Rgb & Polygon::kdColor() const
{
    return _kd;
}

inline void Polygon::setKdColor(const Rgb &value)
{
    _kd = value;
}

inline int Polygon::verticesCount() const
{
    return _vertices.size();
}

inline const Point &Polygon::vertice(int index) const
{
    return _vertices[index];
}

#endif // POLYGON_H
