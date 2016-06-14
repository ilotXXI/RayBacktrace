#include <math.h>
#include <limits.h>

//#include "Unit1.h"
#include "Unit2.h"

//Функция вычисления натуральной степени числа.
float Raise(float x, const int &n)
{
 float y;
 y = x;
 for(int i=1; i<n; ++i)
  y *= x;
 return y;
 }

//Методы используемых классов.
 //Для многоугольника.
  //Конструктор по умолчанию.
polygon::polygon()
{
 top = NULL;
 }

//-----------------------------------------------------------------------------
  //Метод для задания многоугольника.
void polygon::ChangePolygon(float x[], float y[], float z[], int n1, float r[], float g[], float b[], float ks_koeff, int cos_koeff)
{
 int i;
 //Запись массива вершин.
 if (top)
  delete[] top;
 n = n1;
 top = new point[n];
 for (i=0; i<n; ++i)
  {
   top[i].x = x[i];
   top[i].y = y[i];
   top[i].z = z[i];
   }
 //Запись коэффициентов отражения для грани.
 Rka = r[0];  Rkd = r[1];
 Gka = g[0];  Gkd = g[1];
 Bka = b[0];  Bkd = b[1];
    ks = ks_koeff;  c_p_k = cos_koeff;
 //Вычисление коэффициентов для уравнения несущей плоскости.
 A = (y[1] - y[0])*(z[2] - z[0]) - (y[2] - y[0])*(z[1] - z[0]);
 B = (z[1] - z[0])*(x[2] - x[0]) - (x[1] - x[0])*(z[2] - z[0]);
 C = (x[1] - x[0])*(y[2] - y[0]) - (y[1] - y[0])*(x[2] - x[0]);
 //Нормирование вектора нормали.
 D = sqrt(A*A + B*B + C*C); //D пока используется как промежуточная переменная.
 A /= D;
 B /= D;
 C /= D;
 D = -(A*x[0] + B*y[0] + C*z[0]);
 }

//-----------------------------------------------------------------------------
  //Деструктор.
polygon::~polygon()
{
 delete[] top;
 top = NULL;
 }

//-----------------------------------------------------------------------------
  //Метод для определения, лежит ли точка в многоугольнике, если она лежит на его несущей плоскости.
char polygon::PointInPolygon(float x, float y, float z)
{
 int i, m;
 m = 0;
 //Определение, на какую плоскость можно спроецировать многоугольник и точку.
 if(fabs(C) > EPSILON)
  {//Если несущая плоскость многоугольника не перпендикулярна плоскости XY, то многоугольник и точку можно спроецировать на неё.
   switch(LineCross(x, y, top[n-1].y, top[0].x, top[0].y, top[1].x, top[1].y, top[2].y))
    {
     case 1:  //Здесь стоит switch с 2 case'ами, потому что функция LineCross может возвращать также и значение 0, и в этом случае не нужно ничего делать.
       ++m;
       break;
     case -1:
       return 1;
     }
   n -= 2;  //Две последние стороны приходится проверять отдельно из-за индексов.
   for(i=1; i<n; ++i)
     //Проверка, пересекает ли луч очередную сторону.
     switch(LineCross(x, y, top[i-1].y, top[i].x, top[i].y, top[i+1].x, top[i+1].y, top[i+2].y))
      {
       case 1:
         ++m;
         break;
       case -1:
         n += 2;
         return 1;
       }
   //Осталось проверить ещё 2 стороны.
    //Сторона (n-2; n-1) [в силу переобозначения - (n; n+1)].
   switch(LineCross(x, y, top[n-1].y, top[n].x, top[n].y, top[n+1].x, top[n+1].y, top[0].y))
    {
     case 1:
       ++m;
       break;
     case -1:
       n += 2;
       return 1;
     }
    //Сторона (n-1; 0) [в силу переобозначения(после следующего ++n) - (n; 0)].
   ++n;
   switch(LineCross(x, y, top[n-1].y, top[n].x, top[n].y, top[0].x, top[0].y, top[1].y))
    {
     case 1:
       ++m;
       break;
     case -1:
       ++n;
       return 1;
     }
   //Теперь нужно восстановить число углов многоугольника и вернуть результат.
   ++n;
   return m % 2;
   }

 //Продолжение определения плоскости проецирования.
 if(fabs(B) > EPSILON)
  {//Если несущая плоскость многоугольника перпендикулярна плоскости XY, но не перпендикулярна плоскости XZ, то многоугольник и точку можно спроецировать на XZ.
   switch(LineCross(x, z, top[n-1].z, top[0].x, top[0].z, top[1].x, top[1].z, top[2].z))
    {
     case 1:
       ++m;
       break;
     case -1:
       return 1;
     }
   n -= 2;  //Две последние стороны приходится проверять отдельно из-за индексов.
   for(i=1; i<n; ++i)
     //Проверка, пересекает ли луч очередную сторону.
     switch(LineCross(x, z, top[i-1].z, top[i].x, top[i].z, top[i+1].x, top[i+1].z, top[i+2].z))
      {
       case 1:
         ++m;
         break;
       case -1:
         n += 2;
         return 1;
       }
   //Осталось проверить ещё 2 стороны.
    //Сторона (n-2; n-1) [в силу переобозначения - (n; n+1)].
   switch(LineCross(x, z, top[n-1].z, top[n].x, top[n].z, top[n+1].x, top[n+1].z, top[0].z))
    {
     case 1:
       ++m;
       break;
     case -1:
       n += 2;
       return 1;
     }
    //Сторона (n-1; 0) [в силу переобозначения(после следующего ++n) - (n; 0)].
   ++n;
   switch(LineCross(x, z, top[n-1].z, top[n].x, top[n].z, top[0].x, top[0].z, top[1].z))
    {
     case 1:
       ++m;
       break;
     case -1:
       ++n;
       return 1;
     }
   //Теперь нужно восстановить число углов многоугольника и вернуть результат.
   ++n;
   return m % 2;
   }

 //Последняя "стадия" определения плоскости проецирования.
 //Если несущая плоскость многоугольника перпендикулярна и плоскости XY, и плоскости XZ, то многоугольник и точку можно спроецировать на YZ.
   switch(LineCross(y, z, top[n-1].z, top[0].y, top[0].z, top[1].y, top[1].z, top[2].z))
    {
     case 1:
       ++m;
       break;
     case -1:
       return 1;
     }
   n -= 2;  //Две последние стороны приходится проверять отдельно из-за индексов.
   for(i=1; i<n; ++i)
     //Проверка, пересекает ли луч очередную сторону.
     switch(LineCross(y, z, top[i-1].z, top[i].y, top[i].z, top[i+1].y, top[i+1].z, top[i+2].z))
      {
       case 1:
         ++m;
         break;
       case -1:
         n += 2;
         return 1;
       }
   //Осталось проверить ещё 2 стороны.
    //Сторона (n-2; n-1) [в силу переобозначения - (n; n+1)].
   switch(LineCross(y, z, top[n-1].z, top[n].y, top[n].z, top[n+1].y, top[n+1].z, top[0].z))
    {
     case 1:
       ++m;
       break;
     case -1:
       n += 2;
       return 1;
     }
    //Сторона (n-1; 0) [в силу переобозначения(после следующего ++n) - (n; 0)].
   ++n;
   switch(LineCross(y, z, top[n-1].z, top[n].y, top[n].z, top[0].y, top[0].z, top[1].z))
    {
     case 1:
       ++m;
       break;
     case -1:
       ++n;
       return 1;
     }
   //Теперь нужно восстановить число углов многоугольника и вернуть результат.
   ++n;
   return m % 2;
 }









 /*
 int i;
 //Проверка, не совпадает ли точка с одной из вершин.
 for (i=0; i<n; ++i)
  if(top[i].x==x && top[i].y==y && top[i].z==z)
   return 1;
 //Переменные.
 float Vx[2], Vy[2], Vz[2];
 char first, second;
 double Alpha;
 //Вычисление суммы углов.
 first = 0;
 second = 1;
 Alpha = 0;
 Vx[0] = top[0].x - x;
 Vy[0] = top[0].y - y;
 Vz[0] = top[0].z - z;
 for(i=1; i<n; ++i)
  {
   //формирование вектора.
   Vx[second] = top[i].x - x;
   Vy[second] = top[i].y - y;
   Vz[second] = top[i].z - z;
   //Добавление очередного угла между 2 отрезками.
   Alpha += acos( (Vx[first]*Vx[second] + Vy[first]*Vy[second] + Vz[first]*Vz[second])   /   ( sqrt(Vx[first]*Vx[first] + Vy[first]*Vy[first] + Vz[first]*Vz[first])  *  sqrt(Vx[second]*Vx[second] + Vy[second]*Vy[second] + Vz[second]*Vz[second])));
   //Изменение переменных first и second (они нужны, чтобы не вычислять один и тот же вектор несколько раз).
   if (first)
     {
      first = 0;
      second = 1;
      }
    else
     {
      first = 1;
      second = 0;
      }
   }
 //Вычисление оставшегося угла между первым (0) и последним (n-1) отрезками.
 Vx[second] = top[0].x - x;
 Vy[second] = top[0].y - y;
 Vz[second] = top[0].z - z;
 Alpha += acos( (Vx[first]*Vx[second] + Vy[first]*Vy[second] + Vz[first]*Vz[second])   /   ( sqrt(Vx[first]*Vx[first] + Vy[first]*Vy[first] + Vz[first]*Vz[first])  *  sqrt(Vx[second]*Vx[second] + Vy[second]*Vy[second] + Vz[second]*Vz[second])));
 //Сравнение, равна ли найденная сумма углов 360 градусам.
 if (fabs(Alpha - 6.2832)  <  EPSILON)
   return 1;
  else
   return 0;
 }                 */

//-----------------------------------------------------------------------------
 //Метод для определения, пересекает ли луч сторону многоугольника.
char polygon::LineCross(float &x, float &y, float &y0, float &x1, float &y1, float &x2, float &y2, float &y3)
{
 double t, q;
 if (!(x>x1  &&  x>x2))
  {if(fabs(y1 - y2) < EPSILON2) //Если сторона горизонтальна.
    {
     if(fabs(y1 - y) < EPSILON2)
      {
       if(x1 < x2)
         {
          t = x1;
          q = x2;
          }
        else
         {
          t = x2;
          q = x1;
          }
       if(t<=x && x<=q)
        return -1;
       if((y1 - y0) * (y2 - y3) > 0)
        return 1;
       }
     }
    else
     {
      q = (y - y1) / (y2 - y1);
      if(q>EPSILON2  &&  q<=1)   //Если прямая, содержащая луч, пересекает сторону.
        {
         t = x1 - x + (x2 - x1)*q;
         if(t > EPSILON2) //Если луч перескает сторону.
          {
           if(fabs(q-1)<EPSILON2) //Если луч пересекает вершину.
            {if((y2 - y1) * (y2 - y3)  <= 0)
               return 1;
              else
               return 0;
             }
           return 1;
           }
          else
           if(fabs(t) < EPSILON2)  //Если точка лежит на стороне, то она принадлежит многоугольнику.
             return -1;
          }
      }
   }
 return 0;
 }

//-----------------------------------------------------------------------------
 //Метод для перемещения многоугольника.
void polygon::Replace(float x1, float y1, float z1)
{
 int i;
 for (i=0; i<n; ++i)
  {
   top[i].x += x1;
   top[i].y += y1;
   top[i].z += z1;
   }
 //Перевычисление коэффициентов для уравнения несущей плоскости.
 A = (top[1].y - top[0].y)*(top[2].z - top[0].z) - (top[2].y - top[0].y)*(top[1].z - top[0].z);
 B = (top[1].z - top[0].z)*(top[2].x - top[0].x) - (top[1].x - top[0].x)*(top[2].z - top[0].z);
 C = (top[1].x - top[0].x)*(top[2].y - top[0].y) - (top[1].y - top[0].y)*(top[2].x - top[0].x);
 //Нормирование вектора нормали.
 D = sqrt(A*A + B*B + C*C); //D пока используется как промежуточная переменная.
 A /= D;
 B /= D;
 C /= D;
 D = -(A*top[0].x + B*top[0].y + C*top[0].z);
 }

//-----------------------------------------------------------------------------
 //Метод для поворота многоугольника.
void polygon::Rotate(float alpha, short axis)
{
 float old, sinAlpha, cosAlpha;
 int i;
 sinAlpha = sin(alpha);
 cosAlpha = cos(alpha);
 switch (axis)
  {
   case 0: //Поворот вокруг оси Ox.
     for(i=0; i<n; ++i)
      {
       old = top[i].y;
       top[i].y = cosAlpha * old  -  sinAlpha * top[i].z;
       top[i].z = sinAlpha * old  +  cosAlpha * top[i].z;
       }
     break;
   case 1: //Поворот вокруг оси Oy.
     for(i=0; i<n; ++i)
      {
       old = top[i].x;
       top[i].x = cosAlpha * old  +  sinAlpha * top[i].z;
       top[i].z = -sinAlpha * old  +  cosAlpha * top[i].z;
       }
     break;
   default: //Поворот вокруг оси Oz.
     for(i=0; i<n; ++i)
      {
       old = top[i].x;
       top[i].x = cosAlpha * old  -  sinAlpha * top[i].y;
       top[i].y = sinAlpha * old  +  cosAlpha * top[i].y;
       }
   }
 //Перевычисление коэффициентов для уравнения несущей плоскости.
 A = (top[1].y - top[0].y)*(top[2].z - top[0].z) - (top[2].y - top[0].y)*(top[1].z - top[0].z);
 B = (top[1].z - top[0].z)*(top[2].x - top[0].x) - (top[1].x - top[0].x)*(top[2].z - top[0].z);
 C = (top[1].x - top[0].x)*(top[2].y - top[0].y) - (top[1].y - top[0].y)*(top[2].x - top[0].x);
 //Нормирование вектора нормали.
 D = sqrt(A*A + B*B + C*C); //D пока используется как промежуточная переменная.
 A /= D;
 B /= D;
 C /= D;
 D = -(A*top[0].x + B*top[0].y + C*top[0].z);
 }

//-----------------------------------------------------------------------------
 //Метод для масштабирования многоугольника.
void polygon::Scale(float t)
{
 for(int i=0; i<n; ++i)
  {
   top[i].x *= t;
   top[i].y *= t;
   top[i].z *= t;
   }
 //Перевычисление коэффициентов для уравнения несущей плоскости.
 A = (top[1].y - top[0].y)*(top[2].z - top[0].z) - (top[2].y - top[0].y)*(top[1].z - top[0].z);
 B = (top[1].z - top[0].z)*(top[2].x - top[0].x) - (top[1].x - top[0].x)*(top[2].z - top[0].z);
 C = (top[1].x - top[0].x)*(top[2].y - top[0].y) - (top[1].y - top[0].y)*(top[2].x - top[0].x);
 //Нормирование вектора нормали.
 D = sqrt(A*A + B*B + C*C); //D пока используется как промежуточная переменная.
 A /= D;
 B /= D;
 C /= D;
 D = -(A*top[0].x + B*top[0].y + C*top[0].z);
 }

//-----------------------------------------------------------------------------
 //Метод для вычисления координат направляющего вектора отражённого луча.
void polygon::GetLine(const line &l, line &r)
{
 float q;
 q = l.a * A  +  l.b * B  +  l.c * C;
 q += q;             // q = 2*(L; n).
 r.a = l.a - q*A;
 r.b = l.b - q*B;
 r.c = l.c - q*C;    // R = -L + q*n.
 }

//-----------------------------------------------------------------------------
void polygon::Colors(float Rka1, float Gka1, float Bka1, float Rkd1, float Gkd1, float Bkd1, float ks1, float n1)
{
 if(Rka1 >= 0) Rka = Rka1;
 if(Gka1 >= 0) Gka = Gka1;
 if(Bka1 >= 0) Bka = Bka1;
 if(Rkd1 >= 0) Rkd = Rkd1;
 if(Gkd1 >= 0) Gkd = Gkd1;
 if(Bkd1 >= 0) Bkd = Bkd1;
 if(ks1 >= 0) ks = ks1;
 if(n1 >= 0) c_p_k = n1;
 }

//*****************************************************************************
//Для источника света.
 //Конструктор по умолчанию.
spotLight::spotLight()
{ }

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

//*****************************************************************************
//Реализация метода трассировки лучей.
void Draw(TCanvas *display, int width, int height, polygon obj[], int np, spotLight light[], int nl)
{
 float R, G, B;
 line l;
 short looked_lines;
 //Подготовка.
 display->Pen->Color = clBtnFace;
 display->Rectangle(0, 0, width, height);
 looked_lines = 0;
 l.z0 = DISPLAY_Z;
 width /= 2;
 height /= 2;
 //Начало алгоритма трассировки лучей: перебор всех пикселей на окне.
 for (l.x0=-width; l.x0<width; ++l.x0)
  for (l.y0=-height; l.y0<height; ++l.y0)
   {
    //+Формирование луча через пиксель (x0; y0).
    l.c = DISPLAY_Z - VISOR_Z;
    l.a = l.x0;
    l.b = l.y0;
    //+Трассировка сформированного луча.
    Trace(l, obj, np, light, nl, R, G, B, looked_lines);
    //Закраска пикселя "найденным" цветом.
    if (R > 255  ||  R < 0)
     R = 255;
    if (G > 255  ||  G < 0)
     G = 255;
    if (B > 255  ||  B < 0)
     B = 255;
    display->Pixels[(int)l.x0+width][height-(int)l.y0] = ((int)B)*0x010000 + ((int)G)*0x0100 + (int)R;
    }
 }
 
//Процедура трассировки одного луча.
void Trace(line &l, polygon *obj, int np, spotLight *light, int nl, float &R, float &G, float &B, short &looked_lines)
{
 float t, d;
 line r, p;
 float Rotr, Gotr, Botr;
 float Rprel, Gprel, Bprel;
 int cross_pol_n;
 //+Определение пересечения луча l.
 t = CrossingParameter(l, obj, np, cross_pol_n);
 if (t <= 0)        //Если нет пересечения.
   R = G = B = 0;
  else
   {
    //+Определение расстояния d до точки пересечения.
    d = sqrt(l.a*l.a + l.b*l.b + l.c*l.c); //Пока вычислим только норму направляющего вектора, т.к. она нам ещё понадобится. Позже её нужно будет домножить на t.
    //+Вычисление непосредственной освещённости.
     //Предварительные вычисления.
    r.x0 = l.x0+l.a*t;    // -
    r.y0 = l.y0+l.b*t;    // - здесь используем поля r как промежуточные переменные, т.к. позднее эти же значения будут нужны для формирования отражённого луча r.
    r.z0 = l.z0+l.c*t;    // -
     //Нормирование направляющего вектора исходного луча.
    l.a /= d;
    l.b /= d;
    l.c /= d;
    d *= t; //Теперь норма направляющего вектора нам не нужна, и можно вычислить расстояние.
     //Вычисление интенсивности.
    GetIntensivity(r.x0, r.y0, r.z0, obj, np, light, nl, R, G, B, cross_pol_n, l);
    if (0)                          /*-Если поверхность прозрачна*/
        {
         //-Формирование преломлённого луча p.
         //-Трассировка p.
         Trace(p, obj, np, light, nl, Rprel, Gprel, Bprel, looked_lines);
         //-   Iпр. = It * kt.
         }
     else
      {//+Iпр. = 0
       Rprel = 0;
       Gprel = 0;
       Bprel = 0;
       }
    //Если рассмотрено не слишком много отражённых лучей, то нужно рассмотреть отражённый луч.
    if(looked_lines < MAX_LOOKED_LINES_CNT)
      {
       //+Формирование отражённого луча r.
       obj[cross_pol_n].GetLine(l, r);
       //+Трассировка r.
       ++looked_lines;
       Trace(r, obj, np, light, nl, Rotr, Gotr, Botr, looked_lines);
       --looked_lines;
       //Обработка полученных интенсивностей.
       Rotr *= obj[cross_pol_n].ks;
       Gotr *= obj[cross_pol_n].ks;
       Botr *= obj[cross_pol_n].ks;
       if(Rotr < -EPSILON  ||  Rotr > 255)
        Rotr = 255;
       if(Gotr < -EPSILON  ||  Gotr > 255)
        Gotr = 255;
       if(Botr < -EPSILON  ||  Botr > 255)
        Botr = 255;
       }
     else
      {
       //Если рассмотрено слишком много лучей, то далее формировать и трассировать новые отражённые лучи не будем.
       Rotr = 0;
       Gotr = 0;
       Botr = 0;
       }
    /* Iотр = Is * ks;
       Ih = Ial + Iпр + Iотр;
       I = Ih * exp(-b*d) */
    R += Rotr + Rprel;
    G += Gotr + Gprel;
    B += Botr + Bprel;
    d = exp(-B_KOEFFICIENT * d);
    R *= d;
    G *= d;
    B *= d;
    }
 }

//Функция определения параметра и номера для пересечения луча с многоугольником.
float CrossingParameter(line l, polygon *obj, int np, int &cross_pol_n)
{
 int i;
 float t, tmin;
 tmin = INT_MAX;
 for (i=0; i<np; ++i)
  {
   t = obj[i].A * l.a  +  obj[i].B * l.b  +  obj[i].C * l.c;
   if (t != 0)
    {
     t = -(obj[i].A * l.x0  +  obj[i].B * l.y0  +  obj[i].C * l.z0  +  obj[i].D)/t;
     if (t > EPSILON  &&  t < tmin)  //Если найдено пересечение с несущей плоскостью, и оно ближе остальных найденных на данный момент пересечений.
       //+Уточнение, пересекает ли луч многоугольник.
       if (obj[i].PointInPolygon(l.x0 + l.a*t,  l.y0 + l.b*t,  l.z0 + l.c*t))
        {
         tmin = t;
         cross_pol_n = i;
         }
     }
   }
 if (tmin == INT_MAX)
   return -1.;
  else
   return tmin;
 }

//Функция вычисления непосредственной освещённости.
void GetIntensivity(const float &x, const float &y, const float &z, polygon *obj, const int &np, spotLight *light, const int &nl, float &R, float &G, float &B, const int &cross_pol_n, const line &l)
{
 char flag;
 line line_to_light;
 float t, cosT, d, cosA, otr_vekt_x, otr_vekt_y, otr_vekt_z;
 int i, j;
 //Вычисление рассеянной составляющей.
 R = Ia * obj[cross_pol_n].Rka;
 G = Ia * obj[cross_pol_n].Gka;
 B = Ia * obj[cross_pol_n].Bka;
 //Вычисления составлющих от диффузионного рассеивания и зеркального отражения.
 line_to_light.x0 = x;
 line_to_light.y0 = y;
 line_to_light.z0 = z;
 for (i=0; i<nl; ++i)          //Перебор источников света.
  {
   //формирование луча от рассматриваемой точки до источника света.
   line_to_light.a = light[i].place.x - x;
   line_to_light.b = light[i].place.y - y;
   line_to_light.c = light[i].place.z - z;
   //Поиск пересечения луча с другими объектами сцены.
   flag = 1;
   for (j=0; j<cross_pol_n; ++j)       //Перебор многоугольников.
    {
     t = obj[j].A * line_to_light.a  +  obj[j].B * line_to_light.b  +  obj[j].C * line_to_light.c;
     if (t != 0)
      {
       t = -(obj[j].A * line_to_light.x0  +  obj[j].B * line_to_light.y0  +  obj[j].C * line_to_light.z0  +  obj[j].D)/t;
       if (t > EPSILON  &&  t < 1)      // ~t>0.
         //+Уточнение, пересекает ли луч многоугольник.
         if (obj[j].PointInPolygon(line_to_light.x0 + line_to_light.a*t,  line_to_light.y0 + line_to_light.b*t,  line_to_light.z0 + line_to_light.c*t))
          {
           flag = 0;
           break;
           }
       }
     }
   if (flag)
     for (++j; j<np; ++j)              //Перебор оставшихся многоугольников.
      {
       t = obj[j].A * line_to_light.a  +  obj[j].B * line_to_light.b  +  obj[j].C * line_to_light.c;
       if (t != 0)
        {
         t = -(obj[j].A * line_to_light.x0  +  obj[j].B * line_to_light.y0  +  obj[j].C * line_to_light.z0  +  obj[j].D)/t;
         if (t > EPSILON  &&  t < 1)      // ~t>0.
           //+Уточнение, пересекает ли луч многоугольник.
           if (obj[j].PointInPolygon(line_to_light.x0 + line_to_light.a*t,  line_to_light.y0 + line_to_light.b*t,  line_to_light.z0 + line_to_light.c*t))
            {
             flag = 0;
             break;
             }
         }
       }
    else
     continue;
   if (flag) //Если луч не пересекает ни одного многоугольника.
    {
     //Поиск косинуса угла между нормалью к многоугольнику и лучём.
           //Норму line_to_light пока запомним в cosA, т.к. она нам понадобится.
     cosA = sqrt(line_to_light.a*line_to_light.a + line_to_light.b*line_to_light.b + line_to_light.c*line_to_light.c);
     cosT = obj[cross_pol_n].A * line_to_light.a  +  obj[cross_pol_n].B * line_to_light.b  +  obj[cross_pol_n].C * line_to_light.c;
     cosT /= sqrt(obj[cross_pol_n].A*obj[cross_pol_n].A + obj[cross_pol_n].B*obj[cross_pol_n].B + obj[cross_pol_n].C*obj[cross_pol_n].C)  *  cosA;
     //Вычисление освещённости диффузионно отражённого света от i-го источника.
     d = sqrt(line_to_light.a*line_to_light.a + line_to_light.b*line_to_light.b + line_to_light.c*line_to_light.c);
     t = cosT * light[i].intensivity / (d + K_KOEFFICIENT);
     R += obj[cross_pol_n].Rkd * t;
     G += obj[cross_pol_n].Gkd * t;
     B += obj[cross_pol_n].Bkd * t;
     //Вычисление освещённости зеркально отражённого света от i-го источника.
      //Нормирование вектора line_to_light.
      line_to_light.a /= cosA;
      line_to_light.b /= cosA;
      line_to_light.c /= cosA;
      //Формирование направляющего вектора луча, зеркально отражённого от line_to_light.
      cosT += cosT;
      line_to_light.a = cosT * obj[cross_pol_n].A  -  line_to_light.a;
      line_to_light.b = cosT * obj[cross_pol_n].B  -  line_to_light.b;
      line_to_light.c = cosT * obj[cross_pol_n].C  -  line_to_light.c;
      //Вычисление косинуса угла между отражённым лучом и исходным лучом l.
      cosA = l.a * line_to_light.a  +  l.b * line_to_light.b  +  l.c * line_to_light.c;
      //Вычисление освещённости зеркально отражённого света.
      cosA = Raise(fabs(cosA), obj[cross_pol_n].c_p_k)  *  obj[cross_pol_n].ks  *  light[i].intensivity  /  (d + K_KOEFFICIENT);
      R += cosA;
      G += cosA;
      B += cosA;
     }
   }
 }
