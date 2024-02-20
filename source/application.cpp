// Pawxel - Copyright (c) 2022-2024 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "easylogging++.h"
#include "platform.h"
#include "application.h"

#include <Utils>
#include <framelessconfig_p.h>

#include "phantomstyle.h"
#include <QStyle>

#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>

#include <QSettings>
#include <QDateTime>

#include <QFont>
#include <QClipboard>
#include <QStandardPaths>
#include <QFileDialog>

#include <QDesktopWidget>
#include <QScreen>

PawxelApp::PawxelApp(int &argc, char **argv) : SingleApplication(argc, argv) {
    LOG(INFO) << "Pawxel started";
    if (!QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.pawxel/").exists()) {
        if (QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.pawxel/"))
            LOG(DEBUG) << "(application::PawxelApp) Created Pawxel preferences directory at ~/.pawxel.";
        else
            LOG(ERROR) << "(application::PawxelApp) Could not create preferences directory. Preferences cannot be saved this session!";
    } else {
        LOG(DEBUG) << "(application::PawxelApp) Found preferences directory at ~/.pawxel.";
    }
    if (!m_preferences)
        m_preferences = new Preferences;
    connect(m_preferences, &Preferences::restartToApply, this, &PawxelApp::onPrefChangeRestartRequired, Qt::UniqueConnection);
    connect(m_preferences, &Preferences::globalHotkeyChanged, this, &PawxelApp::onHotkeyChanged, Qt::UniqueConnection);
    connect(m_preferences, &Preferences::autostartChanged, this, &PawxelApp::onAutostartChanged, Qt::UniqueConnection);
    if (m_preferences->flush()) LOG(INFO) << "(application::PawxelApp) Preferences loaded.";
    // Are we debug yet?
    if (m_preferences->debugMode()) {
        LOG(INFO) << "(application::PawxelApp) Application entered debug mode!";
        QString _homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.pawxel";

        QFileInfo _fi(_homePath + "/debug.conf");
        if (!_fi.exists()) {
            // If the debug logging configuration wasn't loaded from resources already,
            // we'll do it here!
            QFile _resourceConf(":/resources/debug.conf");
            QString _conf;
            if (_resourceConf.open( QIODevice::ReadOnly )) {
                QTextStream rd(&_resourceConf);
                _conf = rd.readAll();
                _resourceConf.close();
            }
            _conf.replace("$LOG_FILE_PATH", _homePath + "/debug.log");
            QFile _debugConf(_homePath + "/debug.conf");
            if (_debugConf.open( QIODevice::ReadWrite)) {
                QTextStream wr(&_debugConf);
                wr << _conf;
                _debugConf.close();
            }
        }
        el::Configurations _conf(_homePath.toStdString() + "/debug.conf");
        el::Loggers::reconfigureAllLoggers(_conf);
        // Print gpu informations (SysGpuInfo is used in debug dialog).
        PWX_retrieveGpuInformations();
    }
    // Set the window icon based on user choice.
    this->setWindowIcon(QIcon(":/resources/icons/icon_" + m_preferences->appIcon() + ".svg"));
    // Check if the OS theme is light or dark, depending on that we're styling our application.
    switch (m_preferences->appTheme()) {
        case 0: {
            m_shouldAppsUseDarkMode = Utils::shouldAppsUseDarkMode();
        } break;
        case 1: {
            m_shouldAppsUseDarkMode = false;
        } break;
        case 2: {
            m_shouldAppsUseDarkMode = true;
        } break;
    }
    // On Windows, use the current accent color as highlight color for QMenu's.
#if defined(Q_OS_WINDOWS)
    m_accentColor = Utils::getDwmAccentColor();
#else
    m_accentColor = QColor(137, 156, 240);
#endif
    LOG(DEBUG) << "(application::PawxelApp) Should Pawxel use dark mode?= " << m_shouldAppsUseDarkMode;
    FramelessConfig::instance()->set(Global::Option::WindowUseRoundCorners);

    // TODO: In version 2.3.4 of FramelessHelper, this results in a white background flashing
    // before any window is displayed. We disable this for now and look into it further afterwards.
    // I even still don't know if blur is a good thing, I am personally not a fan of it.
#if PAWXEL_UNUSED
    FramelessConfig::instance()->set(Global::Option::EnableBlurBehindWindow);
#endif

    // Set application relevant informations.
    this->setApplicationVersion(PAWXEL_VERSION);
    this->setStyle(new PhantomStyle);
    this->setFont(QFont(PLATFORM_FONT, PLATFORM_FONT_SIZE));

    QString _stylesheet;
    QFile _styleFile;
    QPalette pa;
    if (!m_shouldAppsUseDarkMode) {
        _styleFile.setFileName(":/resources/stylesheets/light.style");
    }
    else {
        _styleFile.setFileName(":/resources/stylesheets/dark.style");
    }
    if (_styleFile.open( QIODevice::ReadOnly )) {
        QTextStream rd(&_styleFile);
        _stylesheet = rd.readAll();
        _styleFile.close();
    } else {
        LOG(DEBUG) << "(application::PawxelApp) Could not read stylesheet from resources.";
    }
    this->setStyleSheet(_stylesheet);

    // Check if the user has auto-save for screenshots enabled.
    if (!m_preferences->screenshotsFolder().startsWith("$EMPTY"))
        m_autoSave = true;
    else
        m_autoSave = false;

    PwxApp = this;

    // General parent widget because we have no real ui.
    m_parent = new QWidget;

    // Startup mode
    switch (m_preferences->startupMode()) {
        case 0: {
            m_perfMode = false;
        } break;
        case 1: {
            m_perfMode = true;
        } break;
    }
    if (m_perfMode) {
        this->onSnipAreaRequested();
    } else {
        if (!m_sysTray)
            m_sysTray = new SysTray(m_accentColor, m_shouldAppsUseDarkMode);
        connect(m_sysTray, &SysTray::fullScreenCaptureRequested, this, &PawxelApp::onFullscreenShotRequested);
        connect(m_sysTray, &SysTray::snipAreaRequested, this, &PawxelApp::onSnipAreaRequested);
        connect(m_sysTray, &SysTray::restoreCaptureRequested, this, &PawxelApp::onRestoreCaptureRequested);
        connect(m_sysTray, &SysTray::preferencesWindowRequested, this, &PawxelApp::onPreferencesWindowRequested);
        connect(m_sysTray, &SysTray::aboutDialogRequested, this, &PawxelApp::onAboutWindowRequested);
        connect(m_sysTray, &SysTray::quitRequested, this, &PawxelApp::onQuitRequested);
        m_sysTray->show();
        this->setQuitOnLastWindowClosed(false);
        
        // Register global hotkeys.
        m_gFullscreenHotkey = new QHotkey(QKeySequence(m_preferences->hotkeyFullscreen()), true, this);
        connect(m_gFullscreenHotkey, &QHotkey::activated, this, &PawxelApp::onFullscreenShotRequested);
        LOG(DEBUG) << "(application::PawxelApp) Fullscreen hotkey registered?=" << m_gFullscreenHotkey->isRegistered();
        m_gSnipHotkey = new QHotkey(QKeySequence(m_preferences->hotkeySnip()), true, this);
        connect(m_gSnipHotkey, &QHotkey::activated, this, &PawxelApp::onSnipAreaRequested);
        LOG(DEBUG) << "(application::PawxelApp) Snip hotkey registered?=" << m_gSnipHotkey->isRegistered();
    }
}

PawxelApp::~PawxelApp() {
    delete m_parent;
}

void PawxelApp::savePix(const QString &_fullQualifiedPath, QPixmap _pix) {
    if (!_fullQualifiedPath.isEmpty()) {
        QFile _saveFile(_fullQualifiedPath);
        if (_saveFile.open(QIODevice::WriteOnly))
            _pix.save(&_saveFile);
        else
            LOG(FATAL) << "(application:savePix) Could not write pix to file!";
        _saveFile.close();
    } else {
        LOG(ERROR) << "(application:savePix) Path to save to empty.";
    }
}

void PawxelApp::onQuitRequested(bool _finalQuit) {
    if (m_perfMode) {
        // If we are in performance mode, pawxel is closed on close directly.
        this->quit();
    } else {
        if (_finalQuit)
            this->quit();
        else
            LOG(DEBUG) << "(application:onQuitRequested) Close to systray requested.";
    }
}

void PawxelApp::onHoldLastCapture(QPixmap _pix) {
    m_lastCapturePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/__pwx_last__.png";
    LOG(DEBUG) << "(application:onHoldLastCapture) writing to: " << m_lastCapturePath;
    QFile _captureFile(m_lastCapturePath);
    if (_captureFile.open( QIODevice::WriteOnly )) {
        _pix.save(&_captureFile);
    } else {
        LOG(FATAL) << "(application:onHoldLastCapture) Could not write to temporary location";
    }
    _captureFile.close();
    _pix = QPixmap();
}

void PawxelApp::onRestoreCaptureRequested() {
    connect(this, &PawxelApp::restore, this, &PawxelApp::onShotEditorRequested, Qt::UniqueConnection);
    if (!m_lastCapturePath.isEmpty()) {
        QPixmap _lastCapture = QPixmap(m_lastCapturePath);
        emit restore(_lastCapture);
    }
    QString _lastCapturePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/__pwx_last__.png";
    QFileInfo fi(_lastCapturePath);
    if (fi.exists()) {
        m_lastCapturePath = _lastCapturePath;
        QPixmap _lastCapture = QPixmap(m_lastCapturePath);
        emit restore(_lastCapture);
    } else {
        FHMessageBox *_noLastCapture = new FHMessageBox(FHMessageBox::Ok, m_accentColor, m_shouldAppsUseDarkMode);
        _noLastCapture->setWindowTitle(tr("No last capture"));
        _noLastCapture->setMessageBoxTitle(tr("No last capture"));
        _noLastCapture->setText(tr("There is no recent capture saved. Take a screenshot first."));
        _noLastCapture->setMessageBoxIcon(FHMessageBox::Information);
        {
            _noLastCapture->titleWidget()->setFont(QFont(APP_FONT_BOLD, 9));
            _noLastCapture->textWidget()->setFont(QFont(APP_FONT, 9));
            _noLastCapture->setButtonFont(QFont(APP_FONT, 9));
        }
        connect(_noLastCapture, &FHMessageBox::firstButtonClicked, this, [=]() {
            _noLastCapture->close();
        }, Qt::UniqueConnection);
        _noLastCapture->exec();
    }
}

void PawxelApp::onShotSelectionRequested(QList<QPixmap> _lPixs) {
    LOG(DEBUG) << "(application:onShotSelectionRequested) SelectWindow requested";
    if (!m_selectionWindow)
        m_selectionWindow = new SelectWindow(m_shouldAppsUseDarkMode);
    m_selectionWindow->setObjectName("PreviewWindow"); // same styling
    connect(this, &PawxelApp::feedSelectWindow, m_selectionWindow, &SelectWindow::onMultiPixmaps, Qt::UniqueConnection);
    connect(m_selectionWindow, &SelectWindow::userSelectedScreen, this, &PawxelApp::onShotEditorRequested, Qt::UniqueConnection);
    m_selectionWindow->show();
    emit feedSelectWindow(_lPixs);
}

void PawxelApp::onFullscreenShotRequested() {
    // Skip the preview window.
    connect(this, &PawxelApp::fullScreenCaptureFinished, this, &PawxelApp::onShotEditorRequested, Qt::UniqueConnection);
    // If we have more than one screen attached, show a selection dialog.
    connect(this, &PawxelApp::multiFullScreenCaptureFinished, this, &PawxelApp::onShotSelectionRequested, Qt::UniqueConnection);

    QList<QPixmap> _lScreens;
    // auto x = this->screens().size(); why does this return 1? IMO a list which contains 1 element should start like an array = 0.
    if (this->screens().size() > 1) {
        LOG(DEBUG) << "(application:onFullscreenShotRequested) More than one screen attached.";
        for (int i = 0; i < this->screens().size(); i++) {
            auto _scr = this->screens().at(i);
            //auto _scrRect = _scr->geometry();
            _lScreens.append(_scr->grabWindow(0));
        }
        // + combined
        QRect _geom = this->desktop()->geometry();
        _lScreens.append(QPixmap::grabWindow(0, _geom.x(), _geom.y(), _geom.width(), _geom.height()));
        emit multiFullScreenCaptureFinished(_lScreens);
    } else {
        QRect _geom = this->desktop()->screenGeometry();
        QPixmap _pix = QPixmap::grabWindow(0, _geom.x(), _geom.y(), _geom.width(), _geom.height());
        emit fullScreenCaptureFinished(_pix);
    }
    _lScreens.clear();
}

void PawxelApp::onSnipAreaRequested() {
    LOG(DEBUG) << "(application:onSnipAreaRequested) SnipArea requested";
    if (!m_snipArea)
        m_snipArea = new SnipArea(m_parent);

    m_afterScreenshot = (m_preferences->afterScreenshot());
    switch (m_afterScreenshot) {
        case 0: {
            connect(m_snipArea, &SnipArea::snipped, this, &PawxelApp::onPreviewWindowRequested, Qt::UniqueConnection);
            // Create the window already here (altough we don't show it), so related libraries
            // can be loaded earlier. Possible minimal decrease in startup time?
            if (!m_previewWindow)
                m_previewWindow = new LittlePreviewWindow(m_autoSave, m_shouldAppsUseDarkMode);
            m_previewWindow->setObjectName("PreviewWindow");
            // Always make sure the editor is not visible when requesting the snip area.
            if (m_shotEditor) {
                if (m_shotEditor->isVisible())
                    m_shotEditor->close();
            }
        } break;
        case 1: {
            connect(m_snipArea, &SnipArea::snipped, this, &PawxelApp::onShotEditorRequested, Qt::UniqueConnection);
        } break;
        case 2: {
            connect(m_snipArea, &SnipArea::snipped, this, &PawxelApp::copyPixToClipboard, Qt::UniqueConnection);
        }
    }
    m_snipArea->show();
}

void PawxelApp::onPreviewWindowRequested(QPixmap _pix, QPoint _atMousePos) {
    LOG(DEBUG) << "(application:onPreviewWindowRequested) PreviewWindow requested";
    if (!m_previewWindow)
        m_previewWindow = new LittlePreviewWindow(m_autoSave, m_shouldAppsUseDarkMode);
    connect(this, &PawxelApp::feedPreviewWindow, m_previewWindow, &LittlePreviewWindow::onNewPix, Qt::UniqueConnection);
    connect(m_previewWindow, &LittlePreviewWindow::userWantsEdit, this, &PawxelApp::onShotEditorRequested, Qt::UniqueConnection);
    connect(m_previewWindow, &LittlePreviewWindow::copyToClipboard, this, &PawxelApp::copyPixToClipboard, Qt::UniqueConnection);
    connect(m_previewWindow, &LittlePreviewWindow::saveToDisk, this, &PawxelApp::savePixToDisk, Qt::UniqueConnection);
    connect(m_previewWindow, &LittlePreviewWindow::autoSaveToDisk, this, &PawxelApp::autoSavePixToDisk, Qt::UniqueConnection);
    connect(m_previewWindow, &LittlePreviewWindow::retake, this, &PawxelApp::onSnipAreaRequested, Qt::UniqueConnection);
    m_previewWindow->show();
    emit feedPreviewWindow(_pix);

    // Calculate available screen geom minus the size of the preview window.
    auto _rWidth = PwxApp->desktop()->screenGeometry().width() - 240;
    auto _rHeight = PwxApp->desktop()->screenGeometry().height() - 220;
    LOG(DEBUG) << "(application:onPreviewWindowRequested) Window would be displayed at: " << _atMousePos.x() << ", " << _atMousePos.y();
    LOG(DEBUG) << "(application:onPreviewWindowRequested) Available screen size for prev win is: " << _rWidth << ", " << _rHeight;
    m_previewWindow->move(_atMousePos); // move the preview window to last mouse postion
    /* If x or y of mouse position is greater than calculated screen size, move the window. */
    if (_atMousePos.x() >= _rWidth || _atMousePos.y() >= _rHeight) {
        /* If x of preview window is currently greater than calculated screen size */
        if (m_previewWindow->pos().x() >= _rWidth) {
            /* move window back into screen via x, ignoring y */
            m_previewWindow->move(m_previewWindow->pos().x() - 240, m_previewWindow->pos().y());
        }
        /* If y of preview window is currently greater than calculated screen size */
        if (m_previewWindow->pos().y() >= _rHeight) {
            /* move window back into screen ignoring x, via y */
            m_previewWindow->move(m_previewWindow->pos().x(), m_previewWindow->pos().y() - 220);
        }
    } else if (_atMousePos.x() < 0 || _atMousePos.y() < 0) {
        /* If x of preview window is smaller than zero */
        if (m_previewWindow->pos().x() < 0) {
            /* Move window back into screen via x, ignoring y */
            m_previewWindow->move(m_previewWindow->pos().x() + 240, m_previewWindow->pos().y());
        }
        /* If y of preview window is smaller than zero */
        if (m_previewWindow->pos().y() < 0) {
            /* Move window back into screen ignoring x, via y */
            m_previewWindow->move(m_previewWindow->pos().x(), m_previewWindow->pos().y() + 220);
        }
    }
    LOG(DEBUG) << "(application:onPreviewWindowRequested) Window pos is now: " << m_previewWindow->pos().x() << ", " << m_previewWindow->pos().y();
}

void PawxelApp::onShotEditorRequested(QPixmap _pix) {
    LOG(DEBUG) << "(application:onShotEditorRequested) ShotEditor requested";
    if (m_previewWindow)
        m_previewWindow->hide(); // no close! => https://github.com/wangwenx190/framelesshelper#use
    if (!m_shotEditor)
        m_shotEditor = new EditorWindow(m_autoSave, m_accentColor, m_shouldAppsUseDarkMode);
    connect(this, &PawxelApp::feedEditorWindow, m_shotEditor, &EditorWindow::onNewPix, Qt::UniqueConnection);
    connect(m_shotEditor, &EditorWindow::copyToClipboard, this, &PawxelApp::copyPixToClipboard, Qt::UniqueConnection);
    connect(m_shotEditor, &EditorWindow::saveToDisk, this, &PawxelApp::savePixToDisk, Qt::UniqueConnection);
    connect(m_shotEditor, &EditorWindow::autoSaveToDisk, this, &PawxelApp::autoSavePixToDisk, Qt::UniqueConnection);
    connect(m_shotEditor, &EditorWindow::holdLastCapture, this, &PawxelApp::onHoldLastCapture, Qt::UniqueConnection);
    connect(m_shotEditor, &EditorWindow::preferencesWindowRequested, this, &PawxelApp::onPreferencesWindowRequested, Qt::UniqueConnection);
    connect(m_shotEditor, &EditorWindow::aboutWindowRequested, this, &PawxelApp::onAboutWindowRequested, Qt::UniqueConnection);
    m_shotEditor->show();
    emit feedEditorWindow(_pix);
}

void PawxelApp::onPreferencesWindowRequested() {
    LOG(DEBUG) << "(application:onPreferencesWindowRequested) PreferencesWindow requested";
    if (!m_prefsWindow)
        m_prefsWindow = new PreferencesWindow(m_accentColor, m_shouldAppsUseDarkMode);
    m_prefsWindow->exec();
}

void PawxelApp::onPrefChangeRestartRequired() {
    if (!m_restartMsgBox)
        m_restartMsgBox = new FHMessageBox(FHMessageBox::Ok, m_accentColor, m_shouldAppsUseDarkMode);
    // TODO: Move the following two functions into FHNotifier
    m_restartMsgBox->setWindowTitle(tr("Restart required"));
    m_restartMsgBox->setMessageBoxTitle(tr("Restart required"));
    m_restartMsgBox->setText(tr("A restart is required for these changes to take effect."));
    m_restartMsgBox->setMessageBoxIcon(FHMessageBox::Warning);
    {
        m_restartMsgBox->titleWidget()->setFont(QFont(APP_FONT_BOLD, 9));
        m_restartMsgBox->textWidget()->setFont(QFont(APP_FONT, 9));
        m_restartMsgBox->setButtonFont(QFont(APP_FONT, 9));
    }
    connect(m_restartMsgBox, &FHMessageBox::firstButtonClicked, this, [=]() {
        m_restartMsgBox->close(); // safe?
    }, Qt::UniqueConnection);
    m_restartMsgBox->exec();
}

void PawxelApp::onAboutWindowRequested() {
    LOG(DEBUG) << "(application:onAboutWindowRequested) AboutWindow requested";
    if (!m_aboutWindow)
        m_aboutWindow = new AboutWindow(m_accentColor, m_shouldAppsUseDarkMode);
    m_aboutWindow->setObjectName("AboutWindow");
    m_aboutWindow->exec();
}

void PawxelApp::savePixToDisk(QPixmap _pix) {
    QString _saveFileName = QFileDialog::getSaveFileName(nullptr, tr("Save screenshot"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), SFILETYPES);
    if (!_saveFileName.isEmpty()) {
        this->savePix(_saveFileName, _pix);
    } else {
        LOG(ERROR) << "(application:savePixToDisk) User cancelled save request.";
    }
}

void PawxelApp::autoSavePixToDisk(QPixmap _pix) {
    QString _path = m_preferences->screenshotsFolder();
    if (!QDir(_path).exists()) LOG(FATAL) << "(application:autoSavePixToDisk) Auto-save folder missing!";
    QString _extension = ".png"; // default
    if (m_preferences->saveFileFormat() == 1) _extension = ".jpg";
    QString _fname = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString _fq = _path + "/" + _fname + _extension;
    this->savePix(_fq, _pix);
}

void PawxelApp::copyPixToClipboard(QPixmap _pix) {
    this->clipboard()->setPixmap(_pix, QClipboard::Clipboard);
}

void PawxelApp::onHotkeyChanged(int _id, const QString &_newHotkey) {
    switch (_id) {
        case 0: {
            m_gFullscreenHotkey->setRegistered(false);
            if (m_gFullscreenHotkey->setShortcut(QKeySequence(_newHotkey))) {
                m_gFullscreenHotkey->setRegistered(true);
            } else {
                LOG(DEBUG) << "(application:onHotkeyChanged) Could not register new hotkey for fullscreen on the fly!";
            }
        } break;
        case 1: {
            m_gSnipHotkey->setRegistered(false);
            if (m_gSnipHotkey->setShortcut(QKeySequence(_newHotkey))) {
                m_gSnipHotkey->setRegistered(true);
            } else {
                LOG(DEBUG) << "(application:onHotkeyChanged) Could not register new hotkey for snip on the fly!";
            }
        } break;
    }
}

void PawxelApp::onAutostartChanged(bool _enabled) {
#if defined(Q_OS_WIN)
    QSettings _regSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
#else
    LOG(DEBUG) << "(application:onAutostartChanged) ERR! not yet implemented as each distro does it on it's own way..";
#endif
    if (_enabled) {
#if defined(Q_OS_WIN)
        _regSettings.setValue("pawxel", QDir::toNativeSeparators(PwxApp->applicationFilePath()));
        _regSettings.sync();
        LOG(DEBUG) << "(application:onAutostartChanged) Enabled autostart via REGKEY";
#endif
    } else {
#if defined(Q_OS_WIN)
        _regSettings.remove("pawxel");
        LOG(DEBUG) << "(application:onAutostartChanged) Deleted autostart from REGKEY";
#endif
    }
}
