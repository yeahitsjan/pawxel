// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "easylogging++.h"
#include "platform.h"
#include "application.h"

#include <QFontDatabase>

#include "sniparea.h"

FRAMELESSHELPER_USE_NAMESPACE
PawxelApp *PwxApp { nullptr };
INITIALIZE_EASYLOGGINGPP
using namespace pawxel;
static void initializeIconFont() {
    int c;
    c = QFontDatabase::addApplicationFont(":/resources/icons/action-bar/MaterialIconsOutlined-Regular.otf");
    c = QFontDatabase::addApplicationFont(":/resources/fonts/JosefinSans-Regular.ttf");
    c = QFontDatabase::addApplicationFont(":/resources/fonts/JosefinSans-Light.ttf");
    c = QFontDatabase::addApplicationFont(":/resources/fonts/JosefinSans-Bold.ttf");
    c = QFontDatabase::addApplicationFont(":/resources/fonts/JosefinSans-SemiBold.ttf");
    if (c == -1) {
        LOG(ERROR) << "(main:initializeIconFont) MaterialIconFont not added. Please install manually.";
        #undef APP_FONT
#if defined(Q_OS_WIN)
        #define APP_FONT "Segoe UI"
#else
        #define APP_FONT "Cantarell" // default on GNOME
#endif
    } else {
        LOG(INFO) << "(main:initializeIconFont) MaterialIconFont found in this session.";
    }
}

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    FramelessHelper::Widgets::initialize();
    PawxelApp pwxApp(argc, argv);
    pwxApp.setApplicationName(QLatin1String("Pawxel"));
    pwxApp.setApplicationVersion(PAWXEL_VERSION);
    initializeIconFont();
    
    return pwxApp.exec();
}