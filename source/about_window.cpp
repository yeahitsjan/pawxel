// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "platform.h"
#include "application.h"
#include "about_window.h"

#include <QFile>
#include <QTextStream>

#include <QStyle>
#include <QDesktopWidget>

namespace pawxel {

AboutWindow::AboutWindow(QColor _accentColor, bool _isDarkMode, QWidget *parent) : m_accentColor(_accentColor), m_isDarkMode(_isDarkMode), FramelessDialog(parent) {
    this->setWindowTitle(tr("About Pawxel"));
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFont(QFont(APP_FONT, APP_FONT_SIZE));
#if defined(Q_OS_LINUX)
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setFixedSize(QSize(330, 440));
#else
    this->setFixedSize(QSize(330, 490));
#endif
    // Move window to center of screen.
    QRect desktopRect = PwxApp->desktop()->availableGeometry(this);
    QPoint center = desktopRect.center();
    this->move(center.x() - width() * 0.5, center.y() - height() * 0.5);

    if (!m_windowLayout)
        m_windowLayout = new QVBoxLayout(this);
    m_windowLayout->setSpacing(4);
    m_windowLayout->setMargin(12);
    m_windowLayout->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    m_iconBtn = new QPushButton(this);
    m_iconBtn->setIcon(QIcon(":/resources/icons/icon_" + PwxApp->preferences()->appIcon() + ".svg"));
    m_iconBtn->setIconSize(QSize(52, 52));
    m_iconBtn->setObjectName("ActionButtons");
    m_iconBtn->setFixedSize(QSize(72, 72));
    m_windowLayout->addWidget(m_iconBtn);

    m_versionLbl = new QLabel(this);
    m_versionLbl->setObjectName("VersionLabel");
    m_versionLbl->setText(tr("Version ") + PAWXEL_VERSION);
    m_windowLayout->addWidget(m_versionLbl);

    // Read license browser text.
    QFile _lFile(":/resources/texts/licensebrowser.txt");
    QString _cnt;
    if (_lFile.open(QIODevice::ReadOnly)) {
        QTextStream rd(&_lFile);
        _cnt = rd.readAll();
    }
    m_licenseBrowser = new QTextBrowser(this);
    m_licenseBrowser->setHtml(_cnt);
    m_licenseBrowser->setOpenExternalLinks(true);
    m_licenseBrowser->setObjectName("LicenseTextBrowser");
    m_licenseBrowser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_windowLayout->addWidget(m_licenseBrowser);

    QPushButton *_closeBtn = new QPushButton(tr("Close"), this);
    connect(_closeBtn, &QPushButton::clicked, this, [=]() {
        this->close();
    });
    _closeBtn->setFixedWidth(62);
    _closeBtn->setObjectName("ActionButtons");
    m_windowLayout->addWidget(_closeBtn);

    m_windowLayout->setAlignment(m_iconBtn, Qt::AlignHCenter);
    m_windowLayout->setAlignment(m_versionLbl, Qt::AlignHCenter);
    m_windowLayout->setAlignment(_closeBtn, Qt::AlignHCenter);

    this->setLayout(m_windowLayout);
}

AboutWindow::~AboutWindow() {
}

} // namespace
