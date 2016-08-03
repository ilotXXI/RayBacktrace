#include "Renderer.h"

#include <QSize>

#include "Canvas.h"

Renderer::Renderer(QObject *parent)
    : QObject(parent)
    , _canvas(1, 1)
{
}

void Renderer::render(const Scene &scene, const QSize &size)
{
    if (!size.isValid())
        return;
    if (size.width() != _canvas.width() || size.height() != _canvas.height()) {
        _canvas = Canvas(size.width(), size.height());
    } else {
        _canvas.clear();
    }

    emit renderStarted();

    performRendering(scene, _canvas);

    emit renderFinished();
}
