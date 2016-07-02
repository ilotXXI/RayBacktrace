#include "Scene.h"

Scene::Scene()
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
