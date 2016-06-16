#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Point.h"

class polygon;
struct line;

class spotLight
{
 private:
  point place;          //Координаты источника света.
  float intensivity;    //Интенсивность источника света.

 public:
  spotLight();
  void ChangeSpotLight(float x1, float y1, float z1, float I);
  void Rotate(float alpha, short axis);
  void Replace(float x1, float y1, float z1);
  void ChangeIntensivity(float I);

  //Функции-друзья класса.
  friend void GetIntensivity(const float &, const float &, const float &, polygon *, const int &, spotLight *, const int &, float &, float &, float &, const int &, const line &l);
 };

#endif // SPOTLIGHT_H
