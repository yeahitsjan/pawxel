// Pawxel - Copyright (c) 2022-2024 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <FramelessMainWindow>
#include <QObject>
#include <QWidget>

#include <QPointer>
#include <QVector>
#include <QList>

#include <QVBoxLayout>
#include <QToolBar>
#include <QPushButton>
#include <QLabel>

#include <QMenu>
#include <QAction>

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include <QPixmap>
#include <QColor>

#include <QRect>
#include <QPointF>

#include "editor_view.h"
#include "emoji_widget.h"

FRAMELESSHELPER_USE_NAMESPACE

namespace pawxel {

class ColorInfoWidget : public QWidget {
    Q_OBJECT
public:
    explicit ColorInfoWidget(QWidget *parent = nullptr);
    ~ColorInfoWidget();

protected:
    QPushButton *m_colorBtn;
    QLabel *m_colorLbl;

public slots:
    void onPixelColorChanged(QRgb _val);
};

class ImageSizeWidget : public QWidget {
    Q_OBJECT
public:
    explicit ImageSizeWidget(QWidget *parent = nullptr);
    ~ImageSizeWidget();

protected:
    QLabel *m_imageSizeLbl;

public slots:
    void onNewPix(QPixmap _pix);
};

class EditorWindow : public FramelessMainWindow {
    Q_OBJECT
public:
    explicit EditorWindow(bool _autoSave, QColor _accentColor, bool _isDarkMode, QWidget *parent = nullptr);
    ~EditorWindow();

protected:
    QPointer<EditorView> m_editorView;
    QPointer<QVBoxLayout> m_layout;

    QPointer<QWidget> m_titleBarHolder;
    QPointer<QToolBar> m_spaceBar;

    QPointer<ColorInfoWidget> m_colorInfoWidget;
    QGraphicsOpacityEffect *m_colorInfoEffect;
    QPropertyAnimation *m_animColorInfo;
    
    QPointer<ImageSizeWidget> m_imageSizeWidget;
    QGraphicsOpacityEffect *m_imageSizeEffect;
    QPropertyAnimation *m_animImageSize;

    QPointer<QHBoxLayout> m_editorLayout;
    
    QPointer<QToolBar> m_actionBar;
    QGraphicsOpacityEffect *m_actionBarEffect;
    QPropertyAnimation *m_animActionBar;

    QPointer<QMenu> m_actionMenu;

    QPointer<EmojiWidget> m_emojiWidget;
    
    QPointer<QWidget> m_passthroughWidget;

    QPointer<QToolBar> m_bottomBar;

    // Events
    void closeEvent(QCloseEvent *ev) override;

private:
    QPushButton *m_actionCopyBtn;
    QPushButton *m_actionSaveBtn;
    QPushButton *m_actionTextBtn;
    QPushButton *m_actionInsertEmojiBtn;
    QPushButton *m_pixelDistanceBtn;
    QPushButton *m_actionSquareBtn;
    QPushButton *m_actionBlurBtn;
    QPushButton *m_actionMenuBtn;
    QList<QPushButton*> m_lActionBtns;

    // ActionMenu
    QAction *m_actionCircleTool;
    QAction *m_actionMarkerTool;
    QAction *m_actionPreferences;
    QAction *m_actionAbout;

    QPushButton *m_minimizeBtn;
    QPushButton *m_closeBtn;
    QList<QPushButton*> m_lWindowBtns;

    QPixmap m_pix;

    // Required to blur a specific section (BlurItem)
    QVector<QRect> m_vSelectionValues;
    QVector<QPointF> m_vFromSelectionValues;
    QVector<QPointF> m_vToSelectionValues;

    QColor m_accentColor;
    bool m_isDarkMode;
    bool m_autoSave;

public slots:
    void onNewPix(QPixmap _pix);
    void onSave(QPixmap _pix);
    void onCopy(QPixmap _pix);

    void onEditorMouseDown();
    void onEditorMouseRelease();

    void onBlurSelection(QRect _rect, QPointF _from, QPointF _to);

    void onEmojiRequested(const QString &_fpath);

signals:
    // Send to internal QGraphicsView (emitted on load), for further editing.
    void sendToEditor(QPixmap _pix);
    // Send pixmap to determine image size.
    void sendToImageSizeWidget(QPixmap _pix);

    // The last capture is hold, elements from viewport removed.
    void clearViewportOnClose();

    // Save the edited screenshot to disk.
    void saveToDiskRequested();
    void saveToDisk(QPixmap _pix);
    void autoSaveToDisk(QPixmap _pix);

    // Copy pixmap to clipboard.
    void copyToClipboardRequested();
    void copyToClipboard(QPixmap _pix);

    // Send item id to be added to the editor.
    void addItemToScene(int _id);
    void addEmojiToScene(QString _path);

    void getImageFromRect(const QRect _rect, const QPointF &_from, const QPointF &_to);

    // Window requests.
    void aboutWindowRequested();
    void preferencesWindowRequested();
};

} // namespace
