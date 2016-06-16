#ifndef Unit2H
#define Unit2H

#include <vector>

#include <QColor>

#include "Point.h"

#define VISOR_Z               1200
#define DISPLAY_Z             500
#define Ia                    150
#define K_KOEFFICIENT         0.01
#define B_KOEFFICIENT         0.001
#define MAX_LOOKED_LINES_CNT  20

typedef std::vector<std::vector<QColor>>    Canvas;

class polygon;
struct line;
class spotLight;

void Draw(Canvas &display, polygon *obj, int np, spotLight *light, int nl);
void Trace(line &l, polygon *obj, int np, spotLight *light, int nl, float &R, float &G, float &B, short &looked_lines);
float CrossingParameter(line l, polygon *obj, int np, int &cross_pol_n);
//---------------------------------------------------------------------------
#endif
