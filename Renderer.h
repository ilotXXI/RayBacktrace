#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>
#include <QSize>

#include "Canvas.h"
#include "Scene.h"


class Renderer: public  QObject
{
    Q_OBJECT

public:
    explicit Renderer(QObject *parent = nullptr);
    virtual ~Renderer() = default;

    void render(const Scene &scene, const QSize &size);
    void renderAsync(const Scene &scene, const QSize &size);
    Canvas result() const;

signals:
    void renderStarted();
    void renderFinished();

    void progressChanged(float weight);

protected:
    virtual void performRendering(const Scene &scene, Canvas &canvas) = 0;

private:
    Canvas  _canvas;

private slots:
    void callRender(Scene scene, QSize size);
};

inline Canvas Renderer::result() const
{
    return _canvas;
}

#endif // RENDERER_H
