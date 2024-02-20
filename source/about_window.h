// Pawxel - Copyright (c) 2022-2024 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <FramelessDialog>
#include <QObject>
#include <QWidget>

#include <QPointer>

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextBrowser>

FRAMELESSHELPER_USE_NAMESPACE

namespace pawxel {

class AboutWindow : public FramelessDialog {
    Q_OBJECT
public:
    explicit AboutWindow(QColor _accentColor, bool _isDarkMode, QWidget *parent = nullptr);
    ~AboutWindow();

protected:
    QPointer<QVBoxLayout> m_windowLayout;
    QPushButton *m_iconBtn;

    QLabel *m_versionLbl;
    QTextBrowser *m_licenseBrowser;

private:
    QColor m_accentColor;
    bool m_isDarkMode;
};

} // namespace