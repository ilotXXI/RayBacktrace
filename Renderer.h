#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>

#include "Canvas.h"

class QSize;
class Scene;


class Renderer: public  QObject
{
    Q_OBJECT

public:
    explicit Renderer(QObject *parent = nullptr);
    virtual ~Renderer() = default;

    void render(const Scene &scene, const QSize &size);
    Canvas result() const;

signals:
    void renderStarted();
    void renderFinished();

protected:
    virtual void performRendering(const Scene &scene, Canvas &canvas) = 0;

private:
    Canvas  _canvas;
};

inline Canvas Renderer::result() const
{
    return _canvas;
}

#endif // RENDERER_H
