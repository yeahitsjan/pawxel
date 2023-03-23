// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "platform.h"
#include "application.h"
#include "easylogging++.h"
#include "items.h"

namespace pawxel {

BaseItem::BaseItem() {
    // Initial values.
    m_pen.setColor(QColor(255, 0, 0));
    m_pen.setWidth(4);
    {
        m_color = m_pen.color();
        m_width = m_pen.width();
    }
}

QSizeF BaseItem::boundingRectSize() {
    LOG(DEBUG) << "<QSizeF>BaseItem::boundingRectSize()";
}

QAbstractGraphicsShapeItem* BaseItem::innerItem() {
    LOG(DEBUG) << "<QAbstractGraphicsShapeItem*>BaseItem::innerItem()";
}

void BaseItem::onColorChange(QColor _col) {
    LOG(DEBUG) << "BaseItem::onColorChange(QColor)";
}

void BaseItem::onWidthChange(int _val) {
    LOG(DEBUG) << "BaseItem::onWidthChange(int)";
}

void BaseItem::onResize(const QRectF &_rect) {
    LOG(DEBUG) << "BaseItem::onResize(const QRectF&)";
}

RectItem::RectItem() {
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    // Initial values. Color red, width 2.
    m_pen.setColor(QColor(PwxApp->preferences()->lastColor()));
    m_pen.setWidth(2);
    {
        m_color = m_pen.color();
        m_width = m_pen.width();
    }
    // Always have a transparent inner.
    this->setPen(m_pen);
    this->setBrush(QColor(255, 255, 255, 0));
    // Initial position and size.
    this->setRect(QRectF(0, 0, 150, 50));

    // Generate item hash.
    m_mdid = PWX_md5FromString("rect#" + PWX_dateTimeStr());
}

void RectItem::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
    if (ev->button() == Qt::RightButton) {
        emit itemEditRequested(this, ev->screenPos());
    }
}

void RectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        emit resizerRequested(this);
    }
}

void RectItem::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key_Delete) {
        LOG(DEBUG) << "rect# Delete key pressed.";
        emit itemDeleteRequested(this->itemHash());
    }
}

void RectItem::focusOutEvent(QFocusEvent *ev) {
    emit itemLostFocus();
}

void RectItem::onColorChange(QColor _col) {
    m_pen.setColor(_col);
    m_pen.setWidth(m_width);
    this->setPen(m_pen);
    this->update();

    LOG(DEBUG) << m_mdid << ": Item color changed to: " << _col.name();
    PwxApp->preferences()->setLastColor(_col.name());
    m_color = m_pen.color();
}

void RectItem::onWidthChange(int _val) {
    m_pen.setColor(m_color);
    m_pen.setWidth(_val);
    this->setPen(m_pen);
    this->update();

    LOG(DEBUG) << m_mdid << ": Item width changed to: " << _val;
    m_width = m_pen.width();
}

void RectItem::onResize(const QRectF &_rect) {
    QPointF _pos = this->pos();
    this->setPos(_pos + _rect.topLeft());
    QRectF _old = this->rect();
    this->setRect(QRectF(_old.topLeft(), _rect.size()));
}

CircleItem::CircleItem() {
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    m_pen.setColor(QColor(PwxApp->preferences()->lastColor()));
    m_pen.setWidth(4);
    {
        m_color = m_pen.color();
        m_width = m_pen.width();
    }
    this->setPen(m_pen);
    this->setBrush(QColor(255, 255, 255, 0));
    this->setRect(QRectF(0, 0, 100, 100));

    m_mdid = PWX_md5FromString("ellipse#" + PWX_dateTimeStr());
}

void CircleItem::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
    if (ev->button() == Qt::RightButton) {
        emit itemEditRequested(this, ev->screenPos());
    }
}

void CircleItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        emit resizerRequested(this);
    }
}

void CircleItem::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key_Delete) {
        LOG(DEBUG) << "ellipse# Delete key pressed.";
        emit itemDeleteRequested(this->itemHash());
    }
}

void CircleItem::focusOutEvent(QFocusEvent *ev) {
    emit itemLostFocus();
}

void CircleItem::onColorChange(QColor _col) {
    m_pen.setColor(_col);
    m_pen.setWidth(m_width);
    this->setPen(m_pen);
    this->update();

    LOG(DEBUG) << m_mdid << ": Item color changed to: " << _col.name();
    PwxApp->preferences()->setLastColor(_col.name());
    m_color = m_pen.color();
}

void CircleItem::onWidthChange(int _val) {
    m_pen.setColor(m_color);
    m_pen.setWidth(_val);
    this->setPen(m_pen);
    this->update();

    LOG(DEBUG) << m_mdid << ": Item width changed to: " << _val;
    m_width = m_pen.width();
}

void CircleItem::onResize(const QRectF &_rect) {
    QPointF _pos = this->pos();
    this->setPos(_pos + _rect.topLeft());
    QRectF _old = this->rect();
    this->setRect(QRectF(_old.topLeft(), _rect.size()));
}

TextItem::TextItem(QGraphicsItem *parent) : QGraphicsTextItem(parent) {
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setPlainText(tr("Text"));
    m_text = this->toPlainText();
    m_color = QColor(PwxApp->preferences()->lastColor());
    m_font = QFont(PLATFORM_FONT, PLATFORM_FONT_SIZE);
    this->setDefaultTextColor(m_color);
    this->setFont(m_font);
    this->setPos(10, 10);

    m_mdid = PWX_md5FromString("text#" + PWX_dateTimeStr());
}

void TextItem::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
    if (ev->button() == Qt::RightButton) {
        emit textEditRequested(this, ev->screenPos());
    }
    QGraphicsTextItem::mousePressEvent(ev);
}

void TextItem::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key_Delete) {
        LOG(DEBUG) << "text# Delete key pressed.";
        emit textDeleteRequested(this->itemHash());
    }
}

void TextItem::onTextChange(const QString &_txt) {
    this->setPlainText(_txt);
    this->update();

    LOG(DEBUG) << "Item text changed.";
    m_text = _txt;
}

void TextItem::onColorChange(QColor _col) {
    this->setDefaultTextColor(_col);
    this->update();

    LOG(DEBUG) << "Item text color changed to: " << _col.name();
    PwxApp->preferences()->setLastColor(_col.name());
    m_color = _col;
}

void TextItem::onFontChange(QFont _f) {
    this->setFont(_f);
    this->update();

    LOG(DEBUG) << "Item font changed to: " << _f.family();
    m_font = _f;
}

DistanceItem::DistanceItem() {
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    m_pen.setColor(QColor(PwxApp->preferences()->lastColor()));
    m_pen.setWidth(0);
    {
        m_color = m_pen.color();
        m_width = m_pen.width();
    }
    this->setPen(m_pen);
    this->setBrush(m_color);
    this->setRect(QRectF(0, 0, 150, 6));

    m_distance = this->rect().width();

    m_pxItem = new QGraphicsSimpleTextItem("px", this->innerItem());
    {
        m_pxItem->setPen(m_pen);
        m_pxItem->setBrush(m_color);
        m_pxItem->setFont(QFont(APP_FONT, 12));
        m_pxItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        m_pxItem->setPos(this->rect().topLeft());
        // Set current pixel distance
        m_pxItem->setText(QString::number(m_distance) + " px");
    }

    m_mdid = PWX_md5FromString("distance#" + PWX_dateTimeStr());
}

void DistanceItem::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
    if (ev->button() == Qt::RightButton) {
        emit itemEditRequested(this, ev->screenPos());
    }
}

void DistanceItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        emit resizerRequested(this);
    }
}

void DistanceItem::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key_Delete) {
        LOG(DEBUG) << "distance# Delete key pressed.";
        emit itemDeleteRequested(this->itemHash());
    }
}

void DistanceItem::focusOutEvent(QFocusEvent *ev) {
    emit itemLostFocus();
}

void DistanceItem::onColorChange(QColor _col) {
    m_pen.setColor(_col);
    m_pen.setWidth(m_width);

    // Update the line
    this->setPen(m_pen);
    this->setBrush(_col);
    this->update();

    // Update the text color
    m_pxItem->setPen(m_pen);
    m_pxItem->setBrush(_col);
    m_pxItem->update();

    LOG(DEBUG) << m_mdid << ": Item color changed to: " << _col.name();
    PwxApp->preferences()->setLastColor(_col.name());
    m_color = m_pen.color();
}

void DistanceItem::onResize(const QRectF &_rect) {
    QRectF _crect = _rect;
    _crect.setHeight(6);
    m_distance = _rect.width();

    QPointF _pos = this->pos();
    this->setPos(_pos + _crect.topLeft());
    QRectF _old = this->rect();
    this->setRect(QRectF(_old.topLeft(), _crect.size()));

    m_pxItem->setText(QString::number(m_distance) + " px");
    m_pxItem->update();
}

MarkerItem::MarkerItem() {
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    m_pen.setColor(QColor(255, 255, 255, 0));
    m_pen.setWidth(1);
    {
        m_currentColor = QColor(0, 255, 0, 75);
        m_color = m_currentColor;
    }
    this->setPen(m_pen);
    this->setBrush(m_currentColor);
    this->setRect(QRectF(0, 0, 150, 25));

    m_mdid = PWX_md5FromString("marker#" + PWX_dateTimeStr());
}

void MarkerItem::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
    if (ev->button() == Qt::RightButton) {
        emit itemEditRequested(this, ev->screenPos());
    }
}

void MarkerItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        emit resizerRequested(this);
    }
}

void MarkerItem::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key_Delete) {
        LOG(DEBUG) << "marker# Delete key pressed.";
        emit itemDeleteRequested(this->itemHash());
    }
}

void MarkerItem::focusOutEvent(QFocusEvent *ev) {
    emit itemLostFocus();
}

void MarkerItem::onColorChange(QColor _col) {
    QColor _ecol(_col.red(), _col.green(), _col.blue(), 75);
    m_pen.setColor(_ecol);
    m_pen.setWidth(m_width);
    m_currentColor = _ecol;

    // Update the marker
    this->setPen(m_pen);
    this->setBrush(_ecol);
    this->update();

    LOG(DEBUG) << m_mdid << ": Item color changed to: " << _col.name();
    m_color = m_pen.color();
}

void MarkerItem::onResize(const QRectF &_rect) {
    QPointF _pos = this->pos();
    this->setPos(_pos + _rect.topLeft());
    QRectF _old = this->rect();
    this->setRect(QRectF(_old.topLeft(), _rect.size()));
}

EmojiItem::EmojiItem(const QString &_emojiPath, QGraphicsItem *parent) : m_emojiPath(_emojiPath), QGraphicsPixmapItem(parent) {
    if (_emojiPath.isEmpty()) {
        LOG(FATAL) << "(emojiitem::emojiitem) You should not be able to reach this, or you allow item creation without arguments. That's stupid...";
    }
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setPixmap(QPixmap(m_emojiPath));
    this->setTransformationMode(Qt::SmoothTransformation); // TODO: related to text AA option?

    m_mdid = PWX_md5FromString("emoji#" + PWX_dateTimeStr());
}

void EmojiItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        emit resizerRequested(this);
    }
}

void EmojiItem::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key_Delete) {
        LOG(DEBUG) << "emoji# Delete key pressed.";
        emit svgDeleteRequested(this->itemHash());
    }
}

void EmojiItem::focusOutEvent(QFocusEvent *ev) {
    emit itemLostFocus();
}

void EmojiItem::onResize(const QRectF &_rect) {
    // todo: resize
}

} // namespace