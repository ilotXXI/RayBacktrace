#ifndef LINE_H
#define LINE_H

struct Line
{
    /*  If directing vector is normed to 1, th line points are expressed as:
        x(t) = x0 + a * t
        y(t) = y0 + b * t
        z(t) = z0 + c * t
    */
    float x0, y0, z0;
    float a, b, c;

    float directingVectorAbs() const;
    void renormDirectingVector(float toAbs = 1.f);
};


inline float Line::directingVectorAbs() const
{
    return sqrt(a * a + b * b + c * c);
}

inline void Line::renormDirectingVector(float toAbs)
{
    const float oldAbs = directingVectorAbs();
    if (oldAbs < 1E-5)
        return;
    const float factor = toAbs / oldAbs;
    a *= factor;
    b *= factor;
    c *= factor;
}

#endif // LINE_H
