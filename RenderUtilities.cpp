#include <math.h>
#include <limits.h>

#include "RenderUtilities.h"

#include "Point.h"
#include "Canvas.h"
#include "Polygon.h"
#include "Line.h"
#include "SpotLight.h"

#define EPSILON                 0.0005
#define VISOR_Z                 1200
#define DISPLAY_Z               500
#define Ia                      150
#define K_KOEFFICIENT           0.01f
#define B_KOEFFICIENT           0.001f
#define MAX_LOOKED_LINES_CNT    20

static void GetIntensivity(const float &x, const float &y, const float &z,
                           const Polygon *obj, const int &np,
                           const SpotLight *light,
                           const int &nl, float &R, float &G, float &B,
                           const int &cross_pol_n, const Line &l);
// If no cross is found the crossedPolInd is negative.
static float CrossingParameter(const Line &l, const Polygon *obj,
                               int np, int &crossedPolInd);
static Line reflectedRay(const Polygon &from, const Line &instRay,
                         const Point &reflectionPoint);

//Функция вычисления натуральной степени числа.
float Raise(float base, int power)
{
    float y = base;
    for (int i = 1; i < power; ++i)
        y *= base;
    return y;
}

Rgb TraceForPixel(int pixX, int pixY, const Polygon *polygons, int polCount,
                  const SpotLight *lights, int lightsCount)
{
    Line ray;
    ray.z0 = DISPLAY_Z;
    ray.c = DISPLAY_Z - VISOR_Z;
    ray.a = ray.x0 = float(pixX);
    ray.b = ray.y0 = float(pixY);

    float R, G, B;
    Trace(ray, polygons, polCount, lights, lightsCount, R, G, B, 0);

    //Закраска пикселя вычисленным цветом.
    if (R > 255  ||  R < 0)
        R = 255;
    if (G > 255  ||  G < 0)
        G = 255;
    if (B > 255  ||  B < 0)
        B = 255;

    return Rgb(R, G, B);
}

//Процедура трассировки одного луча.
void Trace(Line &ray, const Polygon *polygons, int polCount,
           const SpotLight *lights, int lightsCount,
           float &R, float &G, float &B, short lookCount)
{
    float Rotr, Gotr, Botr;
    int crossedPolInd = 0;

    //+Определение пересечения луча l.
    const float t = CrossingParameter(ray, polygons, polCount, crossedPolInd);
    if (crossedPolInd < 0) {
        //Если нет пересечения.
        R = G = B = 0;
        return;
    }

    //+Вычисление непосредственной освещённости.
    //Предварительные вычисления.
    const Point crossPoint = {ray.x0 + ray.a * t,
        ray.y0 + ray.b * t, ray.z0 + ray.c * t};

    //Нормирование направляющего вектора исходного луча.
    const float directingNorm = ray.directingVectorAbs();
    if (directingNorm > 0.f) {
        ray.a /= directingNorm;
        ray.b /= directingNorm;
        ray.c /= directingNorm;
    }

    //Вычисление интенсивности.
    GetIntensivity(crossPoint.x, crossPoint.y, crossPoint.z, polygons, polCount,
        lights, lightsCount, R, G, B, crossedPolInd, ray);

    //TODO: трассировка для преломлённого луча.

    if(lookCount < MAX_LOOKED_LINES_CNT)
    {
        //+Формирование отражённого луча.
        Line reflRay =
            reflectedRay(polygons[crossedPolInd], ray, crossPoint);
        //+Его трассировка.
        Trace(reflRay, polygons, polCount, lights, lightsCount,
            Rotr, Gotr, Botr, lookCount + 1);

        //Обработка полученных интенсивностей.
        const float ks = polygons[crossedPolInd].getKs();
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
        //Если рассмотрено слишком много лучей, то формировать и
        //трассировать новые отражённые лучи не будем.
        Rotr = 0;
        Gotr = 0;
        Botr = 0;
    }

    /*  Iотр = Is * ks;
        Ih = Ial + Iпр + Iотр;
        I = Ih * exp(-b * d)
    */
    //TODO: добавить составляющие преломления.
    R += Rotr;
    G += Gotr;
    B += Botr;

    const float dist = directingNorm * t;
    const float attenuation = exp(- B_KOEFFICIENT * dist);

    R *= attenuation;
    G *= attenuation;
    B *= attenuation;
}

//Функция определения параметра и номера для пересечения луча с многоугольником.
float CrossingParameter(const Line &l, const Polygon *obj, int np, int &crossedPolInd)
{
    float tmin = std::numeric_limits<float>::max();
    crossedPolInd = - 1;
    bool isFound = false;

    for (int i=0; i<np; ++i)
    {
        float t =
            obj[i].getA() * l.a  +  obj[i].getB() * l.b  +  obj[i].getC() * l.c;
        if (t != 0)
        {
            t = - (obj[i].getA() * l.x0  +  obj[i].getB() * l.y0  +
                obj[i].getC() * l.z0  +  obj[i].getD()) / t;

            if (t > EPSILON  &&  t < tmin)  //Если найдено пересечение с несущей плоскостью, и оно ближе остальных найденных на данный момент пересечений.
                //+Уточнение, пересекает ли луч многоугольник.
                if (obj[i].pointInPolygon(l.x0 + l.a*t,
                        l.y0 + l.b*t, l.z0 + l.c*t))
                {
                    tmin = t;
                    crossedPolInd = i;
                    isFound = true;
                }
        }
    }

    return isFound ? tmin : (- 1.f);
}

Line reflectedRay(const Polygon &from, const Line &instRay,
                  const Point &reflectionPoint)
{
    Line res;

    res.x0 = reflectionPoint.x;
    res.y0 = reflectionPoint.y;
    res.z0 = reflectionPoint.z;

    float q;
    q = instRay.a * from.getA() +
        instRay.b * from.getB() + instRay.c * from.getC();
    q += q;             // q = 2*(L; n).
    res.a = instRay.a - q * from.getA();
    res.b = instRay.b - q * from.getB();
    res.c = instRay.c - q * from.getC();    // R = -L + q*n.

    return res;
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
    const Rgb diffusedRgb = obj[cross_pol_n].diffusionWeights();
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
            t = obj[j].getA() * line_to_light.a  +
                obj[j].getB() * line_to_light.b  +
                obj[j].getC() * line_to_light.c;
            if (t != 0)
            {
                t = -(obj[j].getA() * line_to_light.x0  +
                    obj[j].getB() * line_to_light.y0  +
                    obj[j].getC() * line_to_light.z0  +  obj[j].getD())/t;
                if (t > EPSILON  &&  t < 1)      // ~t>0.
                    //+Уточнение, пересекает ли луч многоугольник.
                    if (obj[j].pointInPolygon(
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
            for (++j; j<np; ++j)    //Перебор оставшихся многоугольников.
            {
                t = obj[j].getA() * line_to_light.a  +
                    obj[j].getB() * line_to_light.b  +
                    obj[j].getC() * line_to_light.c;
                if (t != 0)
                {
                    t = -(obj[j].getA() * line_to_light.x0  +
                        obj[j].getB() * line_to_light.y0  +
                        obj[j].getC() * line_to_light.z0  +  obj[j].getD())/t;
                    if (t > EPSILON  &&  t < 1)      // ~t>0.
                        //+Уточнение, пересекает ли луч многоугольник.
                        if (obj[j].pointInPolygon(line_to_light.x0 + line_to_light.a*t,  line_to_light.y0 + line_to_light.b*t,  line_to_light.z0 + line_to_light.c*t))
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
            cosA = line_to_light.directingVectorAbs();
            cosT = obj[cross_pol_n].getA() * line_to_light.a  +
                obj[cross_pol_n].getB() * line_to_light.b  +
                obj[cross_pol_n].getC() * line_to_light.c;
            cosT /= sqrt(obj[cross_pol_n].getA()*obj[cross_pol_n].getA() +
                obj[cross_pol_n].getB()*obj[cross_pol_n].getB() +
                obj[cross_pol_n].getC()*obj[cross_pol_n].getC())  *  cosA;

            //Вычисление освещённости диффузионно отражённого света от i-го источника.
            d = line_to_light.directingVectorAbs();
            t = cosT * light[i].Intensivity() / (d + K_KOEFFICIENT);

            const Rgb reflectedRgb = obj[cross_pol_n].reflectionWeights();
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
            line_to_light.a = cosT * obj[cross_pol_n].getA()  -  line_to_light.a;
            line_to_light.b = cosT * obj[cross_pol_n].getB()  -  line_to_light.b;
            line_to_light.c = cosT * obj[cross_pol_n].getC()  -  line_to_light.c;
            //Вычисление косинуса угла между отражённым лучом и исходным лучом l.
            cosA = l.a * line_to_light.a  +  l.b * line_to_light.b  +
                l.c * line_to_light.c;
            //Вычисление освещённости зеркально отражённого света.
            cosA = Raise(fabs(cosA), obj[cross_pol_n].cosPower()) *
                obj[cross_pol_n].getKs() *
                light[i].Intensivity() / (d + K_KOEFFICIENT);
            R += cosA;
            G += cosA;
            B += cosA;
        }
    }
}
