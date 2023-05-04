// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <QObject>
#include <QPointer>

#include <QColor>

#include <QSystemTrayIcon>
#include <QMenu>

namespace pawxel {

class SysTray : public QSystemTrayIcon {
    Q_OBJECT
public:
    explicit SysTray(QColor _accentColor, bool _isDarkMode, QObject *parent = nullptr);
    ~SysTray();

protected:
    QPointer<QMenu> m_contextMenu;

private:
    QAction *m_fullscreenCaptureAction;
    QAction *m_snipCaptureAction;
    QAction *m_restoreCaptureAction;
    QAction *m_prefsAction;
    QAction *m_aboutAction;
    QAction *m_quitAction;

    QColor m_accentColor;
    bool m_isDarkMode;

signals:
    // User wants to create a fullscreen screenshot.
    void fullScreenCaptureRequested();
    // User wants to snip a screenshot.
    void snipAreaRequested();
    // User wants to restore the last capture.
    void restoreCaptureRequested();
    // User wants to access the preferences.
    void preferencesWindowRequested();
    // User wants to access the about dialog.
    void aboutDialogRequested();
    // User wants to quit the application.
    void quitRequested(bool _finalQuit);
};

} // namespace