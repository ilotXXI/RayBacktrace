#ifndef POLYGON_H
#define POLYGON_H

#include <vector>

#include "Point.h"

struct Line;
class SpotLight;
class Rgb;

class Polygon
{
private:
    std::vector<Point>  top;           //Массив вершин.
    float               A, B, C, D;    //Коэффициенты уравнения несущей плоскости.

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
    char PointInPolygon(float x, float y, float z);
    char LineCross(float &x, float &y, float &y0, float &x1, float &y1,
                   float &x2, float &y2, float &y3);
    void Rotate(float alpha, short axis);
    void Replace(float x1, float y1, float z1);
    void Scale(float t);
    void GetLine(const Line &l, Line &r);
    void Colors(float Rka, float Gka, float Bka, float Rkd, float Gkd,
                float Bkd, float ks, float n);

    int VerticesCount() const;
    const Point & Vertice(int index) const;

    //Функции-друзья класса.
    friend float CrossingParameter(Line , Polygon *, int , int &);
    friend void GetIntensivity(const float &, const float &, const float &,
                               Polygon *, const int &, SpotLight *,
                               const int &, float &, float &, float &,
                               const int &, const Line &l);
};

inline int Polygon::VerticesCount() const
{
    return top.size();
}

#endif // POLYGON_H
