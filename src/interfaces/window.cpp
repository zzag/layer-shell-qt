/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "window.h"
#include <layershellqt_logging.h>

#include "../qwaylandlayershelldata_p.h"
#include "../qwaylandlayershellintegration_p.h"
#include "../qwaylandlayersurface_p.h"

#include <QtWaylandClient/private/qwaylandwindow_p.h>

using namespace LayerShellQt;

static QWaylandLayerShellIntegration *s_integration = nullptr;

Window::Window(QWaylandLayerShellData *shellData)
    : m_shellData(shellData)
{
}

Window::~Window()
{
}

Window *Window::get(QWindow *window)
{
    window->winId();
    auto waylandWindow = dynamic_cast<QtWaylandClient::QWaylandWindow *>(window->handle());
    if (!waylandWindow) {
        return nullptr;
    }

    if (!s_integration) {
        s_integration = new QWaylandLayerShellIntegration();
    }

    if (waylandWindow->shellIntegration() != s_integration) {
        waylandWindow->setShellIntegration(s_integration);
        if (waylandWindow->shellIntegration() != s_integration) {
            return nullptr;
        }
    }

    QWaylandLayerShellData *shellData = static_cast<QWaylandLayerShellData *>(waylandWindow->shellData());
    if (!shellData->interface) {
        shellData->interface.reset(new Window(shellData));
    }

    return shellData->interface.get();
}

void Window::setAnchors(Anchors anchors)
{
    m_shellData->anchors = anchors;
    if (auto shellSurface = static_cast<QWaylandLayerSurface *>(m_shellData->window->shellSurface())) {
        shellSurface->setAnchor(anchors);
    }
    Q_EMIT anchorsChanged();
}

Window::Anchors Window::anchors() const
{
    return m_shellData->anchors;
}

void Window::setExclusiveZone(int32_t zone)
{
    m_shellData->exclusionZone = zone;
    if (auto shellSurface = static_cast<QWaylandLayerSurface *>(m_shellData->window->shellSurface())) {
        shellSurface->setExclusiveZone(zone);
    }
    Q_EMIT exclusionZoneChanged();
}

int32_t Window::exclusionZone() const
{
    return m_shellData->exclusionZone;
}

void Window::setMargins(const QMargins &margins)
{
    m_shellData->margins = margins;
    if (auto shellSurface = static_cast<QWaylandLayerSurface *>(m_shellData->window->shellSurface())) {
        shellSurface->setMargins(margins);
    }
    Q_EMIT marginsChanged();
}

QMargins Window::margins() const
{
    return m_shellData->margins;
}

void Window::setKeyboardInteractivity(KeyboardInteractivity interactivity)
{
    m_shellData->keyboardInteractivity = interactivity;
    if (auto shellSurface = static_cast<QWaylandLayerSurface *>(m_shellData->window->shellSurface())) {
        shellSurface->setKeyboardInteractivity(interactivity);
    }
    Q_EMIT keyboardInteractivityChanged();
}

Window::KeyboardInteractivity Window::keyboardInteractivity() const
{
    return m_shellData->keyboardInteractivity;
}

void Window::setLayer(Layer layer)
{
    m_shellData->layer = layer;
}

void Window::setScope(const QString &scope)
{
    m_shellData->scope = scope;
    // this is static and must be set before the platform window is created
}

QString Window::scope() const
{
    return m_shellData->scope;
}

Window::Layer Window::layer() const
{
    return m_shellData->layer;
}

QScreen *Window::desiredOutput() const
{
    return m_shellData->desiredOutput;
}

void Window::setDesiredOutput(QScreen *output)
{
    m_shellData->desiredOutput = output;
}
