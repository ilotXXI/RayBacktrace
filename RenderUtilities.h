#ifndef Unit2H
#define Unit2H

#include <vector>

#include <QColor>

#include "Point.h"

typedef std::vector<std::vector<QColor>>    Canvas;

class Polygon;
struct Line;
class SpotLight;

void Draw(Canvas &display, const Polygon *obj, int np,
          const SpotLight *light, int nl);
void Trace(Line &l, const Polygon *obj, int np, const SpotLight *light,
           int nl, float &R, float &G, float &B, short &looked_lines);
float CrossingParameter(Line l, const Polygon *obj, int np, int &cross_pol_n);
//---------------------------------------------------------------------------
#endif
