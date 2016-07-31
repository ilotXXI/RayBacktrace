#include "SpotLight.h"

#include <math.h>

//Конструктор по умолчанию.
SpotLight::SpotLight()
{
}

SpotLight::SpotLight(float x1, float y1, float z1, float I)
    : _place(x1, y1, z1)
    , _intensivity(I)
{
}

//-----------------------------------------------------------------------------
//Метод для перемещения источника света.
void SpotLight::Replace(float x1, float y1, float z1)
{
    _place.x += x1;
    _place.y += y1;
    _place.z += z1;
}

//-----------------------------------------------------------------------------
//Метод для поворота источника света.
void SpotLight::Rotate(float alpha, short axis)
{
    float old, sinAlpha, cosAlpha;
    sinAlpha = sin(alpha);
    cosAlpha = cos(alpha);
    switch (axis)
    {
    case 0: //Поворот вокруг оси Ox.
        old = _place.y;
        _place.y = cosAlpha * old  -  sinAlpha * _place.z;
        _place.z = sinAlpha * old  +  cosAlpha * _place.z;
        break;
    case 1: //Поворот вокруг оси Oy.
        old = _place.x;
        _place.x = cosAlpha * old  +  sinAlpha * _place.z;
        _place.z = -sinAlpha * old  +  cosAlpha * _place.z;
        break;
    default: //Поворот вокруг оси Oz.
        old = _place.x;
        _place.x = cosAlpha * old  -  sinAlpha * _place.y;
        _place.y = sinAlpha * old  +  cosAlpha * _place.y;
    }
}
//Метод для изменения интенсивности источника света.
void SpotLight::SetIntensivity(float I)
{
    _intensivity = I;
}
