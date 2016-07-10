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
    std::vector<Point>  top;           //Массив вершин.
    //Коэффициенты уравнения несущей плоскости.
    float               A;
    float               B;
    float               C;
    float               D;

public:
    Polygon(const std::vector<Point> &vertices, const Rgb &ka, const Rgb &kd,
            float ksCoeff, int cosCoeff);
    Polygon(float x[], float y[], float z[], int n1, float r[], float g[],
            float b[], float ks_koeff, int cos_koeff);

    int c_p_k;  //Степень косинуса для зеркальной составляющей освещённости.
    float Rka, Gka, Bka, Rkd, Gkd, Bkd, ks;     //Коэффициенты поверхности.

    void ChangePolygon(const std::vector<Point> &vertices, const Rgb &ka,
                       const Rgb &kd, float ksCoeff, int cosCoeff);
    void ChangePolygon(float x[], float y[], float z[], int n1, float r[],
                       float g[], float b[], float ks_koeff, int cos_koeff);
    char PointInPolygon(float x, float y, float z) const;
    char LineCross(const float &x, const float &y, const float &y0,
                   const float &x1, const float &y1, const float &x2,
                   const float &y2, const float &y3) const;
    void Rotate(float alpha, short axis);
    void Replace(float x1, float y1, float z1);
    void Scale(float t);
    void GetLine(const Line &l, Line &r) const;
    void Colors(float Rka, float Gka, float Bka, float Rkd, float Gkd,
                float Bkd, float ks, float n);

    Rgb KaColor() const;
    void SetKaColor(const Rgb &value);
    Rgb KdColor() const;
    void SetKdColor(const Rgb &value);

    int VerticesCount() const;
    const Point & Vertice(int index) const;

    //Функции-друзья класса.
    friend float CrossingParameter(Line , Polygon *, int , int &);
//    friend void GetIntensivity(const float &, const float &, const float &,
//                               Polygon *, const int &, SpotLight *,
//                               const int &, float &, float &, float &,
//                               const int &, const Line &l);
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
    return A;
}

inline float Polygon::GetB() const
{
    return B;
}

inline float Polygon::GetC() const
{
    return C;
}

inline float Polygon::GetD() const
{
    return D;
}

inline float Polygon::GetKs() const
{
    return ks;
}

inline int Polygon::CosPower() const
{
    return c_p_k;
}

inline void Polygon::SetCosPower(int value)
{
    c_p_k = value;
}

inline void Polygon::SetKs(float value)
{
    ks = value;
}

inline Rgb Polygon::KaColor() const
{
    return Rgb(Rka, Gka, Bka);
}

inline void Polygon::SetKaColor(const Rgb &value)
{
    Rka = value.red();
    Gka = value.green();
    Bka = value.blue();
}

inline Rgb Polygon::KdColor() const
{
    return Rgb(Rkd, Gkd, Bkd);
}

inline void Polygon::SetKdColor(const Rgb &value)
{
    Rkd = value.red();
    Gkd = value.green();
    Bkd = value.blue();
}

inline int Polygon::VerticesCount() const
{
    return top.size();
}

#endif // POLYGON_H
