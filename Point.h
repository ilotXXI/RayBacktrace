#ifndef POINT_H
#define POINT_H

struct Point
{
    float x, y, z;

    Point();
    Point(float xX, float yY, float zZ);
};

inline Point::Point()
    : x(0.f)
    , y(0.f)
    , z(0.f)
{
}

inline Point::Point(float xX, float yY, float zZ)
    : x(xX)
    , y(yY)
    , z(zZ)
{
}

#endif // POINT_H
