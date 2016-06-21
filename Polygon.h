#ifndef POLYGON_H
#define POLYGON_H

#include <vector>

struct point;
struct line;
class spotLight;

class polygon
{
private:
    std::vector<point>  top;           //Массив вершин.
    float               A, B, C, D;    //Коэффициенты уравнения несущей плоскости.
    int                 n;             //Кол-во вершин.

public:
    polygon(float x[], float y[], float z[], int n1, float r[], float g[],
            float b[], float ks_koeff, int cos_koeff);

    int c_p_k;  //Степень косинуса для зеркальной составляющей освещённости.
    float Rka, Gka, Bka, Rkd, Gkd, Bkd, ks;     //Коэффициенты поверхности.
    void ChangePolygon(float x[], float y[], float z[], int n1, float r[], float g[], float b[], float ks_koeff, int cos_koeff);
    char PointInPolygon(float x, float y, float z);
    char LineCross(float &x, float &y, float &y0, float &x1, float &y1, float &x2, float &y2, float &y3);
    void operator=(polygon &Polygon2);
    void Rotate(float alpha, short axis);
    void Replace(float x1, float y1, float z1);
    void Scale(float t);
    void GetLine(const line &l, line &r);
    void Colors(float Rka, float Gka, float Bka, float Rkd, float Gkd, float Bkd, float ks, float n);

    int VerticesCount() const;
    const point & Vertice(int index) const;

    //Функции-друзья класса.
    friend float CrossingParameter(line , polygon *, int , int &);
    friend void GetIntensivity(const float &, const float &, const float &, polygon *, const int &, spotLight *, const int &, float &, float &, float &, const int &, const line &l);
};

inline int polygon::VerticesCount() const
{
    return n;
}

#endif // POLYGON_H
