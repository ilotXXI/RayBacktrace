#ifndef Unit2H
#define Unit2H

#include <vector>

#include <QColor>

#include "Point.h"

class Canvas;
class Polygon;
struct Line;
class SpotLight;

void Draw(Canvas &canvas, const Polygon *obj, int np,
          const SpotLight *light, int nl);
void Trace(Line &l, const Polygon *obj, int np, const SpotLight *light,
           int nl, float &R, float &G, float &B, short &looked_lines);
//---------------------------------------------------------------------------
#endif
