#include "Renderer.h"

Renderer::Renderer(QObject *parent)
    : QObject(parent)
    , _canvas(1, 1)
{
    qRegisterMetaType<Scene>("Scene");
//    qRegisterMetaType<QSize>("QSize");
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

void Renderer::renderAsync(const Scene &scene, const QSize &size)
{
    QMetaObject::invokeMethod(this, "callRender", Qt::QueuedConnection,
        Q_ARG(Scene, scene), Q_ARG(QSize, size));
}

void Renderer::callRender(Scene scene, QSize size)
{
    render(scene, size);
}
