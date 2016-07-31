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

    char PointInPolygon(float x, float y, float z) const;
    char LineCross(const float &x, const float &y, const float &y0,
                   const float &x1, const float &y1, const float &x2,
                   const float &y2, const float &y3) const;
    void Rotate(float alpha, short axis);
    void Replace(float x1, float y1, float z1);
    void Scale(float t);
    void GetLine(const Line &l, Line &r) const;

    const Rgb &KaColor() const;
    void SetKaColor(const Rgb &value);
    const Rgb &KdColor() const;
    void SetKdColor(const Rgb &value);

    int VerticesCount() const;
    const Point & Vertice(int index) const;

    float GetA() const;
    float GetB() const;
    float GetC() const;
    float GetD() const;

    float GetKs() const;
    void SetKs(float value);
    int CosPower() const;
    void SetCosPower(int value);
};


inline float Polygon::GetA() const
{
    return _A;
}

inline float Polygon::GetB() const
{
    return _B;
}

inline float Polygon::GetC() const
{
    return _C;
}

inline float Polygon::GetD() const
{
    return _D;
}

inline float Polygon::GetKs() const
{
    return _ks;
}

inline int Polygon::CosPower() const
{
    return _cosPow;
}

inline void Polygon::SetCosPower(int value)
{
    _cosPow = value;
}

inline void Polygon::SetKs(float value)
{
    _ks = value;
}

inline const Rgb & Polygon::KaColor() const
{
    return _ka;
}

inline void Polygon::SetKaColor(const Rgb &value)
{
    _ka = value;
}

inline const Rgb & Polygon::KdColor() const
{
    return _kd;
}

inline void Polygon::SetKdColor(const Rgb &value)
{
    _kd = value;
}

inline int Polygon::VerticesCount() const
{
    return _vertices.size();
}

inline const Point &Polygon::Vertice(int index) const
{
    return _vertices[index];
}

#endif // POLYGON_H
