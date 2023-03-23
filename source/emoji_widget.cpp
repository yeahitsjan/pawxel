// Pawxel - Copyright (c) 2022 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#include "platform.h"
#include "easylogging++.h"
#include "emoji_widget.h"

#include <QHBoxLayout>
#include <QScrollArea>

#include <QDirIterator>

namespace pawxel {

EmojiButton::EmojiButton(const QString &_path, QWidget *parent) : m_fullPath(_path), QPushButton(parent) {
    this->setObjectName("ActionButtons");
    // Apply emoji as icon to button.
    this->setIcon(QIcon(m_fullPath));
    this->setIconSize(QSize(42, 42));
    this->setFixedSize(QSize(60, 60));
}

EmojiWidget::EmojiWidget(QColor _accentColor, bool _isDarkMode, QWidget *parent) : m_accentColor(_accentColor), m_isDarkMode(_isDarkMode), FramelessDialog(parent) {
#if defined(Q_OS_WINDOWS)
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
#else
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
#endif
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(QSize(345, 400));
    this->setPalette(PWX_appPalette(QColor(0, 0, 0), m_isDarkMode));

    if (!m_widgetLayout)
        m_widgetLayout = new QVBoxLayout(this);
    m_widgetLayout->setSpacing(2);
    m_widgetLayout->setMargin(0);

    QHBoxLayout *_head = new QHBoxLayout;
    {
        _head->setSpacing(0);
        _head->setMargin(0);
    }

    QWidget *_space = new QWidget;
    _space->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_closeBtn = new QPushButton(CLO_CODEPOINT, this);
    {
        connect(m_closeBtn, &QPushButton::clicked, this, [=]() {
            this->hide();
        });
        m_closeBtn->setFont(QFont(TB_FONT, TB_SIZE));
        m_closeBtn->setObjectName("WindowButtons");
        m_closeBtn->setFixedSize(QSize(38, 38));
    }
    _head->addWidget(_space);
    _head->addWidget(m_closeBtn);

    m_widgetLayout->addLayout(_head);
    QScrollArea *_fill = new QScrollArea(this);
    _fill->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (!m_emojiGrid)
        m_emojiGrid = new FlowLayout;
    this->loadEmojiGrid(":/emojis/resources/icons/openmoji/");
    _fill->setLayout(m_emojiGrid);
    m_widgetLayout->addWidget(_fill);

    this->setLayout(m_widgetLayout);
}

EmojiWidget::~EmojiWidget() {
}

void EmojiWidget::loadEmojiGrid(const QString &_path) {
    if (!m_emojiGrid) LOG(ERROR) << "Grid not popluated before loading.";
    QDirIterator _it(_path);
    while (_it.hasNext()) {
        m_lEmojis.append(_it.next());
    }
    for (int i = 0; i < m_lEmojis.size(); ++i) {
        QString _emoji = m_lEmojis[i];
        EmojiButton *_btn = new EmojiButton(_emoji);
        connect(_btn, &QPushButton::clicked, [this, _emoji] { emit emojiRequested(_emoji); });
        m_emojiGrid->addWidget(_btn);
    }
}

} // namespace
