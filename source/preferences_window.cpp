// Pawxel - Copyright (c) 2022-2023 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "platform.h"
#include "application.h"
#include "preferences_window.h"

#include <QFont>
#include <QStyle>
#include "phantomstyle.h"

#include <QFrame>

namespace pawxel {

PreferencesWindow::PreferencesWindow(QColor _accentColor, bool _isDarkMode, QWidget *parent) : m_accentColor(_accentColor), m_isDarkMode(_isDarkMode), FramelessDialog(parent) {
    // ---
    this->setFont(QFont(APP_FONT, APP_FONT_SIZE));
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(QSize(385, 465));
    this->setWindowTitle(tr("Preferences"));
    this->setPalette(PWX_appPalette(m_accentColor, m_isDarkMode));

    // ---
    if (!m_windowLayout)
        m_windowLayout = new QVBoxLayout(this);
    m_windowLayout->setSpacing(0);
    m_windowLayout->setMargin(0);

    QHBoxLayout *_head = new QHBoxLayout;
    _head->setSpacing(0);
    _head->setMargin(0);
    QWidget *_space = new QWidget;
    _space->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_closeBtn = new QPushButton(CLO_CODEPOINT, this);
    {
        connect(m_closeBtn, &QPushButton::clicked, this, [=]() {
            this->close();
        });
        m_closeBtn->setFont(QFont(TB_FONT, TB_SIZE));
        m_closeBtn->setObjectName("WindowButtons");
        m_closeBtn->setFixedSize(QSize(38, 38));
    }
    _head->addWidget(_space);
    _head->addWidget(m_closeBtn);
    m_windowLayout->addLayout(_head);

    QWidget *_headWidget = new QWidget(this);
    _headWidget->setObjectName("PreferencesHeader");
    m_tabBtnLayout = new QHBoxLayout(_headWidget);
    {
        m_tabBtnLayout->setMargin(4);
        QWidget *_spaceLeft = new QWidget;
        _spaceLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        m_tabBtnLayout->addWidget(_spaceLeft);

#define GENERAL_CODEPOINT "\uE8B8"
#define HOTKEYS_CODEPOINT "\uE312"

        m_lTabBtns.append( m_generalBtn = new QPushButton(GENERAL_CODEPOINT, this) );
        m_lTabBtns.append( m_hotkeysBtn = new QPushButton(HOTKEYS_CODEPOINT, this) );
        foreach (QPushButton *_btn, m_lTabBtns) {
            _btn->setFont(QFont("Material Icons Outlined", 20));
            _btn->setObjectName("PreferencesButton");
            _btn->setFixedSize(QSize(72, 52));
            _btn->setCheckable(true);
        }
        m_tabBtnLayout->addWidget(m_generalBtn);
        m_tabBtnLayout->addWidget(m_hotkeysBtn);

        QWidget *_spaceRight = new QWidget;
        _spaceRight->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        m_tabBtnLayout->addWidget(_spaceRight);
    }
    _headWidget->setLayout(m_tabBtnLayout);
    m_windowLayout->addWidget(_headWidget);

    if (!m_stack)
        m_stack = new QStackedWidget(this);
    m_stack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    {
        m_stack->addWidget(this->createGeneralPage());
        connect(m_generalBtn, &QPushButton::clicked, this, [=]() {
            m_generalBtn->setChecked(true);
            m_hotkeysBtn->setChecked(false);
            // --
            m_stack->setCurrentIndex(0);
        });
        m_stack->addWidget(this->createHotkeyPage());
        connect(m_hotkeysBtn, &QPushButton::clicked, this, [=]() {
            m_hotkeysBtn->setChecked(true);
            m_generalBtn->setChecked(false);
            // --
            m_stack->setCurrentIndex(1);
        });
    }
    // On load index is always 0, set general button in checked state.
    m_generalBtn->setChecked(true);
    
    m_windowLayout->addWidget(m_stack);

    this->setLayout(m_windowLayout);
}

PreferencesWindow::~PreferencesWindow() {
}

QWidget* PreferencesWindow::createGeneralPage() {
    if (!m_generalPage)
        m_generalPage = new QWidget(this);
    
    m_generalPageLayout = new QFormLayout(m_generalPage);
    {
        m_generalPageLayout->setVerticalSpacing(8);
        m_generalPageLayout->setMargin(16);
    }
    QHBoxLayout *_modeLayout = new QHBoxLayout;
    {
        m_normalModeBox = new QRadioButton(tr("Normal"), m_generalPage);
        _modeLayout->addWidget(m_normalModeBox);
        m_performanceModeBox = new QRadioButton(tr("Performance"), m_generalPage);
        _modeLayout->addWidget(m_performanceModeBox);

        switch(PwxApp->preferences()->startupMode()) {
            case 0: {
                m_normalModeBox->setChecked(true);
                m_performanceModeBox->setChecked(false);
            } break;
            case 1: {
                m_normalModeBox->setChecked(false);
                m_performanceModeBox->setChecked(true);
            } break;
        }

        connect(m_normalModeBox, &QRadioButton::toggled, this, [=](bool _check) {
            if (_check) {
                m_performanceModeBox->setChecked(false);
                PwxApp->preferences()->setStartupMode(0);
            }
        });

        connect(m_performanceModeBox, &QRadioButton::toggled, this, [=](bool _check) {
            if (_check) {
                m_normalModeBox->setChecked(false);
                PwxApp->preferences()->setStartupMode(1);
            }
        });
    }
    m_generalPageLayout->addRow(tr("App Behavior"), new QWidget);
    m_generalPageLayout->addRow(tr("Startup mode:"), _modeLayout);
    m_appIconBox = new QComboBox(m_generalPage);
    {
        QStringList _icons = {"default", "nightly"};
        m_appIconBox->addItems(_icons);
        m_appIconBox->setCurrentText(PwxApp->preferences()->appIcon());
        connect(m_appIconBox, &QComboBox::currentTextChanged, this, [=](QString _icon) {
            PwxApp->preferences()->setAppIcon(_icon);
        });
    }
    m_generalPageLayout->addRow(tr("App icon:"), m_appIconBox);
    m_appThemeBox = new QComboBox(m_generalPage);
    {
        QStringList _themes = {"Auto (System)", "Light", "Dark"};
        m_appThemeBox->addItems(_themes);
        m_appThemeBox->setCurrentIndex(PwxApp->preferences()->appTheme());
        connect(m_appThemeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int _i) {
            PwxApp->preferences()->setAppTheme(_i);
        });
    }
    m_generalPageLayout->addRow(tr("Theme:"), m_appThemeBox);
    m_editorBackgroundBox = new QComboBox(m_generalPage);
    {
        QStringList _backgrounds = {"Cross", "Grid"};
        m_editorBackgroundBox->addItems(_backgrounds);
        m_editorBackgroundBox->setCurrentIndex(PwxApp->preferences()->editorBackground());
        connect(m_editorBackgroundBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int _i) {
            PwxApp->preferences()->setEditorBackground(_i);
        });
    }
    m_generalPageLayout->addRow(tr("Editor Background:"), m_editorBackgroundBox);
    m_screenShotChoiceBox = new QComboBox(m_generalPage);
    {
        QStringList _screenShotChoices = {"Open Preview", "Open Editor"};
        m_screenShotChoiceBox->addItems(_screenShotChoices);
        m_screenShotChoiceBox->setCurrentIndex(PwxApp->preferences()->screenShotChoice());
        connect(m_screenShotChoiceBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int _i) {
            PwxApp->preferences()->setScreenShotChoice(_i);
        });
    }
    m_generalPageLayout->addRow(tr("Screen Shot Choice:"), m_screenShotChoiceBox);
    m_autostartBox = new QCheckBox(m_generalPage);
    {
        m_autostartBox->setFixedWidth(20);
        m_autostartBox->setChecked(PwxApp->preferences()->autostartEnabled());
        connect(m_autostartBox, &QCheckBox::toggled, this, [=](bool _b) {
            PwxApp->preferences()->setAutostart(_b);
        });
    }
    m_generalPageLayout->addRow(tr("Enable autostart:"), m_autostartBox);

    m_generalPageLayout->addRow(tr("Auto-save"), new QWidget);
    m_screenshotsFolderEdit = new QLineEdit(m_generalPage);
    {
        m_screenshotsFolderEdit->setPlaceholderText(tr("Leave empty if not wanted"));
        if (!PwxApp->preferences()->screenshotsFolder().startsWith("$EMPTY"))
            m_screenshotsFolderEdit->setText(PwxApp->preferences()->screenshotsFolder());
        connect(m_screenshotsFolderEdit, &QLineEdit::editingFinished, this, [=]() {
            if (!m_screenshotsFolderEdit->text().isEmpty()) {
                PwxApp->preferences()->setScreenshotsFolder(m_screenshotsFolderEdit->text());
            } else {
                PwxApp->preferences()->setScreenshotsFolder("$EMPTY");
            }
        });
    }
    m_generalPageLayout->addRow(tr("Screenshots folder:"), m_screenshotsFolderEdit);
    m_saveFileFormatBox = new QComboBox(m_generalPage);
    {
        m_saveFileFormatBox->addItem("PNG");
        m_saveFileFormatBox->addItem("Auto (PNG/JPG)");
        m_saveFileFormatBox->setCurrentIndex(PwxApp->preferences()->saveFileFormat());
        connect(m_saveFileFormatBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int _i) {
            PwxApp->preferences()->setSaveFileFormat(_i);
        });
    }
    m_generalPageLayout->addRow(tr("Save file format:"), m_saveFileFormatBox);

    m_generalPageLayout->addRow(tr("Editor Preferences"), new QWidget);
#if defined(PAWXEL_EXPERIMENTAL)
    m_invertMouseZoomBox = new QCheckBox(m_generalPage);
    {
        m_invertMouseZoomBox->setChecked(PwxApp->preferences()->mouseZoomInverted());
        connect(m_invertMouseZoomBox, &QCheckBox::toggled, this, [=](bool _b) {
            PwxApp->preferences()->setInvertMouseZoom(_b);
        });
    }
    m_generalPageLayout->addRow(tr("Invert Editor mouse zoom:"), m_invertMouseZoomBox);
#endif
    m_editorGLBox = new QCheckBox(m_generalPage);
    {
        m_editorGLBox->setChecked(PwxApp->preferences()->editorUsesGLAcceleration());
        connect(m_editorGLBox, &QCheckBox::toggled, this, [=](bool _b) {
            PwxApp->preferences()->setEditorGLAcceleration(_b);
        });
    }
    m_generalPageLayout->addRow(tr("Enable GPU-powered editing:"), m_editorGLBox);
    m_editorAABox = new QCheckBox(m_generalPage);
    {
        m_editorAABox->setChecked(PwxApp->preferences()->editorUsesTextAA());
        connect(m_editorAABox, &QCheckBox::toggled, this, [=](bool _b) {
            PwxApp->preferences()->setEditorTextAA(_b);
        });
    }
    m_generalPageLayout->addRow(tr("Enable Editor anti-aliasing:"), m_editorAABox);

    // font-styling
    // 1. set all labels to "Josefin Sans":
    for (int i = 0; i < m_generalPageLayout->rowCount(); i++) {
        auto _lbl = m_generalPageLayout->itemAt(i, QFormLayout::LabelRole)->widget();
        _lbl->setFont(QFont(APP_FONT, 9));
    }
    // 2. set all headers to "Bold":
    auto _head1 = m_generalPageLayout->itemAt(0, QFormLayout::LabelRole)->widget();
    _head1->setFont(QFont(APP_FONT, 10));
    auto _head2 = m_generalPageLayout->itemAt(6, QFormLayout::LabelRole)->widget();
    _head2->setFont(QFont(APP_FONT, 10));
    auto _head3 = m_generalPageLayout->itemAt(9, QFormLayout::LabelRole)->widget();
    _head3->setFont(QFont(APP_FONT, 10));

    m_generalPageLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    m_generalPage->setLayout(m_generalPageLayout);
    return m_generalPage;
}

QWidget* PreferencesWindow::createHotkeyPage() {
    if (!m_hotkeyPage)
        m_hotkeyPage = new QWidget(this);
    
    m_hotkeyPageLayout = new QFormLayout(m_hotkeyPage);
    {
        m_hotkeyPageLayout->setVerticalSpacing(8);
        m_hotkeyPageLayout->setMargin(16);
    }
    m_hotkeySnipEdit = new QKeySequenceEdit(QKeySequence(PwxApp->preferences()->hotkeySnip()), m_hotkeyPage);
    connect(m_hotkeySnipEdit, &QKeySequenceEdit::editingFinished, this, [=]() {
        int _val = m_hotkeySnipEdit->keySequence()[0];
        QKeySequence _seq(_val);
        m_hotkeySnipEdit->setKeySequence(_seq);
        PwxApp->preferences()->setHotkeySnip(_seq.toString());
    });
    m_hotkeyPageLayout->addRow(tr("Snip hotkey:"), m_hotkeySnipEdit);
    m_hotkeyFullscreenEdit = new QKeySequenceEdit(QKeySequence(PwxApp->preferences()->hotkeyFullscreen()), m_hotkeyPage);
    connect(m_hotkeyFullscreenEdit, &QKeySequenceEdit::editingFinished, this, [=]() {
        int _val = m_hotkeyFullscreenEdit->keySequence()[0];
        QKeySequence _seq(_val);
        m_hotkeyFullscreenEdit->setKeySequence(_seq);
      PwxApp->preferences()->setHotkeyFullscreen(_seq.toString());
    });
    m_hotkeyPageLayout->addRow(tr("Fullscreen hotkey:"), m_hotkeyFullscreenEdit);

    for(int i = 0; i < m_hotkeyPageLayout->rowCount(); i++) {
        auto _lbl = m_hotkeyPageLayout->itemAt(i, QFormLayout::LabelRole)->widget();
        _lbl->setFont(QFont(APP_FONT, APP_FONT_SIZE));
    }

    m_hotkeyPageLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    m_hotkeyPage->setLayout(m_hotkeyPageLayout);
    return m_hotkeyPage;
}

} // namespace
