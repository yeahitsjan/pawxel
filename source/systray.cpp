// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "platform.h"
#include "application.h"
#include "preferences.h"
#include "systray.h"

#include "phantomstyle.h"
#include <QStyle>
#include <QStyleFactory>

namespace pawxel {

SysTray::SysTray(QColor _accentColor, bool _isDarkMode, QObject *parent) : m_accentColor(_accentColor), m_isDarkMode(_isDarkMode), QSystemTrayIcon(parent) {
    if (!m_contextMenu)
        m_contextMenu = new QMenu;
    m_contextMenu->setPalette(PWX_appPalette(m_accentColor, m_isDarkMode));
    m_contextMenu->setFont(QFont("Josefin Sans SemiBold", 9));
    {
        m_fullscreenCaptureAction = new QAction(tr("Fullscreen"), m_contextMenu);
        connect(m_fullscreenCaptureAction, &QAction::triggered, this, [=]() {
            m_contextMenu->close(); // force close context menu to not display it in screenshot
            emit fullScreenCaptureRequested();
        });
        m_snipCaptureAction = new QAction(tr("Snip"), m_contextMenu);
        connect(m_snipCaptureAction, &QAction::triggered, this, [=]() {
            emit snipAreaRequested(); // todo: delay?
        });

        m_restoreCaptureAction = new QAction(tr("Restore last capture"));
        m_restoreCaptureAction->setEnabled(false); // default, wait for first capture
        connect(m_restoreCaptureAction, &QAction::triggered, this, [=]() {
            emit restoreCaptureRequested();
        });

        m_prefsAction = new QAction(tr("Preferences"), m_contextMenu);
        connect(m_prefsAction, &QAction::triggered, this, [=]() {
            emit preferencesWindowRequested();
        });
        m_aboutAction = new QAction(tr("About Pawxel"), m_contextMenu);
        connect(m_aboutAction, &QAction::triggered, this, [=]() {
            emit aboutDialogRequested();
        });
        m_quitAction = new QAction(tr("Quit"), m_contextMenu);
        connect(m_quitAction, &QAction::triggered, this, [=]() {
            emit quitRequested(true);
        });

        m_contextMenu->addAction(m_fullscreenCaptureAction);
        m_contextMenu->addAction(m_snipCaptureAction);
        m_contextMenu->addSeparator();
        m_contextMenu->addAction(m_restoreCaptureAction);
        m_contextMenu->addSeparator();
        m_contextMenu->addAction(m_prefsAction);
        m_contextMenu->addAction(m_aboutAction);
        m_contextMenu->addSeparator();
        m_contextMenu->addAction(m_quitAction);
    }

    this->setContextMenu(m_contextMenu);
    this->setToolTip("Pawxel");
    this->setIcon(QIcon(":/resources/icons/icon_" + PwxApp->preferences()->appIcon() + ".svg"));
}

SysTray::~SysTray() {
}

void SysTray::onCaptureFinished() {
    // Enable action after we are sure the user did a screenshot.
    m_restoreCaptureAction->setEnabled(true);
}

} // namespace