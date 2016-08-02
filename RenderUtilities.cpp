#include <math.h>
#include <limits.h>

#include "RenderUtilities.h"

#include "Canvas.h"
#include "Polygon.h"
#include "Line.h"
#include "SpotLight.h"

#define EPSILON 0.0005
#define VISOR_Z               1200
#define DISPLAY_Z             500
#define Ia                    150
#define K_KOEFFICIENT         0.01
#define B_KOEFFICIENT         0.001
#define MAX_LOOKED_LINES_CNT  20

static void GetIntensivity(const float &x, const float &y, const float &z,
                           const Polygon *obj, const int &np,
                           const SpotLight *light,
                           const int &nl, float &R, float &G, float &B,
                           const int &cross_pol_n, const Line &l);
static float CrossingParameter(Line l, const Polygon *obj,
                               int np, int &cross_pol_n);

//Функция вычисления натуральной степени числа.
float Raise(float x, const int &n)
{
    float y;
    y = x;
    for (int i = 1; i < n; ++i)
        y *= x;
    return y;
}

//Реализация метода трассировки лучей.
void Draw(Canvas &canvas, const Polygon *obj, int np,
          const SpotLight *light, int nl)
{
    float R, G, B;
    Line l;
    short looked_lines;
    //Подготовка.
    canvas.clear();

    looked_lines = 0;
    l.z0 = DISPLAY_Z;
    const int width = canvas.width() / 2;
    const int height = canvas.height() / 2;

    //Начало алгоритма трассировки лучей: перебор всех пикселей на окне.
    for (l.x0=-width; l.x0<width; ++l.x0)
        for (l.y0=-height + 1; l.y0<height; ++l.y0)
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
            canvas.setPixel((int)l.x0+width, height-(int)l.y0, Rgb(R, G, B));
        }
}

//Процедура трассировки одного луча.
void Trace(Line &l, const Polygon *obj, int np, const SpotLight *light,
           int nl, float &R, float &G, float &B, short &looked_lines)
{
    float t, d;
    Line r, p;
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
            const float ks = obj[cross_pol_n].GetKs();
            Rotr *= ks;
            Gotr *= ks;
            Botr *= ks;
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
float CrossingParameter(Line l, const Polygon *obj, int np, int &cross_pol_n)
{
    int i;
    float t, tmin;
    tmin = float(INT_MAX);
    for (i=0; i<np; ++i)
    {
        t = obj[i].GetA() * l.a  +  obj[i].GetB() * l.b  +  obj[i].GetC() * l.c;
        if (t != 0)
        {
            t = - (obj[i].GetA() * l.x0  +  obj[i].GetB() * l.y0  +
                obj[i].GetC() * l.z0  +  obj[i].GetD()) / t;
            if (t > EPSILON  &&  t < tmin)  //Если найдено пересечение с несущей плоскостью, и оно ближе остальных найденных на данный момент пересечений.
                //+Уточнение, пересекает ли луч многоугольник.
                if (obj[i].PointInPolygon(l.x0 + l.a*t,
                        l.y0 + l.b*t, l.z0 + l.c*t))
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
void GetIntensivity(const float &x, const float &y, const float &z,
                    const Polygon *obj, const int &np, const SpotLight *light,
                    const int &nl, float &R, float &G, float &B,
                    const int &cross_pol_n, const Line &l)
{
    char flag;
    Line line_to_light;
    float t, cosT, d, cosA;
    int i, j;

    //Вычисление рассеянной составляющей.
    const Rgb diffusedRgb = obj[cross_pol_n].KaColor();
    R = Ia * diffusedRgb.red();
    G = Ia * diffusedRgb.green();
    B = Ia * diffusedRgb.blue();

    //Вычисления составлющих от диффузионного рассеивания и зеркального отражения.
    line_to_light.x0 = x;
    line_to_light.y0 = y;
    line_to_light.z0 = z;
    for (i=0; i<nl; ++i)          //Перебор источников света.
    {
        //формирование луча от рассматриваемой точки до источника света.
        line_to_light.a = light[i].x() - x;
        line_to_light.b = light[i].y() - y;
        line_to_light.c = light[i].z() - z;

        //Поиск пересечения луча с другими объектами сцены.
        flag = 1;
        for (j=0; j<cross_pol_n; ++j)       //Перебор многоугольников.
        {
            t = obj[j].GetA() * line_to_light.a  +
                obj[j].GetB() * line_to_light.b  +
                obj[j].GetC() * line_to_light.c;
            if (t != 0)
            {
                t = -(obj[j].GetA() * line_to_light.x0  +
                    obj[j].GetB() * line_to_light.y0  +
                    obj[j].GetC() * line_to_light.z0  +  obj[j].GetD())/t;
                if (t > EPSILON  &&  t < 1)      // ~t>0.
                    //+Уточнение, пересекает ли луч многоугольник.
                    if (obj[j].PointInPolygon(
                            line_to_light.x0 + line_to_light.a*t,
                            line_to_light.y0 + line_to_light.b*t,
                            line_to_light.z0 + line_to_light.c*t))
                    {
                        flag = 0;
                        break;
                    }
            }
        }
        if (flag)
            for (++j; j<np; ++j)              //Перебор оставшихся многоугольников.
            {
                t = obj[j].GetA() * line_to_light.a  +
                    obj[j].GetB() * line_to_light.b  +
                    obj[j].GetC() * line_to_light.c;
                if (t != 0)
                {
                    t = -(obj[j].GetA() * line_to_light.x0  +
                        obj[j].GetB() * line_to_light.y0  +
                        obj[j].GetC() * line_to_light.z0  +  obj[j].GetD())/t;
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
            cosA = sqrt(line_to_light.a*line_to_light.a +
                line_to_light.b*line_to_light.b +
                line_to_light.c*line_to_light.c);
            cosT = obj[cross_pol_n].GetA() * line_to_light.a  +
                obj[cross_pol_n].GetB() * line_to_light.b  +
                obj[cross_pol_n].GetC() * line_to_light.c;
            cosT /= sqrt(obj[cross_pol_n].GetA()*obj[cross_pol_n].GetA() +
                obj[cross_pol_n].GetB()*obj[cross_pol_n].GetB() +
                obj[cross_pol_n].GetC()*obj[cross_pol_n].GetC())  *  cosA;

            //Вычисление освещённости диффузионно отражённого света от i-го источника.
            d = sqrt(line_to_light.a*line_to_light.a +
                line_to_light.b*line_to_light.b +
                line_to_light.c*line_to_light.c);
            t = cosT * light[i].Intensivity() / (d + K_KOEFFICIENT);

            const Rgb reflectedRgb = obj[cross_pol_n].KdColor();
            R += reflectedRgb.red() * t;
            G += reflectedRgb.green() * t;
            B += reflectedRgb.blue() * t;

            //Вычисление освещённости зеркально отражённого света от i-го источника.
            //Нормирование вектора line_to_light.
            line_to_light.a /= cosA;
            line_to_light.b /= cosA;
            line_to_light.c /= cosA;
            //Формирование направляющего вектора луча, зеркально отражённого от line_to_light.
            cosT += cosT;
            line_to_light.a = cosT * obj[cross_pol_n].GetA()  -  line_to_light.a;
            line_to_light.b = cosT * obj[cross_pol_n].GetB()  -  line_to_light.b;
            line_to_light.c = cosT * obj[cross_pol_n].GetC()  -  line_to_light.c;
            //Вычисление косинуса угла между отражённым лучом и исходным лучом l.
            cosA = l.a * line_to_light.a  +  l.b * line_to_light.b  +
                l.c * line_to_light.c;
            //Вычисление освещённости зеркально отражённого света.
            cosA = Raise(fabs(cosA), obj[cross_pol_n].CosPower()) *
                obj[cross_pol_n].GetKs() *
                light[i].Intensivity() / (d + K_KOEFFICIENT);
            R += cosA;
            G += cosA;
            B += cosA;
        }
    }
}
