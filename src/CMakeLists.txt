# SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

remove_definitions(-DQT_NO_SIGNALS_SLOTS_KEYWORDS)

add_library(LayerShellQtInterface)
if (QT_MAJOR_VERSION EQUAL "5")
    ecm_add_qtwayland_client_protocol(LAYER_SHELL_SOURCES PROTOCOL ${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml BASENAME xdg-shell)
    ecm_add_qtwayland_client_protocol(LAYER_SHELL_SOURCES PROTOCOL wlr-layer-shell-unstable-v1.xml BASENAME wlr-layer-shell-unstable-v1)
else()
    qt6_generate_wayland_protocol_client_sources(LayerShellQtInterface FILES
        ${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/wlr-layer-shell-unstable-v1.xml
    )
endif()

ecm_qt_declare_logging_category(LAYER_SHELL_SOURCES
    HEADER
        layershellqt_logging.h
    IDENTIFIER
        LAYERSHELLQT
    CATEGORY_NAME
        layershellqt
)

target_sources(LayerShellQtInterface PRIVATE qwaylandlayersurface.cpp interfaces/window.cpp qwaylandlayershellintegration.cpp qwaylandlayershell.cpp ${LAYER_SHELL_SOURCES})
target_link_libraries(LayerShellQtInterface PRIVATE Qt::Gui Qt::WaylandClientPrivate Wayland::Client PkgConfig::XKBCOMMON)
if (TARGET Qt::XkbCommonSupportPrivate)
    target_link_libraries(LayerShellQtInterface PRIVATE Qt::XkbCommonSupportPrivate)
endif()
target_include_directories(LayerShellQtInterface PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/LayerShellQt>"
                                              INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/>"
)

set_target_properties(LayerShellQtInterface PROPERTIES VERSION   ${LAYERSHELLQT_VERSION}
                                                       SOVERSION ${LAYERSHELLQT_SOVERSION}
                                                       EXPORT_NAME Interface
)

ecm_generate_headers(LayerShellQt_HEADERS
    HEADER_NAMES
        Window
    RELATIVE interfaces/
    REQUIRED_HEADERS LayerShellQt_HEADERS
)


generate_export_header(LayerShellQtInterface
    BASE_NAME LayerShellQtInterface
    EXPORT_MACRO_NAME LAYERSHELLQT_EXPORT
    EXPORT_FILE_NAME LayerShellQt/layershellqt_export.h
)

install(TARGETS LayerShellQtInterface EXPORT LayerShellQtTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${LayerShellQt_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/LayerShellQt/layershellqt_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/LayerShellQt COMPONENT Devel
)
