#include "Scene.h"

Scene::Scene()
{
}

Scene::Scene(const std::vector<Polygon> &polygons,
             const std::vector<SpotLight> &lights)
    : _polygons(polygons)
    , _lights(lights)
{
}

void Scene::setPolygons(const std::vector<Polygon> &polygons)
{
    _polygons = polygons;
}

void Scene::setLights(const std::vector<SpotLight> &lights)
{
    _lights = lights;
}

void Scene::clear()
{
    _polygons.clear();
    _lights.clear();
}
