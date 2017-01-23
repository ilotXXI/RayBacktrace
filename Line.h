#ifndef LINE_H
#define LINE_H

struct Line
{
    float x0, y0, z0;
    float a, b, c;

    float directingVectorAbs() const;
};


inline float Line::directingVectorAbs() const
{
    return sqrt(a * a + b * b + c * c);
}

#endif // LINE_H
