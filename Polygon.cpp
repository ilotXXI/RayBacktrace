#include "Polygon.h"

#include <math.h>
#include <assert.h>
#include <algorithm>

#include "Point.h"
#include "Line.h"
#include "SpotLight.h"
#include "Rgb.h"

#define EPSILON     0.0005
#define EPSILON2    0.00000005

//#define FUNCTIONS_AS_PROJECTORS

#ifdef FUNCTIONS_AS_PROJECTORS
static inline float getX(const Point &point)
{
    return point.x;
}

static inline float getY(const Point &point)
{
    return point.y;
}

static inline float getZ(const Point &point)
{
    return point.z;
}

#else

static const auto getX = [](const Point &point) -> float { return point.x; };
static const auto getY = [](const Point &point) -> float { return point.y; };
static const auto getZ = [](const Point &point) -> float { return point.z; };

#endif


Polygon::Polygon(const std::vector<Point> &vertices, const Rgb &ka,
                 const Rgb &kd, float ksCoeff, int cosCoeff)
    : _vertices(vertices)
    , _cosPow(cosCoeff)
    , _diffusion(ka)
    , _reflection(kd)
    , _ks(ksCoeff)
{
    const size_t minAllowedVerticesCount = 3;
    if (_vertices.size() < minAllowedVerticesCount) {
        // Polygon cannot have less than 3 vertices.
        assert(false);
        _vertices.resize(minAllowedVerticesCount);
    }

    recalcNormal();
}

bool Polygon::pointIsInPolygon(const Point &point) const
{
    if(fabs(_C) > EPSILON) {
        // Polygon is not ortogonal to XY plain.
        return pointIsInProjection(point, getX, getY);
    } else  if(fabs(_B) > EPSILON) {
        // Polygon is not ortogonal to XZ plain.
        return pointIsInProjection(point, getX, getZ);
    } else {
        // Polygon is not ortogonal to YZ plain.
        return pointIsInProjection(point, getY, getZ);
    }
}

void Polygon::move(float xDelta, float yDelta, float zDelta)
{
    for (auto &vert: _vertices)
    {
        vert.x += xDelta;
        vert.y += yDelta;
        vert.z += zDelta;
    }

    recalcNormal();
}

void Polygon::rotate(float alpha, Axis axis)
{
    float old, sinAlpha, cosAlpha;
    sinAlpha = sin(alpha);
    cosAlpha = cos(alpha);
    switch (axis)
    {
    case AxisOx:
        for (auto &vert:    _vertices)
        {
            old = vert.y;
            vert.y = cosAlpha * old  -  sinAlpha * vert.z;
            vert.z = sinAlpha * old  +  cosAlpha * vert.z;
        }
        break;
    case AxisOy:
        for (auto &vert:    _vertices)
        {
          old = vert.x;
          vert.x = cosAlpha * old  +  sinAlpha * vert.z;
          vert.z = -sinAlpha * old  +  cosAlpha * vert.z;
        }
        break;
    case AxisOz:
        for (auto &vert:    _vertices)
        {
            old = vert.x;
            vert.x = cosAlpha * old  -  sinAlpha * vert.y;
            vert.y = sinAlpha * old  +  cosAlpha * vert.y;
        }
        break;
    default:
        assert(false);
        return;
    }

    recalcNormal();
}

void Polygon::scale(float t)
{
    for (auto &vert:    _vertices)
    {
        vert.x *= t;
        vert.y *= t;
        vert.z *= t;
    }

    recalcNormal();
}

void Polygon::recalcNormal()
{
    _A = (_vertices[1].y - _vertices[0].y) * (_vertices[2].z - _vertices[0].z) -
        (_vertices[2].y - _vertices[0].y) * (_vertices[1].z - _vertices[0].z);
    _B = (_vertices[1].z - _vertices[0].z) * (_vertices[2].x - _vertices[0].x) -
        (_vertices[1].x - _vertices[0].x) * (_vertices[2].z - _vertices[0].z);
    _C = (_vertices[1].x - _vertices[0].x) * (_vertices[2].y - _vertices[0].y) -
        (_vertices[1].y - _vertices[0].y) * (_vertices[2].x - _vertices[0].x);

    const float normalAbs = sqrt(_A * _A + _B * _B + _C * _C);
    _A /= normalAbs;
    _B /= normalAbs;
    _C /= normalAbs;
    _D = - (_A * _vertices[0].x + _B * _vertices[0].y + _C * _vertices[0].z);
}

/*  FuncX and FuncY are functions for points projection to the plain. They
    must have a signature:
        float func(const Point &);
*/
template<typename FuncX, typename FuncY>
bool Polygon::pointIsInProjection(const Point &point,
                                  FuncX projX, FuncY projY) const
{
    // TODO: circular indices or look at triangle as to an especial case.
    const float x = projX(point);
    const float y = projY(point);

    int m = 0;

    switch (lineCross(x, y, projY(_vertices.back()), projX(_vertices.front()),
            projY(_vertices.front()), projX(_vertices[1]),
            projY(_vertices[1]), projY(_vertices[2])))
    {
    case RayCross:
        ++m;
        break;
    case PointIsOnSide:
        return true;
    }

    int n = _vertices.size();
    n -= 2;  //Две последние стороны приходится проверять отдельно из-за индексов.
    for(int i = 1; i < n; ++i)
    {
        //Проверка, пересекает ли луч очередную сторону.
        switch (lineCross(x, y, projY(_vertices[i-1]), projX(_vertices[i]),
                projY(_vertices[i]), projX(_vertices[i+1]),
                projY(_vertices[i+1]), projY(_vertices[i+2])))
        {
        case RayCross:
            ++m;
            break;
        case PointIsOnSide:
            return true;
        }
    }

    //Осталось проверить ещё 2 стороны.
    //Сторона (n-2; n-1) [в силу переобозначения - (n; n+1)].
    switch (lineCross(x, y, projY(_vertices[n-1]), projX(_vertices[n]),
            projY(_vertices[n]), projX(_vertices[n+1]),
            projY(_vertices[n+1]), projY(_vertices[0])))
    {
    case RayCross:
        ++m;
        break;
    case PointIsOnSide:
        return true;
    }

    //Сторона (n-1; 0) [в силу переобозначения(после следующего ++n) - (n; 0)].
    ++n;
    switch (lineCross(x, y, projY(_vertices[n-1]), projX(_vertices[n]),
            projY(_vertices[n]), projX(_vertices[0]),
            projY(_vertices[0]), projY(_vertices[1])))
    {
    case RayCross:
        ++m;
        break;
    case PointIsOnSide:
        return true;
    }

    return (m % 2) ? true : false;
}

/*  Determines in a what way does a horizontal ray from (x, y) point intersects
    a side projection on a 2D plain. The side is a 2D line between (x1, y1) and
    (x2, y2) points. y0 and y3 are y-coordinates of previous and next vertices
    correspondingly. The 2D plain may be any projection of 3D space,
    terminology of "x" and "y" is applied just to the plain.
*/
Polygon::CrossType Polygon::lineCross(float x, float y, float y0,
                                      float x1, float y1,
                                      float x2, float y2, float y3)
{
    if (x > x1 && x > x2) {
        // The ray is righter than the side.
        return NoCross;
    }

    if (fabs(y1 - y2) < EPSILON2) {
        // Especial case: the side is horizontal.

        if (fabs(y1 - y) < EPSILON2) {
            const float leftX = std::min(x1, x2);
            const float rightX = std::max(x1, x2);

            if (leftX <= x && x <= rightX)
                return PointIsOnSide;
            if ((y1 - y0) * (y2 - y3) > 0)
                return RayCross;
        }
    } else {
        const float q = (y - y1) / (y2 - y1);
        if (q > EPSILON2 && q <= 1) {
            // Ray's line intersects the side.
            const float t = x1 - x + (x2 - x1) * q;
            if (t > EPSILON2) {
                // The ray intersects the line.
                if (fabs(q - 1) < EPSILON2) {
                    // The ray intersects a vertex.
                    if ((y2 - y1) * (y2 - y3) <= 0) {
                        return RayCross;
                    } else {
                        return NoCross;
                    }
                }

                return RayCross;
            } else if(t > - EPSILON2) {
                // The point is on the side.
                return PointIsOnSide;
            }
        }
    }

    return NoCross;
}
