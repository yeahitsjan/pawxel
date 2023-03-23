// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "platform.h"
#include "easylogging++.h"
#include "editor_window.h"

#include <QtGlobal>

#include <QSize>
#include <QHBoxLayout>
#include <QFont>

#include "application.h"
#include <QRect>
#include <QDesktopWidget>
#include <QStyle>

#include <Utils>
#include <FramelessWidgetsHelper>

namespace pawxel {

ColorInfoWidget::ColorInfoWidget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *_widgetLayout = new QHBoxLayout(this);
    _widgetLayout->setMargin(2);
    _widgetLayout->setSpacing(6);

    m_colorBtn = new QPushButton(this);
    m_colorBtn->setFixedSize(QSize(16, 16));
    _widgetLayout->addWidget(m_colorBtn);

    QVBoxLayout *_infoLayout = new QVBoxLayout(this);
    _infoLayout->setSpacing(0);
    _infoLayout->setMargin(0);

    m_colorLbl = new QLabel(this);
    m_colorLbl->setObjectName("InfoWidget_Label");
    m_colorLbl->setFont(QFont(APP_FONT_MID, 9));

    QLabel *_infoLbl = new QLabel(tr("Alt to copy"), this);
    _infoLbl->setObjectName("InfoWidget_Descr");
    _infoLbl->setFont(QFont(APP_FONT_MID, 8));

    _infoLayout->addWidget(m_colorLbl);
    _infoLayout->addWidget(_infoLbl);
    _widgetLayout->addLayout(_infoLayout);
    
    this->setLayout(_widgetLayout);
}

ColorInfoWidget::~ColorInfoWidget() {
}

void ColorInfoWidget::onPixelColorChanged(QRgb _val) {
    QColor _color = QColor(_val);
    // Set the button's background color to the current color of the pixel.
    m_colorBtn->setStyleSheet("background-color: " + _color.name() + ";");
    m_colorLbl->setText(_color.name());
}

ImageSizeWidget::ImageSizeWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *_widgetLayout = new QVBoxLayout(this);
    _widgetLayout->setMargin(2);
    _widgetLayout->setSpacing(0);

    m_imageSizeLbl = new QLabel(this);
    m_imageSizeLbl->setObjectName("InfoWidget_Label");
    m_imageSizeLbl->setFont(QFont(APP_FONT_MID, 9));

    QLabel *_infoLbl = new QLabel(tr("Image size"), this);
    _infoLbl->setObjectName("InfoWidget_Descr");
    _infoLbl->setFont(QFont(APP_FONT_MID, 8));

    _widgetLayout->addWidget(m_imageSizeLbl);
    _widgetLayout->addWidget(_infoLbl);
    this->setLayout(_widgetLayout);
}

ImageSizeWidget::~ImageSizeWidget() {
}

void ImageSizeWidget::onNewPix(QPixmap _pix) {
    QString _w = QString::number(_pix.size().width());
    QString _h = QString::number(_pix.size().height());
    m_imageSizeLbl->setText(_w + "x" + _h);
}

EditorWindow::EditorWindow(bool _autoSave, QColor _accentColor, bool _isDarkMode, QWidget *parent) : m_autoSave(_autoSave), m_accentColor(_accentColor), m_isDarkMode(_isDarkMode), FramelessMainWindow(parent) {
    this->resize(QSize(815, 555)); // initial base size
    // Center the window on screen
    this->setGeometry(QStyle::alignedRect(
        Qt::LeftToRight,
        Qt::AlignCenter,
        this->size(),
        PwxApp->desktop()->availableGeometry()
    ));

    this->setAttribute(Qt::WA_DeleteOnClose);

    if (!m_editorView)
        m_editorView = new EditorView(m_isDarkMode, this);
    connect(this, &EditorWindow::sendToEditor, m_editorView, &EditorView::gotPix);
    connect(this, &EditorWindow::saveToDiskRequested, m_editorView, &EditorView::saveRequested);
    connect(this, &EditorWindow::copyToClipboardRequested, m_editorView, &EditorView::copyRequested);
    connect(this, &EditorWindow::addItemToScene, m_editorView, &EditorView::addItem);
    connect(this, &EditorWindow::addEmojiToScene, m_editorView, &EditorView::addEmoji);
    connect(this, &EditorWindow::getImageFromRect, m_editorView, &EditorView::imageFromRectRequested);
    connect(this, &EditorWindow::clearViewportOnClose, m_editorView, &EditorView::clearViewport);
    connect(m_editorView, &EditorView::sendSaveFile, this, &EditorWindow::onSave);
    connect(m_editorView, &EditorView::sendCopyFile, this, &EditorWindow::onCopy);
    connect(m_editorView, &EditorView::mouseDown, this, &EditorWindow::onEditorMouseDown);
    connect(m_editorView, &EditorView::mouseRelease, this, &EditorWindow::onEditorMouseRelease);
    connect(m_editorView, &EditorView::rubberBandChanged, this, &EditorWindow::onBlurSelection);

    if (!m_layout)
        m_layout = new QVBoxLayout;
    m_layout->setSpacing(0);
    m_layout->setMargin(0);

    if (!m_spaceBar)
        m_spaceBar = new QToolBar;
    // disable toolbar-like behavior
    m_spaceBar->setFloatable(false);
    m_spaceBar->setMovable(false);
    m_spaceBar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    // click-through
    m_spaceBar->setAttribute(Qt::WA_TranslucentBackground, true);
    m_spaceBar->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    if(!m_titleBarHolder)
        m_titleBarHolder = new QWidget(this);
    m_titleBarHolder->setObjectName("TitleBar");
    m_titleBarHolder->setToolTip(tr("Click to drag the window"));
    m_titleBarHolder->setFixedHeight(38);

    // Create window specific buttons
    m_lWindowBtns.append( m_minimizeBtn = new QPushButton(MIN_CODEPOINT, m_titleBarHolder) );
    m_minimizeBtn->setToolTip(tr("Minimize"));
    connect(m_minimizeBtn, &QPushButton::clicked, this, &FramelessMainWindow::showMinimized);
    m_lWindowBtns.append( m_closeBtn = new QPushButton(CLO_CODEPOINT, m_titleBarHolder) );
    m_closeBtn->setToolTip(tr("Close"));
    connect(m_closeBtn, &QPushButton::clicked, this, &FramelessMainWindow::close);
    foreach (QPushButton *_btn, m_lWindowBtns) {
        _btn->setFont(QFont(TB_FONT, TB_SIZE));
        _btn->setObjectName("WindowButtons");
        _btn->setFixedSize(QSize(38, 38));
    }

    QHBoxLayout *_titleBarLayout = new QHBoxLayout;
    _titleBarLayout->setSpacing(0);
    _titleBarLayout->setMargin(0);

    _titleBarLayout->addWidget(m_spaceBar);
    _titleBarLayout->addStretch();
    _titleBarLayout->addWidget(m_minimizeBtn);
    _titleBarLayout->addWidget(m_closeBtn);
    m_titleBarHolder->setLayout(_titleBarLayout);

    FramelessWidgetsHelper *_helper = FramelessWidgetsHelper::get(this);
    _helper->setHitTestVisible(m_spaceBar);
#if defined(Q_OS_WIN)
    _helper->setTitleBarWidget(m_titleBarHolder);
#endif
    _helper->setHitTestVisible(m_minimizeBtn);
    _helper->setHitTestVisible(m_closeBtn);
    
    m_layout->addWidget(m_titleBarHolder);

    if (!m_editorLayout)
        m_editorLayout = new QHBoxLayout;
    m_editorLayout->setSpacing(0);
    m_editorLayout->setContentsMargins(6, 0, 0, 0);

    QVBoxLayout *_centerBar = new QVBoxLayout;
    QWidget *_spaceTop = new QWidget;
    _spaceTop->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    // This feels very stupid but I still don't get, what Qt's problem is here. We make the width
    // of the non-visible widget as small as possible and the user can now drag fully around at the top
    // area.
    _spaceTop->setFixedWidth(1);
    _centerBar->addWidget(_spaceTop);
    if (!m_actionBar)
        m_actionBar = new QToolBar(this);
    {
        m_actionBarEffect = new QGraphicsOpacityEffect(m_actionBar);
        m_actionBarEffect->setOpacity(1.0);
        m_actionBar->setGraphicsEffect(m_actionBarEffect);
        // ---
        m_animActionBar = new QPropertyAnimation(m_actionBarEffect, "opacity");
        // ---
        m_actionBar->setFloatable(false);
        m_actionBar->setMovable(false);
        // ---
        m_actionBar->setOrientation(Qt::Vertical);
        m_actionBar->setObjectName("ActionBar");
    }
    _centerBar->addWidget(m_actionBar);
    QWidget *_spaceBottom = new QWidget;
    _spaceBottom->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    _spaceBottom->setFixedWidth(1); // same as _spaceTop
    _centerBar->addWidget(_spaceBottom);
    m_editorLayout->addLayout(_centerBar);
    // add actions
    m_lActionBtns.append( m_actionCopyBtn = new QPushButton("\ue173", m_actionBar) );
    {
        m_actionCopyBtn->setToolTip(tr("Copy"));
        connect(m_actionCopyBtn, &QPushButton::clicked, this, [=]() {
            emit copyToClipboardRequested();
        });
    }
    m_lActionBtns.append( m_actionSaveBtn = new QPushButton("\ue161", m_actionBar) );
    {
        m_actionSaveBtn->setToolTip(tr("Save"));
        connect(m_actionSaveBtn, &QPushButton::clicked, this, [=]() {
            emit saveToDiskRequested();
        });
    }
    m_lActionBtns.append( m_actionTextBtn = new QPushButton("\ue264", m_actionBar) );
    {
        m_actionTextBtn->setToolTip(tr("Add Text"));
        connect(m_actionTextBtn, &QPushButton::clicked, this, [=]() {
            emit addItemToScene(2); // 2 = text
        });
    }
    m_lActionBtns.append( m_actionInsertEmojiBtn = new QPushButton("\ue24e", m_actionBar) );
    {
        m_actionInsertEmojiBtn->setToolTip(tr("Add Emoji"));
        connect(m_actionInsertEmojiBtn, &QPushButton::clicked, this, [=]() {
            if (!m_emojiWidget)
                m_emojiWidget = new EmojiWidget(m_accentColor, m_isDarkMode);
            connect(m_emojiWidget, &EmojiWidget::emojiRequested, this, &EditorWindow::onEmojiRequested, Qt::UniqueConnection);
            m_emojiWidget->show();
            m_emojiWidget->move(QCursor::pos());
        });
    }
    m_lActionBtns.append( m_pixelDistanceBtn = new QPushButton("\uea49", m_actionBar) );
    {
        m_pixelDistanceBtn->setToolTip(tr("Measure distance"));
        connect(m_pixelDistanceBtn, &QPushButton::clicked, this, [=]() {
            emit addItemToScene(4); // 4 = distance
        });
    }
    m_lActionBtns.append( m_actionSquareBtn = new QPushButton("\ue3c6", m_actionBar) );
    {
        m_actionSquareBtn->setToolTip(tr("Rectangle"));
        connect(m_actionSquareBtn, &QPushButton::clicked, this, [=]() {
            emit addItemToScene(0); // 0 = rectangle
        });
    }
#if defined(PAWXEL_EXPERIMENTAL)
    m_lActionBtns.append( m_actionBlurBtn = new QPushButton("\ue3a5", m_actionBar) );
    {
        m_actionBlurBtn->setToolTip(tr("Blur Tool"));
        connect(m_actionBlurBtn, &QPushButton::clicked, this, [=]() {
            m_editorView->setDragMode(QGraphicsView::RubberBandDrag);
        });
    }
#endif
    m_lActionBtns.append( m_actionMenuBtn = new QPushButton("\ueae1", m_actionBar) );
    foreach (QPushButton *_btn, m_lActionBtns) {
        _btn->setFont(QFont("Material Icons Outlined", 16));
        _btn->setObjectName("ActionButtons");
        _btn->setIconSize(QSize(22, 22));
        _btn->setFixedSize(QSize(36, 36));
    }

    m_actionBar->addWidget(m_actionCopyBtn);
    m_actionBar->addWidget(m_actionSaveBtn);
    m_actionBar->addWidget(m_actionTextBtn);
    m_actionBar->addWidget(m_actionInsertEmojiBtn);
    m_actionBar->addWidget(m_pixelDistanceBtn);
    m_actionBar->addWidget(m_actionSquareBtn);
    //m_actionBar->addWidget(m_actionBlurBtn);
    m_actionBar->addWidget(m_actionMenuBtn);
    {
        if (!m_actionMenu)
            m_actionMenu = new QMenu(m_actionMenuBtn);
        m_actionMenu->setPalette(PWX_appPalette(m_accentColor, m_isDarkMode));
        m_actionMenu->setFont(QFont(APP_FONT_MID, APP_FONT_SIZE));

        m_actionCircleTool = new QAction(tr("Circle"), m_actionMenuBtn);
        {
            connect(m_actionCircleTool, &QAction::triggered, this, [=]() {
                emit addItemToScene(1); // 1 = circle
            });
        }
        m_actionMenu->addAction(m_actionCircleTool);
        m_actionMarkerTool = new QAction(tr("Marker"), m_actionMenuBtn);
        {
            connect(m_actionMarkerTool, &QAction::triggered, this, [=]() {
                emit addItemToScene(3); // 3 = marker
            });
        }
        m_actionMenu->addAction(m_actionMarkerTool);
        m_actionMenu->addSeparator();
        m_actionPreferences = new QAction(tr("Preferences"), m_actionMenuBtn);
        connect(m_actionPreferences, &QAction::triggered, this, [=]() {
            emit preferencesWindowRequested();
        });
        m_actionMenu->addAction(m_actionPreferences);
        m_actionAbout = new QAction(tr("About"), m_actionMenuBtn);
        connect(m_actionAbout, &QAction::triggered, this, [=]() {
            emit aboutWindowRequested();
        });
        m_actionMenu->addAction(m_actionAbout);
        
        m_actionMenuBtn->setMenu(m_actionMenu);
    }

    if (!m_passthroughWidget)
        m_passthroughWidget = new QWidget(m_editorView);
    m_passthroughWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Make the passthrough widget "invisible", so the underlying pixmap on the graphics view
    // can be edited.
    m_passthroughWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    m_passthroughWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_editorLayout->addWidget(m_passthroughWidget);

    m_layout->addLayout(m_editorLayout);

    if (!m_bottomBar)
        m_bottomBar = new QToolBar(this);
    m_bottomBar->setObjectName("BottomBar");
    // disable toolbar behavior
    m_bottomBar->setFloatable(false);
    m_bottomBar->setMovable(false);
    // click-through
    m_bottomBar->setFixedHeight(38);
    // todo: this isn't quite working well at the moment. If the image gets zoomed in and moves
    // behinde the bottom bar, we need to click first to be able to send the informations to
    // the ColorInfoWidget. The problem here is that if we click, we kinda "loose focus" of our
    // QGraphicsView and cannot send any more informations to the widget.
    m_bottomBar->setAttribute(Qt::WA_TranslucentBackground, true);
    m_bottomBar->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    {
        if (!m_colorInfoWidget)
            m_colorInfoWidget = new ColorInfoWidget(m_bottomBar);
        // ---
        m_colorInfoEffect = new QGraphicsOpacityEffect(m_colorInfoWidget);
        m_colorInfoEffect->setOpacity(1.0);
        m_colorInfoWidget->setGraphicsEffect(m_colorInfoEffect);
        // ---
        m_animColorInfo = new QPropertyAnimation(m_colorInfoEffect, "opacity");
        connect(m_editorView, &EditorView::feedColorInfoWidget, m_colorInfoWidget, &ColorInfoWidget::onPixelColorChanged);
        // ----------------------------------------
        if (!m_imageSizeWidget)
            m_imageSizeWidget = new ImageSizeWidget(m_bottomBar);
        // ---
        m_imageSizeEffect = new QGraphicsOpacityEffect(m_imageSizeWidget);
        m_imageSizeEffect->setOpacity(1.0);
        m_imageSizeWidget->setGraphicsEffect(m_imageSizeEffect);
        // ---
        m_animImageSize = new QPropertyAnimation(m_imageSizeEffect, "opacity");
        // ---
        connect(this, &EditorWindow::sendToImageSizeWidget, m_imageSizeWidget, &ImageSizeWidget::onNewPix);
        
        QWidget *_spaceBottomBar = new QWidget;
        _spaceBottomBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        // ---
        QLabel *_appNameLbl = new QLabel("pawxel", m_bottomBar);
#if defined(Q_OS_WIN)
        _appNameLbl->setFont(QFont("Josefin Sans Bold", 12));
#else
        _appNameLbl->setFont(QFont(APP_FONT_MID, 12));
#endif
        _appNameLbl->setObjectName("AppNameLabel");

        m_bottomBar->addSeparator(); // hacky but works
        m_bottomBar->addWidget(m_colorInfoWidget);
        // do DPI-aware space between those widgets
        m_bottomBar->addSeparator();
        m_bottomBar->addSeparator();
        m_bottomBar->addWidget(m_imageSizeWidget);
        m_bottomBar->addWidget(_spaceBottomBar);
        m_bottomBar->addWidget(_appNameLbl);
    }
    m_layout->addWidget(m_bottomBar);

    m_editorView->setLayout(m_layout);
    this->setCentralWidget(m_editorView);
}

EditorWindow::~EditorWindow() {
}

void EditorWindow::closeEvent(QCloseEvent *ev) {
    emit holdLastCapture(m_pix);
    emit clearViewportOnClose();
    FramelessMainWindow::closeEvent(ev);
}

void EditorWindow::onNewPix(QPixmap _pix) {
    emit sendToImageSizeWidget(_pix);
    m_pix = _pix;
    // Get the current available screen size.
    QRect currentScreenSz = PwxApp->desktop()->screenGeometry();
    if (m_pix.width() >= currentScreenSz.width() - 76 ||
        m_pix.height() >= currentScreenSz.height() - 76 || 
        m_pix.width() <= 815 ||
        m_pix.height() <= 555) {
        // ???
    } else {
        // Resize the window if the pixmap's size is not greater than the screen
        // size.
        this->resize(QSize(m_pix.width() + 76, m_pix.height() + 76));
        // re-center window on screen
        this->setGeometry(QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            PwxApp->desktop()->availableGeometry()
        ));
    }
    // todo: delete old picture from scene on close!
    if (m_editorView)
        emit sendToEditor(_pix);
}

void EditorWindow::onSave(QPixmap _pix) {
    if (m_autoSave && !PwxApp->preferences()->screenshotsFolder().startsWith("$EMPTY"))
        emit autoSaveToDisk(_pix);
    else
        emit saveToDisk(_pix);
}

void EditorWindow::onCopy(QPixmap _pix) {
    emit copyToClipboard(_pix);
}

/** Animations goes here! **/
/*
    You cannot tell me that this is currently the best way to do it. But as I see there
    is no possibility to apply the same animation to multiple widgets at the same time...
    TODO@jan
*/
void EditorWindow::onEditorMouseDown() {
    // Action bar
    m_animActionBar->setStartValue(1.0);
    m_animActionBar->setEndValue(0.2);
    m_animActionBar->setDuration(100);
    m_animActionBar->start();

    // Color Info widget
    m_animColorInfo->setStartValue(1.0);
    m_animColorInfo->setEndValue(0.2);
    m_animColorInfo->setDuration(100);
    m_animColorInfo->start();

    // Image size widget
    m_animImageSize->setStartValue(1.0);
    m_animImageSize->setEndValue(0.2);
    m_animImageSize->setDuration(100);
    m_animImageSize->start();
}

void EditorWindow::onEditorMouseRelease() {
    // Action bar
    m_animActionBar->setStartValue(0.2);
    m_animActionBar->setEndValue(1.0);
    m_animActionBar->setDuration(100);
    m_animActionBar->start();

    // Color Info widget
    m_animColorInfo->setStartValue(0.2);
    m_animColorInfo->setEndValue(1.0);
    m_animColorInfo->setDuration(100);
    m_animColorInfo->start();

    // Image size widget
    m_animImageSize->setStartValue(0.2);
    m_animImageSize->setEndValue(1.0);
    m_animImageSize->setDuration(100);
    m_animImageSize->start();
}

void EditorWindow::onBlurSelection(QRect _rect, QPointF _from, QPointF _to) {
    // TODO: We should replace this mechanism in the future. Although it's fast enough
    // for most of the people, my brain just tells me that it isn't the fastest and efficient
    // way possible. Probably a QStack or some other first-in-last-out thing could help out here.
    // In theory we do not need to store ALL values, just the last one until it gets NULLed.
    if (!_rect.isNull()) {
        m_vSelectionValues.append(m_editorView->rubberBandRect());
        m_vFromSelectionValues.append(_from);
        m_vToSelectionValues.append(_to);
    } else {
        auto _last = m_vSelectionValues.last();
        auto _lastFrom = m_vFromSelectionValues.last();
        auto _lastTo = m_vToSelectionValues.last();

        m_vSelectionValues.clear(); // do a squeeze?
        m_vFromSelectionValues.clear();
        m_vToSelectionValues.clear();

        emit getImageFromRect(_last, _lastFrom, _lastTo);
    }
}

void EditorWindow::onEmojiRequested(const QString &_fpath) {
    emit addEmojiToScene(_fpath);
}

} // namespace
