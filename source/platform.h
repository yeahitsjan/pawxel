// This header includes platform-specific things (like fonts or colors).
// Also the current Pawxel version is defined here.
#include <QtGlobal>
#include <QPalette>
#include <QColor>
#include <QString>
#include <QCryptographicHash>
#include <QDateTime>

// debug related
#include "easylogging++.h"
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOffscreenSurface>

#define PAWXEL_VERSION "0.1.4"

// Change based on rel type:
//#define PWX_REL
#define PWX_NIGHTLY

// based on logo
#define APP_FONT "Josefin Sans"
#if defined(Q_OS_WIN)
#define APP_FONT_MID "Josefin Sans SemiBold"
#define APP_FONT_BOLD "Josefin Sans Bold"
#define APP_FONT_SIZE 9
#else
#define APP_FONT_MID "Josefin Sans Semibold"
#define APP_FONT_BOLD "Josefin Sans Semibold" // for whatever reason just "Bold" does not work on Linux
#define APP_FONT_SIZE 10
#endif

// supported file types
#define SFILETYPES "PNG (*.png);;JPG (*.jpg *.jpeg);;Bitmap (*.bmp);;X11 Bitmap (*.xbm);;X11 Pixmap (*.xpm)"

#if defined(Q_OS_WIN)
#   define PLATFORM_FONT "Segoe UI"
#   define PLATFORM_FONT_MID "Segoe UI Semibold"
#   define PLATFORM_FONT_SIZE 9
#   define TB_FONT "Segoe MDL2 Assets"
#   define TB_SIZE 8
#   define MIN_CODEPOINT "\uE921"
#   define CLO_CODEPOINT "\uE8BB"
#else
#   define PLATFORM_FONT "Cantarell" // default font in GNOME
#   define PLATFORM_FONT_MID "Cantarell Bold"
#   define PLATFORM_FONT_SIZE 9
#   define TB_FONT "Material Icons Outlined"
#   define TB_SIZE 16
#   define MIN_CODEPOINT "\uE15B"
#   define CLO_CODEPOINT "\uE5CD"
#endif

inline QPalette PWX_appPalette(QColor _accentColor, bool _isDarkMode) {
    QPalette pal;
    if (_isDarkMode) {
        pal.setColor(QPalette::Window, QColor(34, 34, 34));
        pal.setColor(QPalette::Base, QColor(34, 34, 34));
        pal.setColor(QPalette::AlternateBase, QColor(26, 26, 26));
        pal.setColor(QPalette::Button, QColor(26, 26, 26));
        pal.setColor(QPalette::WindowText, QColor(197, 197, 197));
        pal.setColor(QPalette::Text, QColor(197, 197, 197));
        pal.setColor(QPalette::ButtonText, QColor(197, 197, 197));
    } else {
        pal.setColor(QPalette::Window, QColor(225, 225, 225));
        pal.setColor(QPalette::Base, QColor(225, 225, 225));
        pal.setColor(QPalette::AlternateBase, QColor(255, 255, 255));
        pal.setColor(QPalette::Button, QColor(255, 255, 255));
        pal.setColor(QPalette::WindowText, QColor(43, 43, 43));
        pal.setColor(QPalette::Text, QColor(43, 43, 43));
        pal.setColor(QPalette::ButtonText, QColor(43, 43, 43));
    }
    pal.setColor(QPalette::Highlight, _accentColor);
    return pal;
}

inline QString PWX_md5FromString(const QString &_text) {
    return QString("%1").arg(QString(QCryptographicHash::hash(_text.toUtf8(), QCryptographicHash::Md5).toHex()));
}

inline QString PWX_dateTimeStr() {
    QDateTime _date = QDateTime::currentDateTime();
    return _date.toString("dd.MM.yyyy hh:mm:ss");
}

struct SysGpuInfo {
    QString gpuVendor;
    QString deviceName;
    QString glVersion;
    float availableMemory;
    float totalMemory;
};

inline SysGpuInfo PWX_retrieveGpuInformations() {
    SysGpuInfo inf;

    QOpenGLContext ctx;
    ctx.create();

    QOffscreenSurface surf;
    surf.create();

    QOpenGLFunctions funcs;
    ctx.makeCurrent(&surf);
    funcs.initializeOpenGLFunctions();

    inf.gpuVendor = QString::fromLatin1((const char*)funcs.glGetString(GL_VENDOR));
    inf.deviceName = QString::fromLatin1((const char*)funcs.glGetString(GL_RENDERER));
    inf.glVersion = QString::fromLatin1((const char*)funcs.glGetString(GL_VERSION));
    LOG(INFO) << "pwx_debug: GPU vendor = " << inf.gpuVendor;
    LOG(INFO) << "pwx_debug: Device name = " << inf.deviceName;
    LOG(INFO) << "pwx_debug: GL version = " << inf.glVersion;

    GLint _curMemKb = 0;
    GLint _totalMemKb = 0;
    if (ctx.hasExtension("GL_ATI_meminfo")) {
        // @jan: You may think that with checking if this extension exists on AMD gpu's the work is done, but it isn't.
        // Several forum posts show that GL_ATI_meminfo is deprecated and there is no "replacement" for it. Still it works
        // for some specific cards and for some specific driver versions. I tested this with an RX550 2GB and didn't get
        // any results, even got a segfault when diving deeper.
        // I'll still keep this here if some AMD GPU has this extension enabled.
        // Posts:
        // https://community.amd.com/t5/archives-discussions/ccc-12-10-opengl-extension-gl-ati-meminfo-not-available/td-p/83727
        // https://community.amd.com/t5/opengl-vulkan/opangl-gl-ati-meminfo-replacement/td-p/320632
        #define VBO_FREE_MEMORY_ATI 0x87FB
        funcs.glGetIntegerv(VBO_FREE_MEMORY_ATI, &_curMemKb);
#if defined(PAWXEL_EXPERIMENTAL)
        #define GL_TEXTURE_FREE_MEMORY_ATI 0x87FC
        funcs.glGetIntegerv(TEXTURE_FREE_MEMORY_ATI, &_totalMemKb);
#endif
    } else if (ctx.hasExtension("GL_NVX_gpu_memory_info")) {
        // nvidia
        #define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
        #define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
        funcs.glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &_curMemKb);
        funcs.glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &_totalMemKb);
    }
    // todo: Intel

    float _cA = float(_curMemKb) / 1024.0f;
    float _cT = float(_totalMemKb) / 1024.0f;
    inf.availableMemory = _cA;
    inf.totalMemory = _cT;
    LOG(INFO) << "pwx_debug: Available GPU memory = " << inf.availableMemory;
    LOG(INFO) << "pwx_debug: Total GPU memory = " << inf.totalMemory;

    return inf;
}

/* Interesting other directives you could probably use (uncomment if want to):
==============================================================================
#define PAWXEL_UNUSED // enable unused code fragments
#define PAWXEL_EXPERIMENTAL // enable experimental code fragments (does not work when PWX_REL is defined too)
*/