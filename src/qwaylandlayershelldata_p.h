/*
 *   SPDX-FileCopyrightText: 2022 Vlad Zahorodnii <vlad.zahorodnii@kde.org>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef _LAYERSHELLDATA_P_H
#define _LAYERSHELLDATA_P_H

#include "interfaces/window.h"

#include <QPointer>
#include <QtWaylandClient/private/qwaylandshelldata_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

class QWaylandLayerShellData : public QtWaylandClient::QWaylandShellData
{
public:
    explicit QWaylandLayerShellData(QtWaylandClient::QWaylandWindow *window)
        : window(window)
    {
    }

    QtWaylandClient::QWaylandWindow *window;
    QScopedPointer<LayerShellQt::Window> interface;
    QString scope = QStringLiteral("window");
    LayerShellQt::Window::Anchors anchors = {LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorBottom | LayerShellQt::Window::AnchorLeft | LayerShellQt::Window::AnchorRight};
    int32_t exclusionZone = 0;
    LayerShellQt::Window::KeyboardInteractivity keyboardInteractivity = LayerShellQt::Window::KeyboardInteractivityExclusive;
    LayerShellQt::Window::Layer layer = LayerShellQt::Window::LayerTop;
    QMargins margins;
    QPointer<QScreen> desiredOutput;
};

#endif // _LAYERSHELLDATA_P_H