// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <QApplication>
#if defined(Q_OS_WINDOWS)
#   include <SingleApplication.h>
#else
#   include "singleapplication.h" // strange habit, probably a wrong Qmake setup on my Linux devenv
#endif
#include <QObject>
#include <QWidget>

#include <QPointer>

#include <QPixmap>
#include <QPoint>
#include <QColor>

#include <QHotkey>

#include <FHNotifier/FHMessageBox.h>

#include "systray.h"
#include "sniparea.h"
#include "preview_window.h"
#include "select_window.h"
#include "editor_window.h"
#include "preferences.h"
#include "preferences_window.h"
#include "about_window.h"

using namespace pawxel;

class PawxelApp : public SingleApplication {
    Q_OBJECT
public:
    explicit PawxelApp(int &argc, char **argv);
    ~PawxelApp();

    Preferences* preferences() { return m_preferences; }

protected:
    QPointer<Preferences> m_preferences;

    QPointer<SysTray> m_sysTray;
    QPointer<SnipArea> m_snipArea;
    QPointer<LittlePreviewWindow> m_previewWindow;
    QPointer<SelectWindow> m_selectionWindow;
    QPointer<EditorWindow> m_shotEditor;
    
    QPointer<PreferencesWindow> m_prefsWindow;
    QPointer<FHMessageBox> m_restartMsgBox;
    QPointer<AboutWindow> m_aboutWindow;

    bool m_shouldAppsUseDarkMode;
    QColor m_accentColor;

    QHotkey *m_gFullscreenHotkey;
    QHotkey *m_gSnipHotkey;

private:
    QWidget *m_parent;
    QString m_lastCapturePath;

    bool m_perfMode;
    bool m_autoSave;

    void savePix(const QString &_fullQualifiedPath, QPixmap _pix);

public slots:
    void onQuitRequested(bool _finalQuit);

    // Slots related to restore the last capture.
    void onHoldLastCapture(QPixmap _pix);
    void onRestoreCaptureRequested();

    // Slots related to choose a screenshot.
    void onShotSelectionRequested(QList<QPixmap> _lPixs);

    // Slots related to create a screenshot.
    void onFullscreenShotRequested();
    void onSnipAreaRequested();
    void onPreviewWindowRequested(QPixmap _pix, QPoint _atMousePos);
    void onShotEditorRequested(QPixmap _pix);

    void onPreferencesWindowRequested();
    void onPrefChangeRestartRequired();
    void onAboutWindowRequested();

    // Access to the hard disk to save the screenshot. Creates a file dialog.
    void savePixToDisk(QPixmap _pix);
    void autoSavePixToDisk(QPixmap _pix);

    // Access to global clipboard
    void copyPixToClipboard(QPixmap _pix);

    // Change the registered hotkeys.
    void onHotkeyChanged(int _id, const QString &_newHotkey);

    // Change autostart mode.
    void onAutostartChanged(bool _enabled);

signals:
    // Emitted when fullscreen screenshot was captured.
    void fullScreenCaptureFinished(QPixmap _pix);
    void multiFullScreenCaptureFinished(QList<QPixmap> m_lPixs);

    // Signals to send to the screenshot-related windows.
    void feedPreviewWindow(QPixmap _pix);
    void feedEditorWindow(QPixmap _pix);
    void feedSelectWindow(QList<QPixmap> _lPixs);
    
    // Signals related to restore the capture.
    void restore(QPixmap _pix);
};

extern PawxelApp *PwxApp;