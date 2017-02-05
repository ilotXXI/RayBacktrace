#include <math.h>
#include <limits.h>

#include "RenderUtilities.h"

#include "Point.h"
#include "Canvas.h"
#include "Polygon.h"
#include "Line.h"
#include "SpotLight.h"

#define EPSILON                     0.0005
#define VISOR_Z                     1200
#define DISPLAY_Z                   500
#define DIFFUSED_LIGHT_INTENSIVITY  150
#define K_KOEFFICIENT               0.01f
#define B_KOEFFICIENT               0.001f
#define MAX_LOOKED_RAYS_CNT         20

static void CalcIntensivity(const float &x, const float &y, const float &z,
                           const Polygon *polygons, int polygonsCount,
                           const SpotLight *lights,
                           int lightsCount, float &R, float &G, float &B,
                           int polInd, const Line &ray);
//Возвращает false, если луч не пересекает ни одного прямоугольника.
static bool ClosestCross(const Line &ray, const Polygon *polygons,
                         int polygonsCount, int &crossedPolInd,
                         float &lineCrossParameter);
static bool AnyCross(const Line &ray, const Polygon *polygons,
                     int polygonsCount, int &crossedPolInd,
                     float &lineCrossParameter);
static bool HasAnyCross(const Line &ray, const Polygon *polygons,
                        int polygonsCount);
static bool LineCrossesPolygonPlane(const Line &line, const Polygon &polygon,
                                    float &crossParameter);
static Line ReflectedRay(const Polygon &from, const Line &instRay,
                         const Point &reflectionPoint);
//Направляющий вектор line должен быть нормирован к 1.
static Point LinePoint(const Line &line, float parameter);

static float Raise(float base, int power)
{
    float y = base;
    for (int i = 1; i < power; ++i)
        y *= base;
    return y;
}

static inline void Constrain(float &color)
{
    // TODO: range must be [0, 1].
    if (color < - EPSILON || color > 255.f)
        color = 255.f;
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

    Constrain(R);
    Constrain(G);
    Constrain(B);

    return Rgb(R, G, B);
}

//Процедура трассировки одного луча.
void Trace(Line &ray, const Polygon *polygons, int polCount,
           const SpotLight *lights, int lightsCount,
           float &R, float &G, float &B, short lookCount)
{
    float reflectR = 0.f;
    float reflectG = 0.f;
    float reflectB = 0.f;
    int crossedPolInd = 0;

    float t = 0.f;
    const bool rayHasCrosses = ClosestCross(ray, polygons, polCount,
        crossedPolInd, t);
    if (!rayHasCrosses) {
        R = G = B = 0;
        return;
    }

    const Point crossPoint = {ray.x0 + ray.a * t,
        ray.y0 + ray.b * t, ray.z0 + ray.c * t};

    const float directingNorm = ray.directingVectorAbs();
    if (directingNorm > 0.f) {
        ray.a /= directingNorm;
        ray.b /= directingNorm;
        ray.c /= directingNorm;
    }

    CalcIntensivity(crossPoint.x, crossPoint.y, crossPoint.z, polygons, polCount,
        lights, lightsCount, R, G, B, crossedPolInd, ray);

    //TODO: трассировка для преломлённого луча.

    if(lookCount < MAX_LOOKED_RAYS_CNT) {
        Line reflRay =
            ReflectedRay(polygons[crossedPolInd], ray, crossPoint);
        Trace(reflRay, polygons, polCount, lights, lightsCount,
            reflectR, reflectG, reflectB, lookCount + 1);

        const float ks = polygons[crossedPolInd].reflectionCoefficient();
        reflectR *= ks;
        reflectG *= ks;
        reflectB *= ks;
        Constrain(reflectR);
        Constrain(reflectG);
        Constrain(reflectB);
    } else {
        //Достигнута предельная глубина.
        reflectR = 0;
        reflectG = 0;
        reflectB = 0;
    }

    /*  Iотр = Is * ks;
        Ih = Ial + Iпр + Iотр;
        I = Ih * exp(-b * d)
    */
    //TODO: добавить составляющие преломления.
    R += reflectR;
    G += reflectG;
    B += reflectB;

    const float dist = directingNorm * t;
    const float attenuation = exp(- B_KOEFFICIENT * dist);

    R *= attenuation;
    G *= attenuation;
    B *= attenuation;
}

bool ClosestCross(const Line &ray, const Polygon *polygons,
                  int polygonsCount, int &crossedPolInd,
                  float &lineCrossParameter)
{
    lineCrossParameter = 0.f;
    crossedPolInd = - 1;
    float tmin = std::numeric_limits<float>::max();
    bool isFound = false;

    for (int i = 0; i < polygonsCount; ++i) {
        const Polygon &pol = polygons[i];

        float crossParam = 0.f;
        const bool planeIsCrossed = LineCrossesPolygonPlane(ray, pol, crossParam);

        if (!planeIsCrossed || crossParam < EPSILON || crossParam > tmin) {
            //Нет пересечения в прямом направлении, либо уже найдены
            //более близкие многоугольники.
            continue;
        }

        const Point crossPoint = LinePoint(ray, crossParam);
        if (pol.pointIsInPolygon(crossPoint)) {
            tmin = crossParam;
            crossedPolInd = i;
            isFound = true;
        }
    }

    lineCrossParameter = tmin;
    return isFound;
}

bool AnyCross(const Line &ray, const Polygon *polygons,
              int polygonsCount, int &crossedPolInd,
              float &lineCrossParameter)
{
    lineCrossParameter = 0.f;
    crossedPolInd = - 1;
    bool isFound = false;

    for (int i = 0; i < polygonsCount; ++i) {
        const Polygon &pol = polygons[i];

        float crossParam = 0.f;
        const bool planeIsCrossed = LineCrossesPolygonPlane(ray, pol, crossParam);

        if (!planeIsCrossed || crossParam < EPSILON) {
            //Нет пересечения в прямом направлении, либо уже найдены
            //более близкие многоугольники.
            continue;
        }

        const Point crossPoint = LinePoint(ray, crossParam);
        if (pol.pointIsInPolygon(crossPoint)) {
            lineCrossParameter = crossParam;
            crossedPolInd = i;
            isFound = true;
        }
    }

    return isFound;
}

inline bool HasAnyCross(const Line &ray, const Polygon *polygons,
                        int polygonsCount)
{
    int ind = 0;
    float param = 0.f;
    return AnyCross(ray, polygons, polygonsCount, ind, param);
}

inline bool LineCrossesPolygonPlane(const Line &line, const Polygon &polygon,
                                    float &crossParameter)
{
    const float normalToRay =
        polygon.getA() * line.a +
        polygon.getB() * line.b +
        polygon.getC() * line.c;

    if (abs(normalToRay) < 1E-4)
        return false;

    crossParameter = - (polygon.getA() * line.x0 +
        polygon.getB() * line.y0 + polygon.getC() * line.z0 +
        polygon.getD()) / normalToRay;
    return true;
}

inline Line ReflectedRay(const Polygon &from, const Line &instRay,
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

inline Point LinePoint(const Line &line, float parameter)
{
    return Point{line.x0 + line.a * parameter,
        line.y0 + line.b * parameter,
        line.z0 + line.c * parameter};
}

void CalcIntensivity(const float &x, const float &y, const float &z,
                     const Polygon *polygons, int polygonsCount,
                     const SpotLight *lights, int lightsCount,
                     float &R, float &G, float &B,
                     int polInd, const Line &ray)
{
    const Polygon &pol = polygons[polInd];

    //Вычисление рассеянной составляющей.
    const Rgb diffusedRgb = pol.diffusionWeights();
    R = DIFFUSED_LIGHT_INTENSIVITY * diffusedRgb.red();
    G = DIFFUSED_LIGHT_INTENSIVITY * diffusedRgb.green();
    B = DIFFUSED_LIGHT_INTENSIVITY * diffusedRgb.blue();

    //Вычисления составлющих от диффузионного рассеивания и зеркального отражения.
    Line rayToLight;
    rayToLight.x0 = x;
    rayToLight.y0 = y;
    rayToLight.z0 = z;

    const Polygon *firstPolBegin = polygons;
    const int firstPolCount = polInd;
    const Polygon *secondPolBegin = polygons + polInd + 1;
    const int secondPolCount = polygonsCount - polInd - 1;

    for (int i = 0; i < lightsCount; ++i) {
        //формирование луча от рассматриваемой точки до источника света.
        rayToLight.a = lights[i].x() - x;
        rayToLight.b = lights[i].y() - y;
        rayToLight.c = lights[i].z() - z;

        //Поиск пересечения луча с другими объектами сцены.
        const bool lightIsScreened =
            HasAnyCross(rayToLight, firstPolBegin, firstPolCount) ||
            HasAnyCross(rayToLight, secondPolBegin, secondPolCount);
        if (lightIsScreened)
            continue;

        //Вычисление косинуса угла между нормалью к многоугольнику и лучём.
        const float rayToLAbs = rayToLight.directingVectorAbs();
        float cosT = pol.getA() * rayToLight.a + pol.getB() * rayToLight.b +
            pol.getC() * rayToLight.c;
        //Нормаль к поверхности всегда нормированна к 1.
        cosT /= 1.f * rayToLAbs;

        //Вычисление освещённости диффузионно отражённого света от i-го источника.
        const float distToLight = rayToLAbs;
        const float t = cosT * lights[i].Intensivity() /
            (distToLight + K_KOEFFICIENT);

        const Rgb reflectedRgb = pol.reflectionWeights();
        R += reflectedRgb.red() * t;
        G += reflectedRgb.green() * t;
        B += reflectedRgb.blue() * t;

        //Вычисление освещённости зеркально отражённого света от i-го источника.
        rayToLight.a /= rayToLAbs;
        rayToLight.b /= rayToLAbs;
        rayToLight.c /= rayToLAbs;

        cosT += cosT;
        Line reflectedRay = rayToLight;
        reflectedRay.a = cosT * pol.getA() - rayToLight.a;
        reflectedRay.b = cosT * pol.getB() - rayToLight.b;
        reflectedRay.c = cosT * pol.getC() - rayToLight.c;

        //Вычисление косинуса угла между отражённым лучом и исходным лучом.
        float cosA = ray.a * reflectedRay.a  +  ray.b * reflectedRay.b  +
            ray.c * reflectedRay.c;
        //Вычисление освещённости зеркально отражённого света.
        cosA = Raise(fabs(cosA), pol.cosPower()) * pol.reflectionCoefficient() *
            lights[i].Intensivity() / (distToLight + K_KOEFFICIENT);
        R += cosA;
        G += cosA;
        B += cosA;
    }
}
