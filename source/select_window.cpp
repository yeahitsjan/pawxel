// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "platform.h"
#include "application.h"
#include "easylogging++.h"
#include "select_window.h"

#include <QPushButton>

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
    for (int i = 0; i < _lPixs.size(); i++) {
        QPixmap _pix = _lPixs.at(i);
        QPushButton *_btn = new QPushButton;
        {
            _btn->setObjectName("ActionButtons");
            _btn->setFixedSize(QSize(198, 198));
            _btn->setIconSize(QSize(_pix.width() / 12, _pix.height() / 12)); // todo: refine this
            _btn->setIcon(QIcon(_pix));
        }
        connect(_btn, &QPushButton::clicked, [this, _pix] { /* todo */ });
        m_windowLayout->addWidget(_btn);
    }
}

} // namespace