#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Point.h"

class Polygon;
struct Line;

class SpotLight
{
 private:
  Point _place;          //Координаты источника света.
  float _intensivity;    //Интенсивность источника света.

 public:
  SpotLight();
  SpotLight(float x1, float y1, float z1, float I);
  void Rotate(float alpha, short axis);
  void Replace(float x1, float y1, float z1);
  void SetIntensivity(float I);

  const Point & Place() const;
  float Intensivity() const;

  float x() const;
  float y() const;
  float z() const;
  void setX(float value);
  void setY(float value);
  void setZ(float value);
};

inline const Point &SpotLight::Place() const
{
    return _place;
}

inline float SpotLight::Intensivity() const
{
    return _intensivity;
}

inline float SpotLight::x() const
{
    return _place.x;
}

inline float SpotLight::y() const
{
    return _place.y;
}

inline float SpotLight::z() const
{
    return _place.z;
}

inline void SpotLight::setX(float value)
{
    _place.x = value;
}

inline void SpotLight::setY(float value)
{
    _place.y = value;
}

inline void SpotLight::setZ(float value)
{
    _place.z = value;
}

#endif // SPOTLIGHT_H
