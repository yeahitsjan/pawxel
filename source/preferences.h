// Pawxel - Copyright (c) 2022-2024 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <QObject>
#include <QVariant>
#include <QString>

namespace pawxel {

class Preferences : public QObject {
    Q_OBJECT
public:
    explicit Preferences(QObject *parent = nullptr);
    ~Preferences();

    // Read and set the application preferences.
    bool flush();

    // Write application preferences.
    bool write(const QString &_id, QString _newVal);

    void setStartupMode(int _i);
    int startupMode() { return m_startupMode; }

    void setAppIcon(const QString _name);
    QString appIcon() { return m_appIcon; }

    void setAppTheme(int _i);
    int appTheme() { return m_appTheme; }

    void setEditorBackground(int _i);
    int editorBackground() { return m_editorBackground; }

    void setScreenshotsFolder(const QString &_str);
    QString screenshotsFolder() { return m_screenshotsFolder; }

    void setSaveFileFormat(int _i);
    int saveFileFormat() { return m_saveFileFormat; }

    void setAfterScreenshot(int _i);
    int afterScreenshot() { return m_afterScreenshot; }

    void setAutostart(bool _b);
    bool autostartEnabled() { return m_autostart; }

    void setInvertMouseZoom(bool _b);
    bool mouseZoomInverted() { return m_invertMouseZoom; }

    void setEditorGLAcceleration(bool _b);
    bool editorUsesGLAcceleration() { return m_editorGLAcceleration; }

    void setEditorTextAA(bool _b);
    bool editorUsesTextAA() { return m_editorTextAA; }

    void setHotkeyFullscreen(const QString &_str);
    QString hotkeyFullscreen() { return m_hotkeyGlobalFullscreen; }

    void setHotkeySnip(const QString &_str);
    QString hotkeySnip() { return m_hotkeyGlobalSnip; }

    void setLastColor(const QString &_str);
    QString lastColor();

    bool debugMode() { return m_debugMode; }

protected:
    // 0 = normal (systray) mode; 1 = performance mode
    int m_startupMode;

    // default = icon_default.*; pride = icon_pride.* etc..
    QString m_appIcon;

    // 0 = system; 1 = light; 2 = dark
    int m_appTheme;

    // 0 = cross; 1 = grid; 2 = chessboard
    int m_editorBackground;

    QString m_screenshotsFolder;
    // 0 = PNG; 1 = auto
    int m_saveFileFormat;

    // 0 = opens preview; 1 = opens editor; 2 = copies screenshot
    int m_afterScreenshot;

    // only available when startup mode 0
    bool m_autostart;

    bool m_invertMouseZoom;
    bool m_editorGLAcceleration;
    bool m_editorTextAA;

    QString m_hotkeyGlobalFullscreen;
    QString m_hotkeyGlobalSnip;

    // last color used on *any* item
    QString m_lastColor;

    bool m_debugMode;

    QString m_preferencesFilePath;
    QString m_content;

    // Read the full preferences file into a variable / buffer.
    bool readFileIntoBuffer(bool _fallback);
    // Get a specific value.
    QVariant getValue(const QString &_key);

    // Create custom preferences file.
    bool writeCustom();

signals:
    void restartToApply();
    void screenshotsFolderChanged(QString _str);
    void saveFileFormatChanged(int _i);
    void afterScreenshotChanged(int _i);
    void autostartChanged(bool _b);
    void mouseZoomInvertChanged(bool _b);
    void editorAAChanged(bool _b);
    void globalHotkeyChanged(int _id, const QString &_newHotkey);
};

} // namespace