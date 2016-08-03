#ifndef SIMPLERENDERER_H
#define SIMPLERENDERER_H

#include "Renderer.h"

class SimpleRenderer: public    Renderer
{
public:
    SimpleRenderer(QObject *parent = nullptr);

protected:
    void performRendering(const Scene &scene, Canvas &canvas) override;
};

#endif // SIMPLERENDERER_H
