/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "qwaylandlayershell_p.h"
#include "qwaylandlayershelldata_p.h"
#include "qwaylandlayershellintegration_p.h"
#include "qwaylandlayersurface_p.h"

#include <QtWaylandClient/private/qwaylanddisplay_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>
#include <qwayland-wlr-layer-shell-unstable-v1.h>

namespace LayerShellQt
{
QWaylandLayerShellIntegration::QWaylandLayerShellIntegration()
    : m_layerShell(new QWaylandLayerShell())
{
}

QWaylandLayerShellIntegration::~QWaylandLayerShellIntegration()
{
}

bool QWaylandLayerShellIntegration::initialize(QtWaylandClient::QWaylandDisplay *display)
{
    return true;
}

QtWaylandClient::QWaylandShellSurface *QWaylandLayerShellIntegration::createShellSurface(QtWaylandClient::QWaylandWindow *window)
{
    return new QWaylandLayerSurface(m_layerShell.data(), window);
}

QtWaylandClient::QWaylandShellData *QWaylandLayerShellIntegration::createShellData(QtWaylandClient::QWaylandWindow *window)
{
    return new QWaylandLayerShellData(window);
}

}

//#include "qwaylandlayershellintegration.moc"
