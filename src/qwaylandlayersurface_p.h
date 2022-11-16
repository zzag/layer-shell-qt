/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef _LAYERSURFACE_H
#define _LAYERSURFACE_H

#include <wayland-client.h>

#include "layershellqt_export.h"
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>
#include <qwayland-wlr-layer-shell-unstable-v1.h>

#include <QPointer>
#include <QScreen>

namespace LayerShellQt
{
class QWaylandLayerShell;

class LAYERSHELLQT_EXPORT QWaylandLayerSurface : public QtWaylandClient::QWaylandShellSurface, public QtWayland::zwlr_layer_surface_v1
{
    Q_OBJECT
public:
    QWaylandLayerSurface(QWaylandLayerShell *shell, QtWaylandClient::QWaylandWindow *window);
    ~QWaylandLayerSurface() override;

    bool isExposed() const override
    {
        return m_configured;
    }

    bool isCreated() const override;
    bool create() override;
    void destroy() override;

    QString scope() const { return m_scope; }
    void setScope(const QString &scope);

    QScreen *desiredOutput() const { return m_desiredOutput; }
    void setDesiredOutput(QScreen *screen);

    uint32_t anchor() const { return m_anchors; }
    void setAnchor(uint32_t anchor);

    int32_t exclusionZone() const { return m_exclusionZone; }
    void setExclusiveZone(int32_t zone);

    QMargins margins() const { return m_margins; }
    void setMargins(const QMargins &margins);

    uint32_t keyboardInteractivity() const { return m_keyboardInteractivity; }
    void setKeyboardInteractivity(uint32_t interactivity);

    uint32_t layer() const { return m_layer; }
    void setLayer(uint32_t layer);

    void applyConfigure() override;

private:
    void zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height) override;
    void zwlr_layer_surface_v1_closed() override;

    QWaylandLayerShell *m_shell;
    QSize m_pendingSize;
    QString m_scope;
    QPointer<QScreen> m_desiredOutput;
    uint32_t m_anchors = anchor_top | anchor_right | anchor_left | anchor_bottom;
    int32_t m_exclusionZone = 0;
    uint32_t m_keyboardInteractivity = 0;
    uint32_t m_layer = 0;
    QMargins m_margins;
    bool m_configured = false;
};

}

#endif
