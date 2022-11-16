/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "window.h"
#include <layershellqt_logging.h>

#include "../qwaylandlayershellintegration_p.h"
#include "../qwaylandlayersurface_p.h"

#include <QtWaylandClient/private/qwaylandwindow_p.h>

using namespace LayerShellQt;

static QWaylandLayerShellIntegration *s_integration = nullptr;

Window::Window(QWindow *window)
    : QObject(window)
{
    window->winId();

    auto waylandWindow = dynamic_cast<QtWaylandClient::QWaylandWindow *>(window->handle());
    if (!waylandWindow) {
        return;
    }

    if (!s_integration) {
        s_integration = new QWaylandLayerShellIntegration();
    }

    if (waylandWindow->shellIntegration() != s_integration) {
        waylandWindow->setShellIntegration(s_integration);
        if (waylandWindow->shellIntegration() != s_integration) {
            return;
        }
    }

    m_layerSurface = static_cast<QWaylandLayerSurface *>(waylandWindow->shellSurface());
}

Window::~Window()
{
}

bool Window::isValid() const
{
    return m_layerSurface;
}

void Window::setAnchors(Anchors anchors)
{
    m_layerSurface->setAnchor(anchors);
}

Window::Anchors Window::anchors() const
{
    return Anchors(m_layerSurface->anchor());
}

void Window::setExclusiveZone(int32_t zone)
{
    m_layerSurface->setExclusiveZone(zone);
}

int32_t Window::exclusionZone() const
{
    return m_layerSurface->exclusionZone();
}

void Window::setMargins(const QMargins &margins)
{
    m_layerSurface->setMargins(margins);
}

QMargins Window::margins() const
{
    return m_layerSurface->margins();
}

void Window::setKeyboardInteractivity(KeyboardInteractivity interactivity)
{
    m_layerSurface->setKeyboardInteractivity(interactivity);
}

Window::KeyboardInteractivity Window::keyboardInteractivity() const
{
    return KeyboardInteractivity(m_layerSurface->keyboardInteractivity());
}

void Window::setLayer(Layer layer)
{
    m_layerSurface->setLayer(layer);
}

void Window::setScope(const QString &scope)
{
    m_layerSurface->setScope(scope);
}

QString Window::scope() const
{
    return m_layerSurface->scope();
}

Window::Layer Window::layer() const
{
    return Layer(m_layerSurface->layer());
}

QScreen *Window::desiredOutput() const
{
    return m_layerSurface->desiredOutput();
}

void Window::setDesiredOutput(QScreen *output)
{
    m_layerSurface->setDesiredOutput(output);
}
