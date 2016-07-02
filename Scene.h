#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Polygon.h"
#include "SpotLight.h"

class Scene
{
public:
    Scene();

    const std::vector<Polygon> & polygons() const;
    void setPolygons(const std::vector<Polygon> &polygons);
    void addPolygon(const Polygon &polygon);
    void addPolygon(Polygon &&polygon);

    const std::vector<SpotLight> & lights() const;
    void setLights(const std::vector<SpotLight> &lights);
    void addLight(const SpotLight &light);
    void addLight(SpotLight &&light);

    void clear();

private:
    std::vector<Polygon>    _polygons;
    std::vector<SpotLight>  _lights;
};


inline const std::vector<Polygon> & Scene::polygons() const
{
    return _polygons;
}

inline void Scene::addPolygon(const Polygon &polygon)
{
    _polygons.push_back(polygon);
}

inline void Scene::addPolygon(Polygon &&polygon)
{
    _polygons.emplace_back(std::move(polygon));
}

inline const std::vector<SpotLight> & Scene::lights() const
{
    return _lights;
}

inline void Scene::addLight(const SpotLight &light)
{
    _lights.push_back(light);
}

inline void Scene::addLight(SpotLight &&light)
{
    _lights.emplace_back(std::move(light));
}

#endif // SCENE_H
