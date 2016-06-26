#include "Polygon.h"

#include <math.h>

#include "Point.h"
#include "Line.h"
#include "SpotLight.h"
#include "Rgb.h"

#define EPSILON     0.0005
#define EPSILON2    0.00000005

//Для многоугольника.
Polygon::Polygon(const std::vector<Point> &vertices, const Rgb &ka,
                 const Rgb &kd, float ksCoeff, int cosCoeff)
{
    ChangePolygon(vertices, ka, kd, ksCoeff, cosCoeff);
}

Polygon::Polygon(float x[], float y[], float z[], int n1, float r[],
                 float g[], float b[], float ks_koeff, int cos_koeff)
{
    ChangePolygon(x, y, z, n1, r, g, b, ks_koeff, cos_koeff);
}

void Polygon::ChangePolygon(const std::vector<Point> &vertices, const Rgb &ka,
                            const Rgb &kd, float ksCoeff, int cosCoeff)
{
    top = vertices;
    n = top.size();

    //Запись коэффициентов отражения для грани.
    Rka = ka.red();
    Gka = ka.green();
    Bka = ka.blue();
    Rkd = kd.red();
    Gkd = kd.green();
    Bkd = kd.blue();

    ks = ksCoeff;
    c_p_k = cosCoeff;

    //Вычисление коэффициентов для уравнения несущей плоскости.
    const float x[] = {top[0].x, top[1].x, top[2].x};
    const float y[] = {top[0].y, top[1].y, top[2].y};
    const float z[] = {top[0].z, top[1].z, top[2].z};

    A = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
    B = (z[1] - z[0]) * (x[2] - x[0]) - (x[1] - x[0]) * (z[2] - z[0]);
    C = (x[1] - x[0]) * (y[2] - y[0]) - (y[1] - y[0]) * (x[2] - x[0]);

    //Нормирование вектора нормали.
    D = sqrt(A * A + B * B + C * C); //D пока используется как промежуточная переменная.
    A /= D;
    B /= D;
    C /= D;
    D = -(A * x[0] + B * y[0] + C * z[0]);
}

 //Метод для задания многоугольника.
void Polygon::ChangePolygon(float x[], float y[], float z[], int n1, float r[], float g[], float b[], float ks_koeff, int cos_koeff)
{
    int i;
    //Запись массива вершин.
    top.clear();
    n = n1;
    top.resize(n);
    for (i=0; i<n; ++i)
    {
        top[i].x = x[i];
        top[i].y = y[i];
        top[i].z = z[i];
    }

    //Запись коэффициентов отражения для грани.
    Rka = r[0];
    Rkd = r[1];
    Gka = g[0];
    Gkd = g[1];
    Bka = b[0];
    Bkd = b[1];
    ks = ks_koeff;
    c_p_k = cos_koeff;

    //Вычисление коэффициентов для уравнения несущей плоскости.
    A = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
    B = (z[1] - z[0]) * (x[2] - x[0]) - (x[1] - x[0]) * (z[2] - z[0]);
    C = (x[1] - x[0]) * (y[2] - y[0]) - (y[1] - y[0]) * (x[2] - x[0]);

    //Нормирование вектора нормали.
    D = sqrt(A * A + B * B + C * C); //D пока используется как промежуточная переменная.
    A /= D;
    B /= D;
    C /= D;
    D = -(A * x[0] + B * y[0] + C * z[0]);
}

 //Метод для определения, лежит ли точка в многоугольнике, если она лежит на его несущей плоскости.
char Polygon::PointInPolygon(float x, float y, float z)
{
    int i, m;
    m = 0;
    //Определение, на какую плоскость можно спроецировать многоугольник и точку.
    if(fabs(C) > EPSILON)
    {//Если несущая плоскость многоугольника не перпендикулярна плоскости XY, то многоугольник и точку можно спроецировать на неё.
        switch(LineCross(x, y, top[n-1].y, top[0].x, top[0].y, top[1].x, top[1].y, top[2].y))
        {
        case 1:  //Здесь стоит switch с 2 case'ами, потому что функция LineCross может возвращать также и значение 0, и в этом случае не нужно ничего делать.
          ++m;
          break;
        case -1:
          return 1;
        }

        n -= 2;  //Две последние стороны приходится проверять отдельно из-за индексов.
        for(i=1; i<n; ++i)
        {
            //Проверка, пересекает ли луч очередную сторону.
            switch(LineCross(x, y, top[i-1].y, top[i].x, top[i].y, top[i+1].x, top[i+1].y, top[i+2].y))
            {
            case 1:
                ++m;
                break;
            case -1:
                n += 2;
                return 1;
            }
        }

        //Осталось проверить ещё 2 стороны.
        //Сторона (n-2; n-1) [в силу переобозначения - (n; n+1)].
        switch(LineCross(x, y, top[n-1].y, top[n].x, top[n].y, top[n+1].x, top[n+1].y, top[0].y))
        {
        case 1:
            ++m;
            break;
        case -1:
            n += 2;
            return 1;
        }

        //Сторона (n-1; 0) [в силу переобозначения(после следующего ++n) - (n; 0)].
        ++n;
        switch(LineCross(x, y, top[n-1].y, top[n].x, top[n].y, top[0].x, top[0].y, top[1].y))
        {
        case 1:
          ++m;
          break;
        case -1:
          ++n;
          return 1;
        }

        //Теперь нужно восстановить число углов многоугольника и вернуть результат.
        ++n;
        return m % 2;
    }

    //Продолжение определения плоскости проецирования.
    if(fabs(B) > EPSILON)
    {//Если несущая плоскость многоугольника перпендикулярна плоскости XY, но не перпендикулярна плоскости XZ, то многоугольник и точку можно спроецировать на XZ.
        switch(LineCross(x, z, top[n-1].z, top[0].x, top[0].z, top[1].x, top[1].z, top[2].z))
        {
        case 1:
          ++m;
          break;
        case -1:
          return 1;
        }

        n -= 2;  //Две последние стороны приходится проверять отдельно из-за индексов.
        for(i=1; i<n; ++i)
        //Проверка, пересекает ли луч очередную сторону.
        switch(LineCross(x, z, top[i-1].z, top[i].x, top[i].z, top[i+1].x, top[i+1].z, top[i+2].z))
        {
        case 1:
            ++m;
            break;
        case -1:
            n += 2;
            return 1;
        }

        //Осталось проверить ещё 2 стороны.
        //Сторона (n-2; n-1) [в силу переобозначения - (n; n+1)].
        switch(LineCross(x, z, top[n-1].z, top[n].x, top[n].z, top[n+1].x, top[n+1].z, top[0].z))
        {
        case 1:
            ++m;
            break;
        case -1:
            n += 2;
            return 1;
        }

        //Сторона (n-1; 0) [в силу переобозначения(после следующего ++n) - (n; 0)].
        ++n;
        switch(LineCross(x, z, top[n-1].z, top[n].x, top[n].z, top[0].x, top[0].z, top[1].z))
        {
        case 1:
            ++m;
            break;
        case -1:
            ++n;
            return 1;
        }

        //Теперь нужно восстановить число углов многоугольника и вернуть результат.
        ++n;
        return m % 2;
    }

    //Последняя "стадия" определения плоскости проецирования.
    //Если несущая плоскость многоугольника перпендикулярна и плоскости XY, и плоскости XZ, то многоугольник и точку можно спроецировать на YZ.
    switch(LineCross(y, z, top[n-1].z, top[0].y, top[0].z, top[1].y, top[1].z, top[2].z))
    {
    case 1:
        ++m;
        break;
    case -1:
        return 1;
    }

    n -= 2;  //Две последние стороны приходится проверять отдельно из-за индексов.
    for(i=1; i<n; ++i)
    {
    //Проверка, пересекает ли луч очередную сторону.
        switch(LineCross(y, z, top[i-1].z, top[i].y, top[i].z, top[i+1].y, top[i+1].z, top[i+2].z))
        {
        case 1:
            ++m;
            break;
        case -1:
            n += 2;
            return 1;
        }
    }

    //Осталось проверить ещё 2 стороны.
    //Сторона (n-2; n-1) [в силу переобозначения - (n; n+1)].
    switch(LineCross(y, z, top[n-1].z, top[n].y, top[n].z, top[n+1].y, top[n+1].z, top[0].z))
    {
    case 1:
        ++m;
        break;
    case -1:
        n += 2;
        return 1;
    }

    //Сторона (n-1; 0) [в силу переобозначения(после следующего ++n) - (n; 0)].
    ++n;
    switch(LineCross(y, z, top[n-1].z, top[n].y, top[n].z, top[0].y, top[0].z, top[1].z))
    {
    case 1:
        ++m;
        break;
    case -1:
        ++n;
        return 1;
    }

    //Теперь нужно восстановить число углов многоугольника и вернуть результат.
    ++n;
    return m % 2;
}

//Метод для определения, пересекает ли луч сторону многоугольника.
char Polygon::LineCross(float &x, float &y, float &y0, float &x1, float &y1, float &x2, float &y2, float &y3)
{
    double t, q;
    if (!(x>x1  &&  x>x2))
    {
        if (fabs(y1 - y2) < EPSILON2) //Если сторона горизонтальна.
        {
            if (fabs(y1 - y) < EPSILON2)
            {
                if (x1 < x2)
                {
                    t = x1;
                    q = x2;
                }
                else
                {
                    t = x2;
                    q = x1;
                }
                if (t<=x && x<=q)
                    return -1;
                if ((y1 - y0) * (y2 - y3) > 0)
                    return 1;
            }
        }
        else
        {
            q = (y - y1) / (y2 - y1);
            if (q>EPSILON2  &&  q<=1)   //Если прямая, содержащая луч, пересекает сторону.
            {
                t = x1 - x + (x2 - x1)*q;
                if (t > EPSILON2) //Если луч перескает сторону.
                {
                    if (fabs(q-1)<EPSILON2) //Если луч пересекает вершину.
                    {
                        if ((y2 - y1) * (y2 - y3)  <= 0)
                            return 1;
                        else
                            return 0;
                    }
                    return 1;
                }
                else if(fabs(t) < EPSILON2)  //Если точка лежит на стороне, то она принадлежит многоугольнику.
                {
                    return -1;
                }
            }
        }
    }

    return 0;
}

//Метод для перемещения многоугольника.
void Polygon::Replace(float x1, float y1, float z1)
{
    int i;
    for (i=0; i<n; ++i)
    {
        top[i].x += x1;
        top[i].y += y1;
        top[i].z += z1;
    }

    //Перевычисление коэффициентов для уравнения несущей плоскости.
    A = (top[1].y - top[0].y)*(top[2].z - top[0].z) - (top[2].y - top[0].y)*(top[1].z - top[0].z);
    B = (top[1].z - top[0].z)*(top[2].x - top[0].x) - (top[1].x - top[0].x)*(top[2].z - top[0].z);
    C = (top[1].x - top[0].x)*(top[2].y - top[0].y) - (top[1].y - top[0].y)*(top[2].x - top[0].x);

    //Нормирование вектора нормали.
    D = sqrt(A*A + B*B + C*C); //D пока используется как промежуточная переменная.
    A /= D;
    B /= D;
    C /= D;
    D = -(A*top[0].x + B*top[0].y + C*top[0].z);
}

//Метод для поворота многоугольника.
void Polygon::Rotate(float alpha, short axis)
{
    float old, sinAlpha, cosAlpha;
    int i;
    sinAlpha = sin(alpha);
    cosAlpha = cos(alpha);
    switch (axis)
    {
    case 0: //Поворот вокруг оси Ox.
        for (i=0; i<n; ++i)
        {
          old = top[i].y;
          top[i].y = cosAlpha * old  -  sinAlpha * top[i].z;
          top[i].z = sinAlpha * old  +  cosAlpha * top[i].z;
          }
        break;
    case 1: //Поворот вокруг оси Oy.
        for (i=0; i<n; ++i)
        {
          old = top[i].x;
          top[i].x = cosAlpha * old  +  sinAlpha * top[i].z;
          top[i].z = -sinAlpha * old  +  cosAlpha * top[i].z;
        }
        break;
    default: //Поворот вокруг оси Oz.
        for (i=0; i<n; ++i)
        {
            old = top[i].x;
            top[i].x = cosAlpha * old  -  sinAlpha * top[i].y;
            top[i].y = sinAlpha * old  +  cosAlpha * top[i].y;
        }
    }

    //Перевычисление коэффициентов для уравнения несущей плоскости.
    A = (top[1].y - top[0].y)*(top[2].z - top[0].z) - (top[2].y - top[0].y)*(top[1].z - top[0].z);
    B = (top[1].z - top[0].z)*(top[2].x - top[0].x) - (top[1].x - top[0].x)*(top[2].z - top[0].z);
    C = (top[1].x - top[0].x)*(top[2].y - top[0].y) - (top[1].y - top[0].y)*(top[2].x - top[0].x);

    //Нормирование вектора нормали.
    D = sqrt(A*A + B*B + C*C); //D пока используется как промежуточная переменная.
    A /= D;
    B /= D;
    C /= D;
    D = -(A*top[0].x + B*top[0].y + C*top[0].z);
}

//Метод для масштабирования многоугольника.
void Polygon::Scale(float t)
{
    for (int i=0; i<n; ++i)
    {
        top[i].x *= t;
        top[i].y *= t;
        top[i].z *= t;
    }

    //Перевычисление коэффициентов для уравнения несущей плоскости.
    A = (top[1].y - top[0].y)*(top[2].z - top[0].z) - (top[2].y - top[0].y)*(top[1].z - top[0].z);
    B = (top[1].z - top[0].z)*(top[2].x - top[0].x) - (top[1].x - top[0].x)*(top[2].z - top[0].z);
    C = (top[1].x - top[0].x)*(top[2].y - top[0].y) - (top[1].y - top[0].y)*(top[2].x - top[0].x);

    //Нормирование вектора нормали.
    D = sqrt(A*A + B*B + C*C); //D пока используется как промежуточная переменная.
    A /= D;
    B /= D;
    C /= D;
    D = -(A*top[0].x + B*top[0].y + C*top[0].z);
}

//Метод для вычисления координат направляющего вектора отражённого луча.
void Polygon::GetLine(const Line &l, Line &r)
{
    float q;
    q = l.a * A  +  l.b * B  +  l.c * C;
    q += q;             // q = 2*(L; n).
    r.a = l.a - q*A;
    r.b = l.b - q*B;
    r.c = l.c - q*C;    // R = -L + q*n.
}

void Polygon::Colors(float Rka1, float Gka1, float Bka1, float Rkd1, float Gkd1, float Bkd1, float ks1, float n1)
{
    if(Rka1 >= 0)
        Rka = Rka1;
    if(Gka1 >= 0)
        Gka = Gka1;
    if(Bka1 >= 0)
        Bka = Bka1;
    if(Rkd1 >= 0)
        Rkd = Rkd1;
    if(Gkd1 >= 0)
        Gkd = Gkd1;
    if(Bkd1 >= 0)
        Bkd = Bkd1;
    if(ks1 >= 0)
        ks = ks1;
    if(n1 >= 0)
        c_p_k = int(n1);
}

const Point &Polygon::Vertice(int index) const
{
    return top[index];
}
