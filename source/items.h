// Pawxel - Copyright (c) 2022-2024 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <QObject>

#include <QColor>
#include <QPen>
#include <QFont>

#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QFocusEvent>

#include <FramelessWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

FRAMELESSHELPER_USE_NAMESPACE

namespace pawxel {

class BaseItem : public QObject {
    Q_OBJECT
public:
    BaseItem();
    
    QString itemHash() {
        return m_mdid;
    }
    
    QColor currentColor() { return m_color; }
    int currentWidth() { return m_width; }
    
    virtual QSizeF boundingRectSize();
    virtual QAbstractGraphicsShapeItem* innerItem();

protected:
    QString m_mdid;
    QPen m_pen;

    QColor m_color;
    int m_width;

public slots:
    virtual void onColorChange(QColor _col);
    virtual void onWidthChange(int _val);

    virtual void onResize(const QRectF &_rect);

signals:
    void itemEditRequested(BaseItem *_itm, QPoint _mousePos);
    void itemDeleteRequested(QString _itmHash);
    void resizerRequested(BaseItem *_itm);
    void itemLostFocus();
};

class RectItem : public BaseItem, public QGraphicsRectItem {
    Q_OBJECT
public:
    RectItem();

    QSizeF boundingRectSize() { return this->boundingRect().size(); }
    QAbstractGraphicsShapeItem* innerItem() { return this; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *ev) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void focusOutEvent(QFocusEvent *ev) override;

public slots:
    void onColorChange(QColor _col) override;
    void onWidthChange(int _val) override;
    void onResize(const QRectF &_rect) override;
};

class CircleItem : public BaseItem, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    CircleItem();

    QSizeF boundingRectSize() { return this->boundingRect().size(); }
    QAbstractGraphicsShapeItem* innerItem() { return this; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *ev) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void focusOutEvent(QFocusEvent *ev) override;

public slots:
    void onColorChange(QColor _col) override;
    void onWidthChange(int _val) override;
    void onResize(const QRectF &_rect) override;
};

class TextItem : public QGraphicsTextItem {
    Q_OBJECT
public:
    TextItem(QGraphicsItem *parent = nullptr);
    QString itemHash() {
        return m_mdid;
    }

    QString currentText() { return m_text; }
    QColor currentColor() { return m_color; }
    QFont currentFont() { return m_font; }
    int currentFontSize() { return m_font.pointSize(); } // TODO: catch -1

protected:
    QString m_mdid;

    QString m_text;
    QColor m_color;
    QFont m_font;

    void mousePressEvent(QGraphicsSceneMouseEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;

public slots:
    void onTextChange(const QString &_txt);
    void onColorChange(QColor _col);
    void onFontChange(QFont _f);

signals:
    void textEditRequested(TextItem *_item, QPoint _mousePos);
    void textDeleteRequested(const QString &_hash);
};

class DistanceItem : public BaseItem, public QGraphicsRectItem {
    Q_OBJECT
public:
    DistanceItem();

    QSizeF boundingRectSize() { return this->boundingRect().size(); }
    QAbstractGraphicsShapeItem* innerItem() { return this; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *ev) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void focusOutEvent(QFocusEvent *ev) override;

    QGraphicsSimpleTextItem *m_pxItem;

    int m_distance;

public slots:
    void onColorChange(QColor _col) override;
    void onResize(const QRectF &_rect) override;
};

class MarkerItem : public BaseItem, public QGraphicsRectItem {
    Q_OBJECT
public:
    MarkerItem();
    QString itemHash() {
        return m_mdid;
    }

    QSizeF boundingRectSize() { return this->boundingRect().size(); }
    QAbstractGraphicsShapeItem* innerItem() { return this; }

protected:
    QColor m_currentColor;
    void mousePressEvent(QGraphicsSceneMouseEvent *ev) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void focusOutEvent(QFocusEvent *ev) override;

public slots:
    void onColorChange(QColor _col) override;
    void onResize(const QRectF &_rect) override;
};

class EmojiItem : public BaseItem, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    EmojiItem(const QString &_emojiPath, QGraphicsItem *parent = nullptr);

    QSizeF boundingRectSize() { return this->boundingRect().size(); }
    //QGraphicsItem* innerItem() { return this; }

protected:
    QString m_emojiPath;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void focusOutEvent(QFocusEvent *ev) override;

public slots:
    void onResize(const QRectF &_rect) override;

signals:
    void svgDeleteRequested(const QString &_hash); // future
};

} // namespace
