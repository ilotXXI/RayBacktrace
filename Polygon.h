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
    enum Axis {AxisOx, AxisOy, AxisOz};

    Polygon(const std::vector<Point> &vertices, const Rgb &_diffusion, const Rgb &_reflection,
            float ksCoeff, int cosCoeff);

    bool pointIsInPolygon(const Point &point) const;
    bool pointIsInPolygon(float x, float y, float z) const;

    void rotate(float alpha, Axis axis);
    void move(float xDelta, float yDelta, float zDelta);
    void scale(float t);

    Rgb diffusionWeights() const;
    void setDiffusionWeights(const Rgb &value);
    Rgb reflectionWeights() const;
    void setReflectionWeights(const Rgb &value);

    int verticesCount() const;
    Point vertice(int index) const;

    /*  Coefficients for containing plain. Vector (A, B, C) is a normal,
        D is offset. The plain's equation is
            A * x + B * y + C * z + D = 0.
    */
    float getA() const;
    float getB() const;
    float getC() const;
    float getD() const;

    float reflectionCoefficient() const;
    void setReflectionCoefficient(float value);
    int cosPower() const;
    void setCosPower(int value);

private:
    enum CrossType {NoCross, RayCross, PointIsOnSide};

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

    void recalcNormal();

    template<typename FuncX, typename FuncY>
    bool pointIsInProjection(const Point &point,
                             FuncX projX, FuncY projY) const;
    static CrossType lineCross(float x, float y, float y0, float x1, float y1,
                               float x2, float y2, float y3);
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

inline float Polygon::reflectionCoefficient() const
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

inline void Polygon::setReflectionCoefficient(float value)
{
    _ks = value;
}

inline bool Polygon::pointIsInPolygon(float x, float y, float z) const
{
    return pointIsInPolygon(Point{x, y, z});
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
    return int(_vertices.size());
}

inline Point Polygon::vertice(int index) const
{
    return _vertices[size_t(index)];
}

#endif // POLYGON_H
