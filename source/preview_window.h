// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <FramelessWidget>
#include <QObject>
#include <QWidget>

#include <QPointer>

#include <QLabel>
#include <QResizeEvent>
#include <QPixmap>

#include <QVBoxLayout>
#include <QPushButton>
#include <QToolBar>

FRAMELESSHELPER_USE_NAMESPACE

namespace pawxel {

class ARPixPreviewLabel : public QLabel {
    Q_OBJECT
public:
    explicit ARPixPreviewLabel(QWidget *parent = nullptr);
    ~ARPixPreviewLabel();

protected:
    void resizeEvent(QResizeEvent *ev) override;

private:
    void updateMargins();

    int m_pixW = 0;
    int m_pixH = 0;

public slots:
    void setPixmap(const QPixmap &_pix);
};

class LittlePreviewWindow : public FramelessWidget {
    Q_OBJECT
public:
    explicit LittlePreviewWindow(bool _autoSave, bool _isDarkMode, QWidget *parent = nullptr);
    ~LittlePreviewWindow();

protected:
    QPointer<QVBoxLayout> m_windowLayout;
    QPointer<ARPixPreviewLabel> m_pixPreview;

    QPointer<QToolBar> m_actionBar;

    QPixmap m_pix;

    void keyPressEvent(QKeyEvent *ev) override;

private:
    QPushButton *m_editBtn;
    QPushButton *m_copyBtn;
    QPushButton *m_saveBtn;
    // -- space
    QPushButton *m_retakeBtn;
    QPushButton *m_trashBtn;
    QList<QPushButton*> m_lActionBtns;

    bool m_isDarkMode;
    bool m_autoSave;

public slots:
    void onNewPix(QPixmap _pix);

signals:
    // User wants to edit the pixmap.
    void userWantsEdit(QPixmap _pix);
    // Save the pixmap to the disk.
    void saveToDisk(QPixmap _pix);
    // Auto-save the pixmap to the disk.
    void autoSaveToDisk(QPixmap _pix);
    // Copy the pixmap to clipboard (directly).
    void copyToClipboard(QPixmap _pix);
    // Take a new screenshot.
    void retake();
};

} // namespace