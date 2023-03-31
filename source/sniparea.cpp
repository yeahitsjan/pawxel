// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "sniparea.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>

#include <QPainter>
#include <QPen>
#include <QPixmap>

namespace pawxel {

SnipArea::SnipArea(QWidget *parent) : QWidget(parent) {
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    this->setGeometry(QApplication::desktop()->geometry());
    this->setFixedSize(QApplication::desktop()->size());
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setCursor(Qt::CrossCursor);
}

SnipArea::~SnipArea() {
}

void SnipArea::grab(QRect _rect, QPoint _mousePos) {
    QPixmap _shot;
    // TODO: replace grabWindow with QScreen::grabWindow; the problem here is that we still want to have multi-monitor support. Do we?
    //_shot = QPixmap::grabWindow(0, _rect.topLeft().x(), _rect.topLeft().y(), _rect.width(), _rect.height());
    QScreen *_tl = QApplication::screenAt(QPoint(QApplication::desktop()->geometry().x(), QApplication::desktop()->geometry().y()));
    _shot = _tl->grabWindow(0, _rect.topLeft().x(), _rect.topLeft().y(), _rect.width(), _rect.height());
    // emit snipped -> transfer _shot + QPoint to display LittlePreviewWindow on mouse pos
    emit snipped(_shot, _mousePos);
    this->close();
}

void SnipArea::mousePressEvent(QMouseEvent *ev) {
    if (ev->button() != Qt::LeftButton) this->close();
    m_mdPos = ev->pos();
    m_capArea = this->calcArea(ev->pos(), ev->pos());
    m_mouseDown = true;
}

void SnipArea::mouseReleaseEvent(QMouseEvent *ev) {
    if (ev->button() != Qt::LeftButton) return;
    m_mouseDown = false;
    this->hide();
    this->grab(m_capArea, QPoint(ev->pos().x(), ev->pos().y()));
}

void SnipArea::mouseMoveEvent(QMouseEvent *ev) {
    if (!m_mouseDown) return;

    m_capArea = this->calcArea(m_mdPos, ev->pos());
    this->update();

    QWidget::mouseMoveEvent(ev);
}

void SnipArea::paintEvent(QPaintEvent *ev) {
    QPainter p(this);
    
    if (m_mouseDown) {
        p.setClipRegion(QRegion(QRect(QPoint(), this->size())).subtracted(QRegion(m_capArea)));
    }

#if defined(Q_OS_WIN)
    QColor _alphamin(0, 0, 0, 1);
    QColor _p2brush(0, 0, 0, 35);
    QColor _p2pen(245, 245, 245, 85);
#else
    QColor _alphamin(0, 0, 0, 10);
    QColor _p2brush(0, 0, 0, 1);
    QColor _p2pen(245, 245, 245, 1);
#endif
    // Difference in alpha as minimal as possible. User should not see that we draw over the screen.
    p.setBrush(_alphamin);
    p.drawRect(QRect(QPoint(), this->size()));

#if defined(Q_OS_WIN)
    if (m_mouseDown) {
        // We use a second QPainter instance to draw a select region background.
        QPainter p2(this);
        p2.setBrush(_p2brush);
        p2.setPen(QPen(_p2pen, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p2.drawRect(m_capArea);
    }
#endif
    
    QWidget::paintEvent(ev);
}

QRect SnipArea::calcArea(QPoint _pA, QPoint _pB) {
    return QRect(QPoint((_pA.x() <= _pB.x() ? _pA.x() : _pB.x()),
        (_pA.y() <= _pB.y() ? _pA.y() : _pB.y())),
        QPoint((_pA.x() >= _pB.x() ? _pA.x() : _pB.x()),
        (_pA.y() >= _pB.y() ? _pA.y() : _pB.y())));
}

} // namespace
