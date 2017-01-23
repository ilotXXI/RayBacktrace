#ifndef RENDER_UTILITIES_H
#define RENDER_UTILITIES_H

#include <vector>

struct Point;
class Canvas;
class Polygon;
struct Line;
class SpotLight;
class Rgb;

Rgb TraceForPixel(int pixX, int pixY, const Polygon *polygons, int polCount,
                  const SpotLight *lights, int lightsCount);
void Trace(Line &ray, const Polygon *polygons, int polCount, const SpotLight *lights,
           int lightsCount, float &R, float &G, float &B, short lookCount);
//---------------------------------------------------------------------------
#endif
