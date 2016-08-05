#ifndef RENDER_UTILITIES_H
#define RENDER_UTILITIES_H

#include <vector>

struct Point;
class Canvas;
class Polygon;
struct Line;
class SpotLight;
class Rgb;

void Draw(Canvas &canvas, const Polygon *obj, int np,
          const SpotLight *light, int nl);
Rgb TraceForPixel(int pixX, int pixY, const Polygon *obj, int np,
                  const SpotLight *light, int nl);
void Trace(Line &l, const Polygon *obj, int np, const SpotLight *light,
           int nl, float &R, float &G, float &B, short looked_lines);
//---------------------------------------------------------------------------
#endif
