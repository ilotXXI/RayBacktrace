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
    : _vertices(vertices)
    , _cosPow(cosCoeff)
    , _diffusion(ka)
    , _reflection(kd)
    , _ks(ksCoeff)
{
    //Вычисление коэффициентов для уравнения несущей плоскости.
    const float x[] = {_vertices[0].x, _vertices[1].x, _vertices[2].x};
    const float y[] = {_vertices[0].y, _vertices[1].y, _vertices[2].y};
    const float z[] = {_vertices[0].z, _vertices[1].z, _vertices[2].z};

    _A = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
    _B = (z[1] - z[0]) * (x[2] - x[0]) - (x[1] - x[0]) * (z[2] - z[0]);
    _C = (x[1] - x[0]) * (y[2] - y[0]) - (y[1] - y[0]) * (x[2] - x[0]);

    //Нормирование вектора нормали.
    _D = sqrt(_A * _A + _B * _B + _C * _C); //D пока используется как промежуточная переменная.
    _A /= _D;
    _B /= _D;
    _C /= _D;
    _D = -(_A * x[0] + _B * y[0] + _C * z[0]);
}

 //Метод для определения, лежит ли точка в многоугольнике, если она лежит на его несущей плоскости.
char Polygon::pointInPolygon(float x, float y, float z) const
{
    int i, m;
    m = 0;
    //Определение, на какую плоскость можно спроецировать многоугольник и точку.
    if(fabs(_C) > EPSILON)
    {//Если несущая плоскость многоугольника не перпендикулярна плоскости XY, то многоугольник и точку можно спроецировать на неё.
        switch (lineCross(x, y, _vertices.back().y, _vertices.front().x, _vertices.front().y,
                _vertices[1].x, _vertices[1].y, _vertices[2].y))
        {
        case 1:  //Здесь стоит switch с 2 case'ами, потому что функция LineCross может возвращать также и значение 0, и в этом случае не нужно ничего делать.
          ++m;
          break;
        case -1:
          return 1;
        }

        int n = _vertices.size();
        n -= 2;  //Две последние стороны приходится проверять отдельно из-за индексов.
        for(i=1; i<n; ++i)
        {
            //Проверка, пересекает ли луч очередную сторону.
            switch(lineCross(x, y, _vertices[i-1].y, _vertices[i].x, _vertices[i].y, _vertices[i+1].x,
                    _vertices[i+1].y, _vertices[i+2].y))
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
        switch(lineCross(x, y, _vertices[n-1].y, _vertices[n].x, _vertices[n].y, _vertices[n+1].x,
                _vertices[n+1].y, _vertices[0].y))
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
        switch(lineCross(x, y, _vertices[n-1].y, _vertices[n].x, _vertices[n].y, _vertices[0].x,
                _vertices[0].y, _vertices[1].y))
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
    if(fabs(_B) > EPSILON)
    {//Если несущая плоскость многоугольника перпендикулярна плоскости XY, но не перпендикулярна плоскости XZ, то многоугольник и точку можно спроецировать на XZ.
        switch(lineCross(x, z, _vertices.back().z, _vertices[0].x, _vertices[0].z, _vertices[1].x,
                _vertices[1].z, _vertices[2].z))
        {
        case 1:
          ++m;
          break;
        case -1:
          return 1;
        }

        int n = _vertices.size() - 2;  //Две последние стороны приходится проверять отдельно из-за индексов.
        for(i=1; i<n; ++i)
        //Проверка, пересекает ли луч очередную сторону.
        switch(lineCross(x, z, _vertices[i-1].z, _vertices[i].x, _vertices[i].z, _vertices[i+1].x,
                _vertices[i+1].z, _vertices[i+2].z))
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
        switch(lineCross(x, z, _vertices[n-1].z, _vertices[n].x, _vertices[n].z, _vertices[n+1].x, _vertices[n+1].z, _vertices[0].z))
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
        switch(lineCross(x, z, _vertices[n-1].z, _vertices[n].x, _vertices[n].z, _vertices[0].x, _vertices[0].z, _vertices[1].z))
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
    switch(lineCross(y, z, _vertices.back().z, _vertices[0].y, _vertices[0].z, _vertices[1].y,
            _vertices[1].z, _vertices[2].z))
    {
    case 1:
        ++m;
        break;
    case -1:
        return 1;
    }

    int n = _vertices.size() - 2;  //Две последние стороны приходится проверять отдельно из-за индексов.
    for(i=1; i<n; ++i)
    {
    //Проверка, пересекает ли луч очередную сторону.
        switch(lineCross(y, z, _vertices[i-1].z, _vertices[i].y, _vertices[i].z, _vertices[i+1].y, _vertices[i+1].z, _vertices[i+2].z))
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
    switch(lineCross(y, z, _vertices[n-1].z, _vertices[n].y, _vertices[n].z, _vertices[n+1].y, _vertices[n+1].z, _vertices[0].z))
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
    switch(lineCross(y, z, _vertices[n-1].z, _vertices[n].y, _vertices[n].z, _vertices[0].y, _vertices[0].z, _vertices[1].z))
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
char Polygon::lineCross(const float &x, const float &y, const float &y0,
                        const float &x1, const float &y1, const float &x2,
                        const float &y2, const float &y3) const
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
void Polygon::replace(float x1, float y1, float z1)
{
    for (auto &vert: _vertices)
    {
        vert.x += x1;
        vert.y += y1;
        vert.z += z1;
    }

    //Перевычисление коэффициентов для уравнения несущей плоскости.
    _A = (_vertices[1].y - _vertices[0].y)*(_vertices[2].z - _vertices[0].z) - (_vertices[2].y - _vertices[0].y)*(_vertices[1].z - _vertices[0].z);
    _B = (_vertices[1].z - _vertices[0].z)*(_vertices[2].x - _vertices[0].x) - (_vertices[1].x - _vertices[0].x)*(_vertices[2].z - _vertices[0].z);
    _C = (_vertices[1].x - _vertices[0].x)*(_vertices[2].y - _vertices[0].y) - (_vertices[1].y - _vertices[0].y)*(_vertices[2].x - _vertices[0].x);

    //Нормирование вектора нормали.
    _D = sqrt(_A*_A + _B*_B + _C*_C); //D пока используется как промежуточная переменная.
    _A /= _D;
    _B /= _D;
    _C /= _D;
    _D = -(_A*_vertices[0].x + _B*_vertices[0].y + _C*_vertices[0].z);
}

//Метод для поворота многоугольника.
void Polygon::rotate(float alpha, short axis)
{
    float old, sinAlpha, cosAlpha;
    sinAlpha = sin(alpha);
    cosAlpha = cos(alpha);
    switch (axis)
    {
    case 0: //Поворот вокруг оси Ox.
        for (auto &vert:    _vertices)
        {
            old = vert.y;
            vert.y = cosAlpha * old  -  sinAlpha * vert.z;
            vert.z = sinAlpha * old  +  cosAlpha * vert.z;
        }
        break;
    case 1: //Поворот вокруг оси Oy.
        for (auto &vert:    _vertices)
        {
          old = vert.x;
          vert.x = cosAlpha * old  +  sinAlpha * vert.z;
          vert.z = -sinAlpha * old  +  cosAlpha * vert.z;
        }
        break;
    default: //Поворот вокруг оси Oz.
        for (auto &vert:    _vertices)
        {
            old = vert.x;
            vert.x = cosAlpha * old  -  sinAlpha * vert.y;
            vert.y = sinAlpha * old  +  cosAlpha * vert.y;
        }
    }

    //Перевычисление коэффициентов для уравнения несущей плоскости.
    _A = (_vertices[1].y - _vertices[0].y)*(_vertices[2].z - _vertices[0].z) - (_vertices[2].y - _vertices[0].y)*(_vertices[1].z - _vertices[0].z);
    _B = (_vertices[1].z - _vertices[0].z)*(_vertices[2].x - _vertices[0].x) - (_vertices[1].x - _vertices[0].x)*(_vertices[2].z - _vertices[0].z);
    _C = (_vertices[1].x - _vertices[0].x)*(_vertices[2].y - _vertices[0].y) - (_vertices[1].y - _vertices[0].y)*(_vertices[2].x - _vertices[0].x);

    //Нормирование вектора нормали.
    _D = sqrt(_A*_A + _B*_B + _C*_C); //D пока используется как промежуточная переменная.
    _A /= _D;
    _B /= _D;
    _C /= _D;
    _D = -(_A*_vertices[0].x + _B*_vertices[0].y + _C*_vertices[0].z);
}

//Метод для масштабирования многоугольника.
void Polygon::scale(float t)
{
    for (auto &vert:    _vertices)
    {
        vert.x *= t;
        vert.y *= t;
        vert.z *= t;
    }

    //Перевычисление коэффициентов для уравнения несущей плоскости.
    _A = (_vertices[1].y - _vertices[0].y)*(_vertices[2].z - _vertices[0].z) - (_vertices[2].y - _vertices[0].y)*(_vertices[1].z - _vertices[0].z);
    _B = (_vertices[1].z - _vertices[0].z)*(_vertices[2].x - _vertices[0].x) - (_vertices[1].x - _vertices[0].x)*(_vertices[2].z - _vertices[0].z);
    _C = (_vertices[1].x - _vertices[0].x)*(_vertices[2].y - _vertices[0].y) - (_vertices[1].y - _vertices[0].y)*(_vertices[2].x - _vertices[0].x);

    //Нормирование вектора нормали.
    _D = sqrt(_A*_A + _B*_B + _C*_C); //D пока используется как промежуточная переменная.
    _A /= _D;
    _B /= _D;
    _C /= _D;
    _D = -(_A*_vertices[0].x + _B*_vertices[0].y + _C*_vertices[0].z);
}
