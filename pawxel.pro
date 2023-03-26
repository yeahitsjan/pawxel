## Main project file
QT += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = pawxel
TEMPLATE = app
CONFIG += c++17 # needed for FramelessHelper & QHotkey
RC_FILE = $$PWD/winres.rc

RC_ICONS = $$PWD/resources/icons/icon.ico

SOURCES += source/application.cpp \
            source/main.cpp \
            source/sniparea.cpp \
            source/preview_window.cpp \
            source/editor_view.cpp \
            source/items.cpp \
            source/editor_window.cpp \
            source/preferences.cpp \
            source/preferences_window.cpp \
            source/systray.cpp \
            source/about_window.cpp \
            source/qt_flowlayout.cpp \
            source/emoji_widget.cpp \
            source/select_window.cpp

HEADERS += source/platform.h \
            source/application.h \
            source/sniparea.h \
            source/preview_window.h \
            source/editor_view.h \
            source/items.h \
            source/editor_window.h \
            source/preferences.h \
            source/preferences_window.h \
            source/systray.h \
            source/about_window.h \
            source/qt_flowlayout.h \
            source/emoji_widget.h \
            source/select_window.h

## Submodules
# Easylogging++
include(modules/easyloggingpp/easyloggingpp.pri)
# FramelessHelper
include(modules/framelesshelper/qmake/core.pri)
include(modules/framelesshelper/qmake/widgets.pri)
# phantomstyle
include(modules/phantomstyle/src/phantom/phantom.pri)
# SingleApplication
include(modules/SingleApplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication
# QHotkey
include(modules/QHotkey/qhotkey.pri)
# resizer-item
include(modules/resizer-item/resizer/resizer-item.pri)
# FHNotifier
include(modules/FHNotifier/FHNotifier.pri)

RESOURCES += \
    resources.qrc