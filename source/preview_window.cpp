// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "platform.h"
#include "application.h"
#include "easylogging++.h"
#include "preview_window.h"

#include <QGraphicsDropShadowEffect>

namespace pawxel {

ARPixPreviewLabel::ARPixPreviewLabel(QWidget *parent) : QLabel(parent) {
    this->setScaledContents(true);
}

ARPixPreviewLabel::~ARPixPreviewLabel() {
}

void ARPixPreviewLabel::resizeEvent(QResizeEvent *ev) {
    this->updateMargins();
    QLabel::resizeEvent(ev);
}

void ARPixPreviewLabel::updateMargins() {
    // I know that's a lot of code to center a QLabel...
    if (m_pixW <= 0 || m_pixH <= 0) return;

    int _w = this->width();
    int _h = this->height();
    if (_w <= 0 || _h <= 0) return;

    if (_w * m_pixH > _h * m_pixW) {
        int _m = (_w - (m_pixW * _h / m_pixH)) / 2;
        this->setContentsMargins(_m, 0, _m, 0);
    } else {
        int _m = (_h - (m_pixH * _w / m_pixW)) / 2;
        this->setContentsMargins(0, _m, 0, _m);
    }
}

void ARPixPreviewLabel::setPixmap(const QPixmap &_pix) {
    m_pixW = _pix.width();
    m_pixH = _pix.height();

    this->updateMargins();
    QLabel::setPixmap(_pix);
}

LittlePreviewWindow::LittlePreviewWindow(bool _autoSave, bool _isDarkMode, QWidget *parent) : m_autoSave(_autoSave), m_isDarkMode(_isDarkMode), FramelessWidget(parent) {
    // Always make sure our preview window is not going to be ignored.
#if defined(Q_OS_WINDOWS)
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
#else
    // TODO(@jan): Bug in framelesshelper. Lets analyze this some time later. Probably just something
    // missing in the definition of FramelessWidget.
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
#endif
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(QSize(240, 220));

    if (!m_windowLayout)
        m_windowLayout = new QVBoxLayout(this);
    m_windowLayout->setSpacing(2);
    m_windowLayout->setMargin(6);

    if (!m_pixPreview)
        m_pixPreview = new ARPixPreviewLabel(this);
    m_pixPreview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // todo: this needs tuning
    QGraphicsDropShadowEffect *_shadow = new QGraphicsDropShadowEffect;
    _shadow->setColor(QColor(0, 0, 0, 10));
    _shadow->setBlurRadius(0);
    _shadow->setOffset(1, 1);
    m_pixPreview->setGraphicsEffect(_shadow);
    
    m_windowLayout->addWidget(m_pixPreview);

    if (!m_actionBar)
        m_actionBar = new QToolBar(this);
    // disable toolbar-like behavior
    m_actionBar->setMovable(false);
    m_actionBar->setFloatable(false);
    m_actionBar->setFixedHeight(32); // fixed height
    {   
        m_lActionBtns.append( m_editBtn = new QPushButton("\ue3c9", m_actionBar) );
        connect(m_editBtn, &QPushButton::clicked, [=]() {
            emit userWantsEdit(m_pix);
        });
        m_actionBar->addWidget(m_editBtn);

        m_lActionBtns.append( m_copyBtn = new QPushButton("\ue173", m_actionBar) );
        connect(m_copyBtn, &QPushButton::clicked, this, [=]() {
            if (!m_pix.isNull()) {
                LOG(INFO) << "Copied to clipboard.";
                emit copyToClipboard(m_pix);
                this->close();
            }
        });
        m_actionBar->addWidget(m_copyBtn);

        m_lActionBtns.append( m_saveBtn = new QPushButton("\ue161", m_actionBar) );
        connect (m_saveBtn, &QPushButton::clicked, this, [=]() {
            if (!m_pix.isNull()) {
                if (m_autoSave && !PwxApp->preferences()->screenshotsFolder().startsWith("$EMPTY")) {
                    LOG(INFO) << "Auto-save requested.";
                    emit autoSaveToDisk(m_pix);
                } else {
                    LOG(INFO) << "Save to disk requested.";
                    emit saveToDisk(m_pix);
                }
                this->close(); // could this result in an unexpected behavior?
            }
        });
        m_actionBar->addWidget(m_saveBtn);

        QWidget *_space = new QWidget(m_actionBar);
        _space->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        m_actionBar->addWidget(_space);

        m_lActionBtns.append( m_retakeBtn = new QPushButton("\uf053", m_actionBar) );
        connect(m_retakeBtn, &QPushButton::clicked, this, [=]() {
            emit retake();
            this->hide();
        });
        m_actionBar->addWidget(m_retakeBtn);

        m_lActionBtns.append( m_trashBtn = new QPushButton("\ue872", m_actionBar) );
        connect(m_trashBtn, &QPushButton::clicked, this, [=]() {
            // TODO: should we clear the pixmap?
            this->close();
        });
        m_actionBar->addWidget(m_trashBtn);
        foreach (QPushButton *_btn, m_lActionBtns) {
            _btn->setFont(QFont("Material Icons Outlined", 16));
            _btn->setObjectName("PreviewActionButton");
            _btn->setIconSize(QSize(18, 18));
            _btn->setFixedSize(QSize(28, 28));
        }
    }
    m_windowLayout->addWidget(m_actionBar);

    this->setLayout(m_windowLayout);
}

LittlePreviewWindow::~LittlePreviewWindow() {
}

// Preview Hotkeys
// keycodes found on https://doc.qt.io/qt-5/qt.html#Key-enum
void LittlePreviewWindow::keyPressEvent(QKeyEvent *ev) {
    if (ev->modifiers() & Qt::ControlModifier) { // detects Control Modifier
        if (ev->key() == 0x043) { // detects c key
            if (!m_pix.isNull()) {
                emit copyToClipboard(m_pix);
                this->close();
            }
        }
        else if (ev->key() == 0x045) { // detects "e" key
            emit userWantsEdit(m_pix);
        }
        else if (ev->key() == 0x052) { // detects "r" key
            if (!m_pix.isNull()) {
                emit retake();
                this->hide();
            }
        }
        else if (ev->key() == 0x053) { // detects "s" key
            if (!m_pix.isNull()) {
                if (m_autoSave && !PwxApp->preferences()->screenshotsFolder().startsWith("$EMPTY")) {
                    emit autoSaveToDisk(m_pix);
                    this->close();
                } else {
                    emit saveToDisk(m_pix);
                    this->close();
                }
            }
        }
        else if (ev->key() == 0x057) { // detects "w" key
            if (!m_pix.isNull()) {
                this->close();
            }
        }
    }
    else if (ev->key() == 0x01000000) { // detects Escape key
        if (!m_pix.isNull()) {
            this->close();
        }
    }
}

void LittlePreviewWindow::onNewPix(QPixmap _pix) {
    if (!m_pixPreview)
        LOG(FATAL) << "(preview_window:onNewPix) Picture preview object not existing. That's fatal!"; // this is going to crash the application
    m_pix = _pix;
    m_pixPreview->setPixmap(_pix);
    m_pixPreview->show();
}

} // namespace