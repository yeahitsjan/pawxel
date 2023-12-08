// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "easylogging++.h"
#include "platform.h"
#include "preferences.h"

#include <QStandardPaths>
#include <QFileInfo>

#include <QFile>
#include <QDir>
#include <QTextStream>

namespace pawxel {

Preferences::Preferences(QObject *parent) : QObject(parent) {
    QString _path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.pawxel/prefs.ini";
    QFileInfo _fi(_path);
    if (_fi.exists()) {
        m_preferencesFilePath = _path;
    } else {
        LOG(INFO) << "(preferences:Preferences) Custom preferences do not exist. Use default.";
#if defined(PWX_NIGHTLY)
        m_preferencesFilePath = ":/resources/default_preferences_nightly.ini";
#else
        m_preferencesFilePath = ":/resources/default_preferences.ini";
#endif
        if (this->readFileIntoBuffer(false)) {
            this->writeCustom();
        }
    }
}

Preferences::~Preferences() {
    m_content = "";
}

bool Preferences::flush() {
    if (this->readFileIntoBuffer(false)) {
        m_startupMode = this->getValue("startup_mode").toInt();
        m_appIcon = this->getValue("app_icon").toString();
        m_appTheme = this->getValue("app_theme").toInt();
        m_editorBackground = this->getValue("editor_background").toInt();
        m_screenshotsFolder = this->getValue("screenshots_folder").toString();
        m_saveFileFormat = this->getValue("save_file_format").toInt();
        m_screenShotChoice = this->getValue("preview_choice").toInt();
        m_autostart = this->getValue("autostart").toBool();
        m_invertMouseZoom = this->getValue("invert_zoom").toBool();
        m_editorGLAcceleration = this->getValue("editor_gl_accel").toBool();
        m_editorTextAA = this->getValue("editor_text_aa").toBool();
        m_hotkeyGlobalFullscreen = this->getValue("hotkey_fullscreen").toString();
        m_hotkeyGlobalSnip = this->getValue("hotkey_area").toString();
        m_lastColor = this->getValue("last_color").toString();
        if (m_content.contains("$debug_mode")) {
            m_debugMode = this->getValue("$debug_mode").toBool();
        } else {
            m_debugMode = false;
        }
    } else {
        LOG(DEBUG) << "(preferences:flush) Could not flush variables (preferences).";
        return false;
    }
    return true;
}

bool Preferences::write(const QString &_id, QString _newVal) {
    QString _cnt;
    QString _oldVal = this->getValue(_id).toString();
    if (m_preferencesFilePath.endsWith("default_preferences.ini")) {
        // this shouldn't be reached
        LOG(ERROR) << "(preferences:write) Cannot write to resources.";
        return false;
    }
    QFile _f(m_preferencesFilePath);
    if (_f.open( QIODevice::ReadWrite )) {
        QTextStream rd(&_f);
        _cnt = rd.readAll();
    }
    if (!_cnt.contains(_id)) {
        _cnt.append("\n" + _id + " = $NEW");
        _oldVal = "$NEW";
    }
    _f.resize(0); // truncate
    _f.close();

    QString _oldLine(_id + " = " + _oldVal);
    QString _newLine(_id + " = " + _newVal);
    _cnt.replace(_oldLine, _newLine);

    if (_f.open( QIODevice::ReadWrite )) {
        QTextStream wr(&_f);
        wr << _cnt;
        return true;
    } else {
        LOG(FATAL) << "(preferences:write) Could not reapply preferences to file!";
    }
    return false;
}

void Preferences::setStartupMode(int _i) {
    if (this->write("startup_mode", QVariant(_i).toString())) {
        m_startupMode = _i;
        LOG(DEBUG) << "This change requires a restart.";
        this->flush();
        emit restartToApply();
    }
}

void Preferences::setAppIcon(const QString _name) {
    if (this->write("app_icon", _name)) {
        m_appIcon = _name;
        LOG(DEBUG) << "This change requires a restart.";
        this->flush();
        emit restartToApply();
    }
}

void Preferences::setAppTheme(int _i) {
    if (this->write("app_theme", QVariant(_i).toString())) {
        m_appTheme = _i;
        LOG(DEBUG) << "This change requires a restart.";
        this->flush();
        emit restartToApply();
    }
}

void Preferences::setEditorBackground(int _i) {
    if (this->write("editor_background", QVariant(_i).toString())) {
        m_editorBackground = _i;
        LOG(DEBUG) << "Changed editor background. Requires restart.";
        this->flush();
        emit restartToApply();
    }
}

void Preferences::setScreenshotsFolder(const QString &_str) {
    if (this->write("screenshots_folder", _str)) {
        if (!_str.startsWith("$EMPTY")) {
            QDir _folder(_str);
            if (_folder.exists()) {
                m_screenshotsFolder = _str;
                LOG(DEBUG) << "(preferences:setScreenshotsFolder) Screenshots auto-save folder changed to: " << m_screenshotsFolder;
            }
        } else {
            m_screenshotsFolder = "$EMPTY";
            LOG(DEBUG) << "(preferences:setScreenshotsFolder) Auto-save screenshots disabled.";
        }
        this->flush();
        emit screenshotsFolderChanged(_str);
    }
}

void Preferences::setSaveFileFormat(int _i) {
    if (this->write("save_file_format", QVariant(_i).toString())) {
        m_saveFileFormat = _i;
        LOG(DEBUG) << "Screenshots auto-save format changed to: " << m_saveFileFormat;
        this->flush();
        emit saveFileFormatChanged(_i);
    }
}

void Preferences::setScreenShotChoice(int _i) {
    if (this->write("preview_choice", QVariant(_i).toString())) {
        m_screenShotChoice = _i;
        this->flush();
        emit screenShotChoiceChanged(_i);
    }
}

void Preferences::setAutostart(bool _b) {
    if (this->write("autostart", QVariant(_b).toString())) {
        m_autostart = _b;
        LOG(DEBUG) << "Autostart changed: " << m_autostart;
        this->flush();
        emit autostartChanged(_b);
    }
}

void Preferences::setInvertMouseZoom(bool _b) {
    if (this->write("invert_zoom", QVariant(_b).toString())) {
        m_invertMouseZoom = _b;
        LOG(DEBUG) << "Invert mouse zoom changed: " << m_invertMouseZoom;
        this->flush();
        emit mouseZoomInvertChanged(_b);
    }
}

void Preferences::setEditorGLAcceleration(bool _b) {
    if (this->write("editor_gl_accel", QVariant(_b).toString())) {
        m_editorGLAcceleration = _b;
        LOG(DEBUG) << "GL acceleration changed: " << m_editorGLAcceleration;
        this->flush();
        emit restartToApply();
    }
}

void Preferences::setEditorTextAA(bool _b) {
    if (this->write("editor_text_aa", QVariant(_b).toString())) {
        m_editorTextAA = _b;
        LOG(DEBUG) << "Antialiasing changed: " << m_editorTextAA;
        this->flush();
        emit editorAAChanged(_b);
    }
}

void Preferences::setHotkeyFullscreen(const QString &_str) {
    if (this->write("hotkey_fullscreen", QVariant(_str).toString())) {
        m_hotkeyGlobalFullscreen = _str;
        LOG(DEBUG) << "Fullscreen hotkey changed: " << m_hotkeyGlobalFullscreen;
        this->flush();
        emit globalHotkeyChanged(0, _str);
    }
}

void Preferences::setHotkeySnip(const QString &_str) {
    if (this->write("hotkey_area", QVariant(_str).toString())) {
        m_hotkeyGlobalSnip = _str;
        LOG(DEBUG) << "Snip hotkey changed: " << m_hotkeyGlobalSnip;
        this->flush();
        emit globalHotkeyChanged(1, _str);
    }
}

void Preferences::setLastColor(const QString &_str) {
    if (this->write("last_color", QVariant(_str).toString())) {
        m_lastColor = _str;
        LOG(DEBUG) << "Last color changed: " << m_lastColor;
        this->flush();
        // no emit needed, loaded on demand when item is added
    }
}

QString Preferences::lastColor() {
    if (m_lastColor.isEmpty()) {
        m_lastColor = "#ff0000";
    }
    return m_lastColor;
}

bool Preferences::readFileIntoBuffer(bool _fallback) {
    QString _content;
    QFile _f;
    if (!_fallback)
        _f.setFileName(m_preferencesFilePath);
    else
        _f.setFileName(":/resources/default_preferences.ini");
    if (_f.exists() && _f.open( QIODevice::ReadOnly )) {
        QTextStream rd(&_f);
        _content = rd.readAll();
    } else {
        LOG(ERROR) << "(preferences:readFileIntoBuffer) Could not read preferences file!";
        return false;
    }
    _f.close();
    m_content = _content;
    if (!m_content.isEmpty()) LOG(DEBUG) << "(preferences:readFileIntoBuffer) Successfully loaded preferences into buffer.";
    return true;
}

QVariant Preferences::getValue(const QString &_key) {
    QVariant _res;
    if (m_content.isEmpty()) {
        LOG(DEBUG) << "(preferences:getValue) Please load the preferences file into the buffer first!";
        return _res;
    }
    QTextStream _stream(&m_content);
    while (!_stream.atEnd()) {
        QString line = _stream.readLine();
        QString id(_key + " = ");
        int pos = line.indexOf(id);
        if (pos >= 0)
            _res = QVariant(line.mid(pos + id.length()));
    }
    return _res;
}

bool Preferences::writeCustom() {
    if (!m_content.isEmpty()) {
#if defined(PWX_NIGHTLY)
        QString _pew = "default_preferences_nightly.ini";
#else
        QString _pew = "default_preferences.ini";
#endif
        if (m_preferencesFilePath.endsWith(_pew))
            m_preferencesFilePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.pawxel/prefs.ini";
        QFile _f(m_preferencesFilePath);
        if (_f.open(QIODevice::ReadWrite)) {
            QTextStream wr(&_f);
            wr << m_content;
            return true;
        }
    }
    return false;
}

} // namespace
