//---------------------------------------------------------------------------
#include "Unit1.h"

#ifndef Unit2H
#define Unit2H

#define VISOR_Z               1200
#define DISPLAY_Z             500
#define Ia                    150
#define K_KOEFFICIENT         0.01
#define EPSILON               0.0005
#define EPSILON2              0.00000005
#define B_KOEFFICIENT         0.001
#define MAX_LOOKED_LINES_CNT  20
struct point
{
 float x, y, z;
 };

struct line
{
 float x0, y0, z0;
 float a, b, c;
 };

class spotLight;

class polygon
{
 private:
  point *top;               //Массив вершин.
  float A, B, C, D;         //Коэффициенты уравнения несущей плоскости.
  int n;                    //Кол-во вершин.
 public:
  polygon();
  ~polygon();
    int c_p_k;  //Степень косинуса для зеркальной составляющей освещённости.
    float Rka, Gka, Bka, Rkd, Gkd, Bkd, ks;     //Коэффициенты поверхности.
  void ChangePolygon(float x[], float y[], float z[], int n1, float r[], float g[], float b[], float ks_koeff, int cos_koeff);
  char PointInPolygon(float x, float y, float z);
  char LineCross(float &x, float &y, float &y0, float &x1, float &y1, float &x2, float &y2, float &y3);
  void operator=(polygon &Polygon2);
  void Rotate(float alpha, short axis);
  void Replace(float x1, float y1, float z1);
  void Scale(float t);
  void GetLine(const line &l, line &r);
  void Colors(float Rka, float Gka, float Bka, float Rkd, float Gkd, float Bkd, float ks, float n);
  //Функции-друзья класса.
  friend float CrossingParameter(line , polygon *, int , int &);
  friend void GetIntensivity(const float &, const float &, const float &, polygon *, const int &, spotLight *, const int &, float &, float &, float &, const int &, const line &l);
  friend void __fastcall TForm1::N2Click(TObject *Sender);
 };

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
  friend void __fastcall TForm1::N2Click(TObject *Sender);
  friend void __fastcall TForm1::N10Click(TObject *Sender);
 };

void Draw(TCanvas *display, int width, int height, polygon *obj, int np, spotLight *light, int nl);
void Trace(line &l, polygon *obj, int np, spotLight *light, int nl, float &R, float &G, float &B, short &looked_lines);
float CrossingParameter(line l, polygon *obj, int np, int &cross_pol_n);
//---------------------------------------------------------------------------
#endif
