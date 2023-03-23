// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "easylogging++.h"
#include "platform.h"
#include "application.h"
#include "editor_view.h"

#include "items.h"

#include <QtOpenGL>
#include <QColor>

#include <QtMath>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPointF>

#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>

#include <QImage>
#include <QPainter>

#include <QColorDialog>
#include <QFontDialog>
#include <QClipboard>

namespace pawxel {

ItemEditWindow::ItemEditWindow(QWidget *parent) : FramelessDialog(parent) {
    this->setFont(QFont("Josefin Sans", 9));
#if defined(Q_OS_LINUX)
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
#else
    this->setWindowFlags(Qt::Tool);
#endif
    this->setMinimumSize(QSize(200, 35));
    this->setFixedSize(QSize(200, 35));

    if (!m_windowLayout)
        m_windowLayout = new QHBoxLayout(this);
    m_windowLayout->setSpacing(0);
    m_windowLayout->setMargin(0);
    
    m_colorBtn = new QPushButton(this);
    m_colorBtn->setCursor(Qt::PointingHandCursor);
    m_colorBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_colorBtn, &QPushButton::clicked, this, &ItemEditWindow::onColorButtonClicked);
    m_colorBtn->setFixedWidth(40);
    m_windowLayout->addWidget(m_colorBtn);

    m_widthSlider = new QSlider(Qt::Horizontal, this);
    m_widthSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_widthSlider->setMinimum(2);
    m_widthSlider->setMaximum(100);
    m_windowLayout->addWidget(m_widthSlider);

    this->setLayout(m_windowLayout);
}

ItemEditWindow::~ItemEditWindow() {
}

void ItemEditWindow::onItemChange(BaseItem *_item) {
    // This feels and looks hella stupid, but it works perfectly fine. Still, we should
    // take another look into this sometime soon...
    if (m_connected) {
        m_widthSlider->disconnect();
        this->disconnect();
        m_connected = false;
    }
    m_item = _item;
    m_colorBtn->setStyleSheet("border: none; background-color: " + m_item->currentColor().name() + ";");
    m_widthSlider->setValue(m_item->currentWidth());
    m_connected = connect(m_widthSlider, &QSlider::sliderMoved, m_item, &BaseItem::onWidthChange);
    connect(this, &ItemEditWindow::colorChanged, m_item, &BaseItem::onColorChange);
}

void ItemEditWindow::onColorButtonClicked() {
    QColor _initial;
    if (!m_item)
        _initial = QColor(255, 0, 0);
    else
        _initial = m_item->currentColor();
    QColor _res = QColorDialog::getColor(_initial);
    if (_res.isValid()) {
        m_colorBtn->setStyleSheet("border: none; background-color: " + _res.name() + ";");
        emit colorChanged(_res);
    }
}

TextEditWindow::TextEditWindow(QWidget *parent) : FramelessDialog(parent) {
    this->setFont(QFont("Josefin Sans", 8));
    this->setAttribute(Qt::WA_DeleteOnClose);
#if defined(Q_OS_LINUX)
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
#else
    this->setWindowFlags(Qt::Tool);
#endif
    this->setMinimumSize(QSize(400, 35));
    this->setFixedSize(QSize(400, 35));
    
    if (!m_windowLayout)
        m_windowLayout = new QHBoxLayout(this);
    m_windowLayout->setSpacing(0);
    m_windowLayout->setMargin(0);

    m_textEdit = new QLineEdit;
    m_textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_textEdit->setPlaceholderText(tr("Empty deletes the item"));
    m_windowLayout->addWidget(m_textEdit);

    m_colorBtn = new QPushButton;
    m_colorBtn->setCursor(Qt::PointingHandCursor);
    m_colorBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_colorBtn, &QPushButton::clicked, this, &TextEditWindow::onColorButtonClicked);
    m_colorBtn->setFixedWidth(40);
    m_windowLayout->addWidget(m_colorBtn);

    m_fontBtn = new QPushButton;
    m_fontBtn->setCursor(Qt::PointingHandCursor);
    m_fontBtn->setObjectName("RTLButton");
    m_fontBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_fontBtn, &QPushButton::clicked, this, &TextEditWindow::onFontButtonClicked);
    m_fontBtn->setFixedWidth(150);
    m_windowLayout->addWidget(m_fontBtn);

    m_fontSizeBox = new QPushButton;
    m_fontSizeBox->setObjectName("RTLButton");
    m_fontSizeBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_fontSizeBox->setFixedWidth(60);
    m_windowLayout->addWidget(m_fontSizeBox);

    this->setLayout(m_windowLayout);
}

TextEditWindow::~TextEditWindow() {
}

void TextEditWindow::onItemChange(TextItem *_item) {
    if (m_connected) {
        m_textEdit->disconnect();
        this->disconnect();
        m_connected = false;
    }
    m_item = _item;
    m_textEdit->setText(m_item->currentText());
    m_colorBtn->setStyleSheet("border: none; background-color: " + m_item->currentColor().name() + ";");
    m_fontBtn->setText(m_item->currentFont().family());
    m_fontBtn->setFont(QFont(m_item->currentFont().family(), PLATFORM_FONT_SIZE-2));
    m_fontSizeBox->setText(QString::number(m_item->currentFontSize()));
    m_connected = connect(m_textEdit, &QLineEdit::editingFinished, this, &TextEditWindow::onEditingFinished);
    {
        connect(this, &TextEditWindow::textChanged, m_item, &TextItem::onTextChange);
        connect(this, &TextEditWindow::colorChanged, m_item, &TextItem::onColorChange);
        connect(this, &TextEditWindow::fontChanged, m_item, &TextItem::onFontChange);
    }
}

void TextEditWindow::onEditingFinished() {
    if (!m_textEdit->text().isEmpty()) {
        emit textChanged(m_textEdit->text());
    } else {
        // delete item
    }
}

void TextEditWindow::onColorButtonClicked() {
    QColor _initial;
    if (!m_item)
        _initial = QColor(255, 0, 0);
    else
        _initial = m_item->currentColor();
    QColor _res = QColorDialog::getColor(_initial);
    if (_res.isValid()) {
        m_colorBtn->setStyleSheet("border: none; background-color: " + _res.name() + ";");
        emit colorChanged(_res);
    }
}

void TextEditWindow::onFontButtonClicked() {
    QFont _initial;
    if (!m_item)
        _initial = QFont(PLATFORM_FONT, PLATFORM_FONT_SIZE);
    else
        _initial = m_item->currentFont();
    bool _ok;
    QFont _res = QFontDialog::getFont(&_ok, _initial, this);
    if (_ok) {
        m_fontBtn->setFont(QFont(_res.family(), PLATFORM_FONT_SIZE-2));
        m_fontBtn->setText(_res.family());
        m_textEdit->setFont(QFont(_res.family(), PLATFORM_FONT_SIZE));
        m_fontSizeBox->setText(QString::number(_res.pointSize()));
        emit fontChanged(_res);
    }
}

QPixmap BackgroundBuilder_P::drawPattern(int _pattern, int _step, const QColor &_colA, const QColor &_colB) {
    QPixmap _px(_step, _step);
    QPainter _p;

    int _pxw = _px.width() - 1;
    _px.fill(_colA);

    _p.begin(&_px);
    // Draw pattern based on selection.
    switch (_pattern) {
        case 0: {
            drawCross(&_p, _pxw, _colB);
        } break;
        case 1: {
            drawSeq(&_p, _pxw, _colB);
        } break;
#if PAWXEL_EXPERIMENTAL
        case 2: {
            drawChessboard(&_p, _colA, _colB);
        } break;
#endif
    }
    return _px;
}

void BackgroundBuilder_P::drawSeq(QPainter *_painter, int _w, const QColor &_col) {
    _painter->setPen(_col);
    _painter->drawLine(0, 0, _w, 0);
    _painter->drawLine(0, 0, 0, _w);
}

void BackgroundBuilder_P::drawCross(QPainter *_painter, int _w, const QColor &_col) {
    _painter->setPen(_col);
    _painter->drawLine(0, 0, 2, 0);
    _painter->drawLine(0, 0, 0, 2);
    _painter->drawLine(0, _w - 1, 0, _w);
    _painter->drawLine(_w - 1, 0, _w, 0);
}

#if PAWXEL_EXPERIMENTAL
void BackgroundBuilder_P::drawChessboard(QPainter *_painter, const QColor &_colA, const QColor &_colB) {
    int _rows = 40;
    int _columns = 40;

    int _sz = _columns * _rows;
    int _tSz = 40;

    for (int i = 0; i < _sz; ++i) {
        int _ridx = i % _rows;
        int _cidx = i / _columns;

        int x = _ridx * _tSz;
        int y = _cidx * _tSz;
        
        auto _color = (i % 2 == 0) ? QColor("red") : QColor("green");
        _painter->fillRect(QRectF(x, y, _tSz, _tSz), _color);
    }
}
#endif

void PixItem::hoverMoveEvent(QGraphicsSceneHoverEvent *ev) {
    QPointF _mPos = ev->pos();
    // Get the current pixel color and send it.
    QRgb _value = this->pixmap().toImage().pixel(_mPos.x(), _mPos.y());
    m_currentValue = _value;
    emit hoverColorChange(_value);
}

void PixItem::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key_Alt) {
        QColor _r = QColor(m_currentValue);
        emit colorCopyRequested(_r.name());
    }
}

EditorView::EditorView(bool _isDarkMode, QWidget *parent) : m_isDarkMode(_isDarkMode), QGraphicsView(parent) {
    // Default setup of the QGraphicsView
    this->setDragMode(QGraphicsView::ScrollHandDrag);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setCacheMode(QGraphicsView::CacheBackground);
    // Zoom enhancements
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    this->setFrameShape(QFrame::NoFrame);

    // GL acceleration / anti-aliasing.
    LOG(DEBUG) << "(editor_view:EditorView) Should editor use GL accel?=" << PwxApp->preferences()->editorUsesGLAcceleration();
    if (PwxApp->preferences()->editorUsesGLAcceleration())
        this->setViewport(new QOpenGLWidget);
    else
        this->setViewport(new QWidget);
    LOG(DEBUG) << "(editor_view:EditorView) Should editor use text AA?=" << PwxApp->preferences()->editorUsesTextAA();
    if (PwxApp->preferences()->editorUsesTextAA())
        this->setRenderHint(QPainter::Antialiasing, true);
    else
        this->setRenderHint(QPainter::Antialiasing, false);
    
    // Enable zooming, clicking etc.
    this->viewport()->installEventFilter(this);
    this->setMouseTracking(true);
    m_modifiers = Qt::ControlModifier;
    m_zoomFacBase = 1.0015;

    // Install resizer item.
    if (!m_resizeItem)
        m_resizeItem = new GraphicsItemResizer;
    m_resizeItem->setBrush(QColor(64, 64, 64));
    m_resizeItem->setMinSize(QSizeF(1, 1));
    m_resizeItem->setHandlersIgnoreTransformations(true);

    QColor _a;
    QColor _b;
    if (_isDarkMode) {
        _a = QColor(43, 43, 43);
        _b = QColor(33, 33, 33);
    } else {
        _a = QColor(254, 254, 254);
        _b = QColor(197, 197, 197);
    }
    this->setBackgroundBrush(BackgroundBuilder_P::drawPattern(PwxApp->preferences()->editorBackground(), 50, _a, _b));
    this->setScene(new QGraphicsScene(this));
}

EditorView::~EditorView() {
}

void EditorView::zoom(double _fac) {
    this->scale(_fac, _fac);
    this->centerOn(_targetScenePos);
    QPointF _delta = _targetViewportPos - QPointF(this->viewport()->width() / 2.0,
                                                    this->viewport()->height() / 2.0);
    QPointF _viewportCenter = this->mapFromScene(_targetScenePos) - _delta;
    this->centerOn(this->mapToScene(_viewportCenter.toPoint()));
}

void EditorView::setZoomFactorBase(double _val) {
    m_zoomFacBase = _val;
}

#if 0
// @DEPRECATED
// But still, do not move this, could be relevant in future...
void EditorView::wheelEvent(QWheelEvent *ev) {
    if (ev->modifiers() & Qt::ControlModifier) {
        const qreal detail = QStyleOptionGraphicsItem::levelOfDetailFromTransform( this->transform() );
        const qreal fac = 1.1;
        if ( (detail < 3.5) && (ev->angleDelta().y() > 0) ) {
            this->scale(fac, fac);
        }
        if ( (detail > 1.0) && (ev->angleDelta().y() < 0) ) {
            this->scale( (1 / fac), (1 / fac));
        }
    } else {
        QGraphicsView::wheelEvent(ev);
    }
}
#endif

void EditorView::contextMenuEvent(QContextMenuEvent *ev) {
    //todo
}

bool EditorView::eventFilter(QObject *object, QEvent *ev) {
    if (ev->type() == QEvent::MouseMove) {
        QMouseEvent *_mouseEv = (QMouseEvent *)ev;
        QPointF _delta = _targetViewportPos - _mouseEv->pos();
        if (qAbs(_delta.x()) > 5 || qAbs(_delta.y()) > 5) {
            _targetViewportPos = _mouseEv->pos();
            _targetScenePos = this->mapToScene(_mouseEv->pos());
        }
    } else if (ev->type() == QEvent::Wheel) {
        QWheelEvent *_wheelEv = (QWheelEvent *)ev;
        if (_wheelEv->modifiers() & m_modifiers) {
            if (_wheelEv->orientation() == Qt::Vertical) {
                double _angle = _wheelEv->angleDelta().y();
                double _fac = qPow(m_zoomFacBase, _angle);
                this->zoom(_fac);
                return true;
            }
        }
    } else if (ev->type() == QEvent::MouseButtonPress) {
        QMouseEvent *_mousePressEv = (QMouseEvent *)ev;
        if (_mousePressEv->button() == Qt::LeftButton) {
            if (m_itemEditor) m_itemEditor->close();
            if (m_textEditor) m_textEditor->close();
            emit mouseDown();
        }
    } else if (ev->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *_mousePressEv = (QMouseEvent *)ev;
        if (_mousePressEv->button() == Qt::LeftButton) emit mouseRelease();
    }
    Q_UNUSED(object);
    return false;
}

const QPixmap EditorView::renderScene() {
    // If we do not clear the selection first it will also get rendered.
    this->scene()->clearSelection();
    // Hide the resizer item.
    bool _wasVisible = false;
    if (m_resizeItem->isVisible()) {
        m_resizeItem->hide();
        _wasVisible = true;
    }
    
    QImage _saveImg(this->scene()->sceneRect().size().toSize(), QImage::Format_ARGB32);
    _saveImg.fill(Qt::transparent);

    QPainter _p(&_saveImg);
    _p.setRenderHint(QPainter::Antialiasing, PwxApp->preferences()->editorUsesTextAA());
    //_p.setRenderHint(QPainter::SmoothPixmapTransform, true); => user decision?
    this->scene()->render(&_p);
    QPixmap _res = QPixmap::fromImage(_saveImg);

    if (_wasVisible) {
        m_resizeItem->show();
        _wasVisible = false;
    }

    return _res;
}

void EditorView::gotPix(QPixmap _pix) {
    // TODO: After we got another new pixmap, the scene rect is probably not resetting back
    // to 0,0. This result in a bad output when saving to disk. I assume it can be fixed
    // like here: https://stackoverflow.com/questions/34449357/qgraphicssceneclear-doesnt-change-scenerect
    // Investigate this!
    this->scene()->clear();
    m_pix = _pix;
    //m_pixItem = this->scene()->addPixmap(m_pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    if (!m_pixItem)
        m_pixItem = new PixItem;
    // TODO(jan): Setting focus on item to retrieve the color via Alt is good. The only problem here
    // is if the item looses focus. Thats not good. Should we find another way? Global key press event (on EditorWindow)?
    // The value is persistent and always filled when hoverColorChange is emitted.
    m_pixItem->setFlag(QGraphicsItem::ItemIsFocusable, true);
    m_pixItem->setPixmap(m_pix);
    m_pixItem->setAcceptHoverEvents(true); // be able to get the color of a pixel
    this->scene()->addItem(m_pixItem);
    this->scene()->setSceneRect(this->scene()->itemsBoundingRect());
    this->scene()->setFocusItem(m_pixItem);
    connect(m_pixItem, &PixItem::hoverColorChange, this, &EditorView::onHoverColorChange);
    connect(m_pixItem, &PixItem::colorCopyRequested, this, &EditorView::onColorCopyRequested);
}

void EditorView::onHoverColorChange(QRgb _val) {
    emit feedColorInfoWidget(_val);
}

void EditorView::onColorCopyRequested(QString _rgbName) {
    if (!m_colorCopyNotification)
        m_colorCopyNotification = new FHNotifyWidget(QColor(), m_isDarkMode);
    m_colorCopyNotification->setNotificationText("Copied <b>" + _rgbName + "</b> to clipboard.");
    m_colorCopyNotification->setTimespan(1500);
    m_colorCopyNotification->setFixedSize(QSize(225, 30)); // workaround
    m_colorCopyNotification->setCloseButtonDisabled();
    m_colorCopyNotification->notificationTextLabel()->setFont(QFont("Josefin Sans", 9));
    
#if defined(Q_OS_WINDOWS)
    PwxApp->clipboard()->setText(_rgbName);
#else
    PwxApp->clipboard()->setText(_rgbName, QClipboard::Selection);
#endif

    m_colorCopyNotification->notify();
}

void EditorView::onItemEditRequested(BaseItem *_itm, QPoint _mousePos) {
    if (!m_itemEditor)
        m_itemEditor = new ItemEditWindow;
    m_itemEditor->setAttribute(Qt::WA_DeleteOnClose);
    m_itemEditor->setObjectName("ItemEditor");
    connect(this, &EditorView::sendItemToEdit, m_itemEditor, &ItemEditWindow::onItemChange, Qt::UniqueConnection);
    m_itemEditor->show();
    m_itemEditor->move(_mousePos);
    emit sendItemToEdit(_itm);
}

void EditorView::onTextEditRequested(TextItem *_itm, QPoint _mousePos) {
    if (!m_textEditor)
        m_textEditor = new TextEditWindow;
    m_textEditor->setObjectName("TextEditor");
    connect(this, &EditorView::sendTextToEdit, m_textEditor, &TextEditWindow::onItemChange, Qt::UniqueConnection);
    m_textEditor->show();
    m_textEditor->move(_mousePos);
    emit sendTextToEdit(_itm);
}

/*** TODO: combine the following two functions ***/
void EditorView::onItemDeleteRequested(QString _itmHash) {
    foreach(BaseItem *_itm, m_lBaseItems) {
        if (_itm->itemHash() == _itmHash) {
            LOG(DEBUG) << "(editor_view:onItemDeleteRequested) Found item on stack.";
            this->scene()->removeItem(_itm->innerItem());
            m_lBaseItems.removeAll(_itm);
            m_lItemsHash.removeAll(_itmHash);
        } else {
            LOG(DEBUG) << "(editor_view:onItemDeleteRequested) _itmHash != itemHash(). Stepping up.";
        }
    }
}

void EditorView::onTextDeleteRequested(QString _itmHash) {
    foreach(TextItem *_itm, m_lTextItems) {
        if (_itm->itemHash() == _itmHash) {
            LOG(DEBUG) << "(editor_view:onTextDeleteRequested) Found item on stack.";
            this->scene()->removeItem(_itm);
            m_lTextItems.removeAll(_itm);
            m_lItemsHash.removeAll(_itmHash);
        } else {
            LOG(DEBUG) << "(editor_view:onTextDeleteRequested) _itmHash != itemHash(). Stepping up.";
        }
    }
}

void EditorView::onEmojiDeleteRequested(QString _itmHash) {
    foreach(EmojiItem *_itm, m_lSvgItems) {
        if (_itm->itemHash() == _itmHash) {
            LOG(DEBUG) << "(editor_view:onEmojiDeleteRequested) Found item on stack.";
            this->scene()->removeItem(_itm);
            m_lSvgItems.removeAll(_itm);
            m_lItemsHash.removeAll(_itmHash);
        } else {
            LOG(DEBUG) << "(editor_view:onEmojiDeleteRequested) _itmHash != itemHash(). Stepping up.";
        }
    }
}

void EditorView::onResizerRequested(BaseItem *_itm) {
    if (!m_resizeItem)
        m_resizeItem = new GraphicsItemResizer;
    if (m_resizerItemConnected) {
        m_resizeItem->disconnect();
        m_resizerItemConnected = false;
    }
    m_resizeItem->setParentItem(_itm->innerItem());
    m_resizeItem->setTargetSize(_itm->boundingRectSize());
    m_resizeItem->show();
    m_resizerItemConnected = connect(m_resizeItem, &GraphicsItemResizer::targetRectChanged, _itm, &BaseItem::onResize);
    connect(m_resizeItem, &GraphicsItemResizer::resizerLostFocus, this, &EditorView::onItemLostFocus);
}

void EditorView::onItemLostFocus() {
    if (m_resizeItem)
        m_resizeItem->hide();
}

void EditorView::imageFromRectRequested(const QRect &_rect, const QPointF &_from, const QPointF &_to) {
    // todo!
    LOG(DEBUG) << "Scene rect: " << this->scene()->sceneRect().width() << ";" << this->scene()->sceneRect().height();
    LOG(DEBUG) << "Rb rect: " << _rect.width() << ";" << _rect.height();
    LOG(DEBUG) << "Rb x, y: " << _from.x() << "/" << _from.y() << ";" << _to.x() << "/" << _to.y();

    QImage _img(_rect.size(), QImage::Format_ARGB32);
    QPainter _p(&_img);
    this->scene()->render(&_p, _img.rect(), _rect);

    _img.save("Z:\\Documents\\test.png");
}

void EditorView::addItem(int _id) {
    switch (_id) {
        case 0: /* rect */ {
            RectItem *_rectItem = new RectItem;
            connect(_rectItem, &RectItem::itemEditRequested, this, &EditorView::onItemEditRequested, Qt::UniqueConnection);
            connect(_rectItem, &RectItem::itemDeleteRequested, this, &EditorView::onItemDeleteRequested, Qt::UniqueConnection);
            connect(_rectItem, &RectItem::resizerRequested, this, &EditorView::onResizerRequested, Qt::UniqueConnection);
            m_lItemsHash.append(_rectItem->itemHash());
            m_lBaseItems.append(_rectItem);
            this->scene()->addItem(_rectItem);
        } break;
        case 1: /* circle */ {
            CircleItem *_circleItem = new CircleItem;
            connect(_circleItem, &CircleItem::itemEditRequested, this, &EditorView::onItemEditRequested, Qt::UniqueConnection);
            connect(_circleItem, &CircleItem::itemDeleteRequested, this, &EditorView::onItemDeleteRequested, Qt::UniqueConnection);
            connect(_circleItem, &CircleItem::resizerRequested, this, &EditorView::onResizerRequested, Qt::UniqueConnection);
            m_lItemsHash.append(_circleItem->itemHash());
            m_lBaseItems.append(_circleItem);
            this->scene()->addItem(_circleItem);
        } break;
        case 2: /* text */ {
            TextItem *_textItem = new TextItem(nullptr); // todo: is this ok? I dont think so..
            connect(_textItem, &TextItem::textEditRequested, this, &EditorView::onTextEditRequested, Qt::UniqueConnection);
            connect(_textItem, &TextItem::textDeleteRequested, this, &EditorView::onTextDeleteRequested, Qt::UniqueConnection);
            m_lItemsHash.append(_textItem->itemHash());
            m_lTextItems.append(_textItem);
            this->scene()->addItem(_textItem);
        } break;
        case 3: /* marker */ {
            MarkerItem *_markerItem = new MarkerItem;
            connect(_markerItem, &MarkerItem::itemEditRequested, this, &EditorView::onItemEditRequested, Qt::UniqueConnection);
            connect(_markerItem, &MarkerItem::itemDeleteRequested, this, &EditorView::onItemDeleteRequested, Qt::UniqueConnection);
            connect(_markerItem, &MarkerItem::resizerRequested, this, &EditorView::onResizerRequested, Qt::UniqueConnection);
            m_lItemsHash.append(_markerItem->itemHash());
            m_lBaseItems.append(_markerItem);
            this->scene()->addItem(_markerItem);
        } break;
        case 4: /* distance */ {
            DistanceItem *_distanceItem = new DistanceItem;
            connect(_distanceItem, &DistanceItem::itemEditRequested, this, &EditorView::onItemEditRequested, Qt::UniqueConnection);
            connect(_distanceItem, &DistanceItem::itemDeleteRequested, this, &EditorView::onItemDeleteRequested, Qt::UniqueConnection);
            connect(_distanceItem, &DistanceItem::resizerRequested, this, &EditorView::onResizerRequested, Qt::UniqueConnection);
            m_lItemsHash.append(_distanceItem->itemHash());
            m_lBaseItems.append(_distanceItem);
            this->scene()->addItem(_distanceItem);
        } break;
    }
    LOG(DEBUG) << "(editor_view:addItem) Current item stack: " << m_lItemsHash;
}

void EditorView::addEmoji(const QString &_path) {
    EmojiItem *_emojiItem = new EmojiItem(_path);
    connect(_emojiItem, &EmojiItem::svgDeleteRequested, this, &EditorView::onEmojiDeleteRequested);
    //connect(_emojiItem, &EmojiItem::resizerRequested, this, &EditorView::onResizerRequested, Qt::UniqueConnection);
    m_lItemsHash.append(_emojiItem->itemHash());
    {
        LOG(DEBUG) << "(editor_view:addItem) Current item stack: " << m_lItemsHash;
    }
    m_lSvgItems.append(_emojiItem);
    this->scene()->addItem(_emojiItem);
}

void EditorView::saveRequested() {
    emit sendSaveFile(this->renderScene());
}

void EditorView::copyRequested() {
    emit sendCopyFile(this->renderScene());
}

void EditorView::clearViewport() {
    // delete all items from lists
    qDeleteAll(m_lBaseItems.begin(), m_lBaseItems.end());
    qDeleteAll(m_lTextItems.begin(), m_lTextItems.end());
    qDeleteAll(m_lSvgItems.begin(), m_lSvgItems.end());
    // clear references in those lists
    m_lBaseItems.clear();
    m_lTextItems.clear();
    m_lSvgItems.clear();
    // clear items hash
    m_lItemsHash.clear();
    this->scene()->clear();
}

} // namespace
