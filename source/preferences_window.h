// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <FramelessDialog>
#include <QObject>
#include <QWidget>

#include <QPointer>
#include <QList>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QFrame>

#include <QFormLayout>
#include <QKeySequenceEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>

#include <QColor>

FRAMELESSHELPER_USE_NAMESPACE

namespace pawxel {

class PreferencesWindow : public FramelessDialog {
    Q_OBJECT
public:
    explicit PreferencesWindow(QColor _accentColor, bool _isDarkMode, QWidget *parent = nullptr);
    ~PreferencesWindow();

protected:
    QPushButton *m_closeBtn;
    QPointer<QVBoxLayout> m_windowLayout;
    QHBoxLayout *m_tabBtnLayout;
    QPointer<QStackedWidget> m_stack;

    QPushButton *m_generalBtn;
    QPushButton *m_hotkeysBtn;
    QList<QPushButton*> m_lTabBtns;

private:
    QPointer<QWidget> m_generalPage;
    QFormLayout *m_generalPageLayout;
    QRadioButton *m_normalModeBox;
    QRadioButton *m_performanceModeBox;
    QComboBox *m_appIconBox;
    QComboBox *m_appThemeBox;
    QComboBox *m_editorBackgroundBox;
    QLineEdit *m_screenshotsFolderEdit;
    QComboBox *m_saveFileFormatBox;
    QComboBox *m_screenShotChoiceBox;
    QCheckBox *m_autostartBox;
    QCheckBox *m_invertMouseZoomBox;
    QCheckBox *m_editorGLBox;
    QCheckBox *m_editorAABox;
    QWidget* createGeneralPage();

    QPointer<QWidget> m_hotkeyPage;
    QFormLayout *m_hotkeyPageLayout;
    QKeySequenceEdit *m_hotkeySnipEdit;
    QKeySequenceEdit *m_hotkeyFullscreenEdit;
    QWidget* createHotkeyPage();

    QColor m_accentColor;
    bool m_isDarkMode;
};

} // namespace