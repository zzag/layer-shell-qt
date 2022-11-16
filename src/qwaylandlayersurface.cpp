/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "qwaylandlayershell_p.h"
#include "qwaylandlayersurface_p.h"

#include "layershellqt_logging.h"

#include <QtWaylandClient/private/qwaylandscreen_p.h>
#include <QtWaylandClient/private/qwaylandsurface_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

namespace LayerShellQt
{
QWaylandLayerSurface::QWaylandLayerSurface(QWaylandLayerShell *shell, QtWaylandClient::QWaylandWindow *window)
    : QtWaylandClient::QWaylandShellSurface(window)
    , m_shell(shell)
{
}

QWaylandLayerSurface::~QWaylandLayerSurface()
{
    destroy();
}

bool QWaylandLayerSurface::isCreated() const
{
    return QtWayland::zwlr_layer_surface_v1::isInitialized();
}

bool QWaylandLayerSurface::create()
{
    wl_output *output = nullptr;
    if (m_desiredOutput) {
        auto waylandScreen = dynamic_cast<QtWaylandClient::QWaylandScreen *>(m_desiredOutput->handle());
        // Qt will always assign a screen to a window, but if the compositor has no screens available a dummy QScreen object is created
        // this will not cast to a QWaylandScreen
        if (!waylandScreen) {
            qCWarning(LAYERSHELLQT) << "Creating a layer shell for placeholder screen. This will be positioned incorrectly";
        } else {
            output = waylandScreen->output();
        }
    }

    QtWayland::zwlr_layer_surface_v1::init(m_shell->get_layer_surface(window()->wlSurface(), output, m_layer, m_scope));

    set_anchor(m_anchors);
    set_exclusive_zone(m_exclusionZone);
    set_margin(m_margins.top(), m_margins.right(), m_margins.bottom(), m_margins.left());
    set_keyboard_interactivity(m_keyboardInteractivity);

    QSize size = window()->surfaceSize();
    if ((m_anchors & anchor_left) && (m_anchors & anchor_right)) {
        size.setWidth(0);
    }
    if ((m_anchors & anchor_top) && (m_anchors & anchor_bottom)) {
        size.setHeight(0);
    }
    if (size.isValid() && size != QSize(0, 0)) {
        set_size(size.width(), size.height());
    }

    return true;
}

void QWaylandLayerSurface::destroy()
{
    m_configured = false;

    if (QtWayland::zwlr_layer_surface_v1::object()) {
        QtWayland::zwlr_layer_surface_v1::destroy();
    }
}

void QWaylandLayerSurface::zwlr_layer_surface_v1_closed()
{
    window()->window()->close();
}

void QWaylandLayerSurface::zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height)
{
    ack_configure(serial);
    m_pendingSize = QSize(width, height);

    if (!m_configured) {
        m_configured = true;
        window()->resizeFromApplyConfigure(m_pendingSize);
        window()->handleExpose(QRect(QPoint(), m_pendingSize));
    } else {
        // Later configures are resizes, so we have to queue them up for a time when we
        // are not painting to the window.
        window()->applyConfigureWhenPossible();
    }
}

void QWaylandLayerSurface::applyConfigure()
{
    window()->resizeFromApplyConfigure(m_pendingSize);
}

void QWaylandLayerSurface::setScope(const QString &scope)
{
    m_scope = scope;
}

void QWaylandLayerSurface::setDesiredOutput(QScreen *screen)
{
    m_desiredOutput = screen;
}

void QWaylandLayerSurface::setAnchor(uint anchor)
{
    m_anchors = anchor;
    if (object()) {
        set_anchor(anchor);
    }
}

void QWaylandLayerSurface::setExclusiveZone(int32_t zone)
{
    m_exclusionZone = zone;
    if (object()) {
        set_exclusive_zone(zone);
    }
}

void QWaylandLayerSurface::setMargins(const QMargins &margins)
{
    m_margins = margins;
    if (object()) {
        set_margin(margins.top(), margins.right(), margins.bottom(), margins.left());
    }
}

void QWaylandLayerSurface::setKeyboardInteractivity(uint32_t interactivity)
{
    m_keyboardInteractivity = interactivity;
    if (m_keyboardInteractivity) {
        set_keyboard_interactivity(interactivity);
    }
}

void QWaylandLayerSurface::setLayer(uint32_t layer)
{
    m_layer = layer;
    if (object()) {
        if (zwlr_layer_surface_v1_get_version(object()) >= ZWLR_LAYER_SURFACE_V1_SET_LAYER_SINCE_VERSION) {
            set_layer(layer);
        }
    }
}

}
