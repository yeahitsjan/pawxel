// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "platform.h"
#include "application.h"
#include "easylogging++.h"
#include "select_window.h"

namespace pawxel {

SelectWindow::SelectWindow(bool _isDarkMode, QWidget *parent) : m_isDarkMode(_isDarkMode), FramelessWidget(parent) {
#if defined(Q_OS_WINDOWS)
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
#else
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
#endif
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->resize(QSize(600, 200));

    if (!m_windowLayout)
        m_windowLayout = new QHBoxLayout(this);
    m_windowLayout->setSpacing(2);
    m_windowLayout->setMargin(6);
    
    this->setLayout(m_windowLayout);
}

SelectWindow::~SelectWindow() {
}

void SelectWindow::onMultiPixmaps(QList<QPixmap> _lPixs) {
    LOG(DEBUG) << "(select_window:onMultiPixmaps)";
}

} // namespace