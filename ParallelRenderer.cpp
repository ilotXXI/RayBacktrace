#include "ParallelRenderer.h"

#include <QAtomicInt>

#include <omp.h>

#include "RenderUtilities.h"

ParallelRenderer::ParallelRenderer(QObject *parent)
    : Renderer(parent)
{
}

void ParallelRenderer::performRendering(const Scene &scene, Canvas &canvas)
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
    QAtomicInt counter(0);

    #pragma omp parallel
    {
        const bool isEmitterThread = (omp_get_thread_num() == 0);

        #pragma omp for nowait
        for (int x = - halfWidth; x < halfWidth; ++x) {
            for (int y = - halfHeight + 1; y < halfHeight; ++y) {
                const Rgb pixResult = TraceForPixel(x, y, polPtr, polCount,
                    lightsPtr, lightsCount);
                canvas.setPixel(x + halfWidth, halfHeight - y, pixResult);
            }

            if (isEmitterThread) {
                const int counterVal = counter++;
                const float progress = float(counterVal) / widthFloat;
                emit progressChanged(progress);
            } else {
                ++counter;
            }
        }
    }
}
