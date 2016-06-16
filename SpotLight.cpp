#include "SpotLight.h"

#include <math.h>

//Конструктор по умолчанию.
spotLight::spotLight()
{
}

//-----------------------------------------------------------------------------
//Метод для задания источника света.
void spotLight::ChangeSpotLight(float x1, float y1, float z1, float I)
{
place.x = x1;
place.y = y1;
place.z = z1;
intensivity = I;
}

//-----------------------------------------------------------------------------
//Метод для перемещения источника света.
void spotLight::Replace(float x1, float y1, float z1)
{
place.x += x1;
place.y += y1;
place.z += z1;
}

//-----------------------------------------------------------------------------
 //Метод для поворота источника света.
void spotLight::Rotate(float alpha, short axis)
{
float old, sinAlpha, cosAlpha;
sinAlpha = sin(alpha);
cosAlpha = cos(alpha);
switch (axis)
 {
  case 0: //Поворот вокруг оси Ox.
    old = place.y;
    place.y = cosAlpha * old  -  sinAlpha * place.z;
    place.z = sinAlpha * old  +  cosAlpha * place.z;
    break;
  case 1: //Поворот вокруг оси Oy.
    old = place.x;
    place.x = cosAlpha * old  +  sinAlpha * place.z;
    place.z = -sinAlpha * old  +  cosAlpha * place.z;
    break;
  default: //Поворот вокруг оси Oz.
    old = place.x;
    place.x = cosAlpha * old  -  sinAlpha * place.y;
    place.y = sinAlpha * old  +  cosAlpha * place.y;
  }
}
//Метод для изменения интенсивности источника света.
void spotLight::ChangeIntensivity(float I)
{
intensivity = I;
}
