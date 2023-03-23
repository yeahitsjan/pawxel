// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <QWidget>
#include <QObject>

#include <QPaintEvent>
#include <QMouseEvent>

#include <QPoint>
#include <QRect>

namespace pawxel {

class SnipArea : public QWidget {
    Q_OBJECT
public:
    SnipArea(QWidget *parent = nullptr);
    ~SnipArea();

    // Grabs the shot out of the given rect.
    void grab(QRect _rect, QPoint _mousePos);

protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void paintEvent(QPaintEvent *ev) override;

private:
    QPoint m_mdPos;
    QPoint m_mcPos;
    QRect m_capArea;
    bool m_mouseDown;
    QRect calcArea(QPoint _pA, QPoint _pB);

signals:
    // Emitted when the user finished snipping.
    void snipped(QPixmap _pix, QPoint _mousePos);
};

} // namespace