#ifndef PARALLELRENDERER_H
#define PARALLELRENDERER_H

#include "Renderer.h"

class ParallelRenderer : public Renderer
{
    Q_OBJECT
public:
    ParallelRenderer(QObject *parent = nullptr);

protected:
    void performRendering(const Scene &scene, Canvas &canvas) override;
};

#endif // PARALLELRENDERER_H
