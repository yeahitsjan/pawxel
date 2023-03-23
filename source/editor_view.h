// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <QGraphicsView>
#include <QObject>
#include <QWidget>

#include <QPointer>
#include <QList>

#include <QPixmap>
#include <QPainter>
#include <QColor>

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>
#include <QKeyEvent>
#include <QRgb>

#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QEvent>

#include <FramelessDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>

#include <FHNotifier/FHNotifyWidget.h>

#include "graphicsitemresizer.h"

#include "items.h"

FRAMELESSHELPER_USE_NAMESPACE

namespace pawxel {

class ItemEditWindow : public FramelessDialog {
    Q_OBJECT
public:
    explicit ItemEditWindow(QWidget *parent = nullptr);
    ~ItemEditWindow();

protected:
    QPointer<QHBoxLayout> m_windowLayout;
    QPushButton *m_colorBtn;
    QSlider *m_widthSlider;

    QPointer<BaseItem> m_item;

private:
    bool m_connected = false;

public slots:
    void onItemChange(BaseItem *_item);
    void onColorButtonClicked();

signals:
    void colorChanged(QColor _col);
};

class TextEditWindow : public FramelessDialog {
    Q_OBJECT
public:
    TextEditWindow(QWidget *parent = nullptr);
    ~TextEditWindow();

protected:
    QPointer<QHBoxLayout> m_windowLayout;
    QLineEdit *m_textEdit;
    QPushButton *m_colorBtn;
    QPushButton *m_fontBtn;
    QPushButton *m_fontSizeBox;

    QPointer<TextItem> m_item;

private:
    bool m_connected = false;

public slots:
    void onItemChange(TextItem *_item);
    void onEditingFinished();
    void onColorButtonClicked();
    void onFontButtonClicked();

signals:
    void textChanged(const QString &_txt);
    void colorChanged(QColor _col);
    void fontChanged(QFont _f);
};

class BackgroundBuilder_P {
public:
    static QPixmap drawPattern(int _pattern, int _step, const QColor &_colA, const QColor &_colB);

private:
    static void drawSeq(QPainter *_painter, int _w, const QColor &_col);
    static void drawCross(QPainter *_painter, int _w, const QColor &_col);
#if PAWXEL_EXPERIMENTAL
    static void drawChessboard(QPainter *_painter, const QColor &_colA, const QColor &_colB);
#endif
};

class PixItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;

    QRgb m_currentValue;

signals:
    // Emitted when the underlying pixel has a new color.
    void hoverColorChange(QRgb _val);
    // Emitted when the user presses Alt on keyboard.
    void colorCopyRequested(QString _rgbName);
};

class EditorView : public QGraphicsView {
    Q_OBJECT
public:
    explicit EditorView(bool _isDarkMode, QWidget *parent = nullptr);
    ~EditorView();

    void zoom(double _fac);
    void setZoomFactorBase(double _val);

protected:
    //void wheelEvent(QWheelEvent *ev) override;
    void contextMenuEvent(QContextMenuEvent *ev) override;
    bool eventFilter(QObject *object, QEvent *ev) override;

private:
    bool m_isDarkMode;
    QPointer<ItemEditWindow> m_itemEditor;
    QPointer<TextEditWindow> m_textEditor;

    QPointer<FHNotifyWidget> m_colorCopyNotification;

    QPointer<PixItem> m_pixItem;
    QPixmap m_pix;

    QPointer<GraphicsItemResizer> m_resizeItem;
    bool m_resizerItemConnected = false;
    QList<QString> m_lItemsHash;
    QList<BaseItem*> m_lBaseItems;
    QList<TextItem*> m_lTextItems;
    QList<EmojiItem*> m_lSvgItems; // future

    Qt::KeyboardModifiers  m_modifiers;
    double m_zoomFacBase;
    QPointF _targetScenePos, _targetViewportPos;

    // Render the current scene into a pixmap.
    const QPixmap renderScene();

public slots:
    void gotPix(QPixmap _pix);
    void onHoverColorChange(QRgb _val);
    void onColorCopyRequested(QString _rgbName);
    void onItemEditRequested(BaseItem *_itm, QPoint _mousePos);
    void onTextEditRequested(TextItem *_itm, QPoint _mousePos);
    void onItemDeleteRequested(QString _itmHash);
    void onTextDeleteRequested(QString _itmHash);
    void onEmojiDeleteRequested(QString _itmHash);

    void onResizerRequested(BaseItem *_itm);
    void onItemLostFocus();

    void imageFromRectRequested(const QRect &_rect, const QPointF &_from, const QPointF &_to);

    // Adds an item to the scene based on it's Id.
    void addItem(int _id);
    void addEmoji(const QString &_path);

    // Render and send as save.
    void saveRequested();
    // Render and send as copy.
    void copyRequested();

    // General function to clear the viewport. Mainly used on editor close.
    void clearViewport();

signals:
    // Send color under pixel to the color info widget.
    void feedColorInfoWidget(QRgb _val);

    void sendSaveFile(QPixmap _pix);
    void sendCopyFile(QPixmap _pix);
    void sendItemToEdit(BaseItem *_itm);
    void sendTextToEdit(TextItem *_itm);

    // Emitted on user interaction. Required to animate
    // UI elements.
    void mouseDown();
    void mouseRelease();

    // Emitted when user zoomed.
    void zoomed();
};

} // namespace