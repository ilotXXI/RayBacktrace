#ifndef POLYGON_H
#define POLYGON_H

#include <vector>

#include "Point.h"
#include "Rgb.h"

struct Line;
class SpotLight;

class Polygon
{
public:
    Polygon(const std::vector<Point> &vertices, const Rgb &_diffusion, const Rgb &_reflection,
            float ksCoeff, int cosCoeff);

    bool pointIsInPolygon(const Point &point) const;
    bool pointIsInPolygon(float x, float y, float z) const;
    char lineCross(const float &x, const float &y, const float &y0,
                   const float &x1, const float &y1, const float &x2,
                   const float &y2, const float &y3) const;
    void rotate(float alpha, short axis);
    void replace(float x1, float y1, float z1);
    void scale(float t);

    Rgb diffusionWeights() const;
    void setDiffusionWeights(const Rgb &value);
    Rgb reflectionWeights() const;
    void setReflectionWeights(const Rgb &value);

    int verticesCount() const;
    Point vertice(int index) const;

    float getA() const;
    float getB() const;
    float getC() const;
    float getD() const;

    float getKs() const;
    void setKs(float value);
    int cosPower() const;
    void setCosPower(int value);

private:
    std::vector<Point>  _vertices;

    //Степень косинуса для зеркальной составляющей освещённости.
    int                 _cosPow;

    Rgb                 _diffusion;
    Rgb                 _reflection;

    float               _ks;

    //Коэффициенты уравнения несущей плоскости.
    float               _A;
    float               _B;
    float               _C;
    float               _D;
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

inline bool Polygon::pointIsInPolygon(const Point &point) const
{
    return pointIsInPolygon(point.x, point.y, point.z);
}

inline Rgb Polygon::diffusionWeights() const
{
    return _diffusion;
}

inline void Polygon::setDiffusionWeights(const Rgb &value)
{
    _diffusion = value;
}

inline Rgb Polygon::reflectionWeights() const
{
    return _reflection;
}

inline void Polygon::setReflectionWeights(const Rgb &value)
{
    _reflection = value;
}

inline int Polygon::verticesCount() const
{
    return _vertices.size();
}

inline Point Polygon::vertice(int index) const
{
    return _vertices[index];
}

#endif // POLYGON_H
