#include "SimpleRenderer.h"

#include "RenderUtilities.h"
#include "Scene.h"

SimpleRenderer::SimpleRenderer(QObject *parent)
    : Renderer(parent)
{
}

void SimpleRenderer::performRendering(const Scene &scene, Canvas &canvas)
{
    Draw(canvas, scene.polygons().data(), scene.polygonsCount(),
        scene.lights().data(), scene.lightsCount());
}
