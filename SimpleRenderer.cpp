#include "SimpleRenderer.h"

#include "RenderUtilities.h"
#include "Scene.h"

SimpleRenderer::SimpleRenderer(QObject *parent)
    : Renderer(parent)
{
}

void SimpleRenderer::performRendering(const Scene &scene, Canvas &canvas)
{
    const auto pol = scene.polygons();
    const Polygon *polPtr = pol.data();
    const int polCount = pol.size();

    const auto lights = scene.lights();
    const SpotLight *lightsPtr = lights.data();
    const int lightsCount = lights.size();

    const int halfWidth = canvas.width() / 2;
    const int halfHeight = canvas.height() / 2;
    const float widthFloat = float(canvas.width());
    for (int x = - halfWidth; x < halfWidth; ++x) {
        for (int y = - halfHeight + 1; y < halfHeight; ++y) {
            const Rgb pixResult = TraceForPixel(x, y, polPtr, polCount,
                lightsPtr, lightsCount);
            canvas.setPixel(x + halfWidth, halfHeight - y, pixResult);
        }

        const float progress = float(x + halfWidth) / widthFloat;
        emit progressChanged(progress);
    }
}
