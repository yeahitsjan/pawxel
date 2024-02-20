// Pawxel - Copyright (c) 2022-2024 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <FramelessDialog>
#include <QObject>
#include <QWidget>

#include <QPointer>
#include <QStringList>

#include <QColor>

#include <QVBoxLayout>
#include <QPushButton>

#include "qt_flowlayout.h"

FRAMELESSHELPER_USE_NAMESPACE

namespace pawxel {

class EmojiButton : public QPushButton {
    Q_OBJECT
public:
    EmojiButton(const QString &path, QWidget *parent = nullptr);
    QString fullEmojiPath() { return m_fullPath; }
    void setFullEmojiPath(const QString _path) {
        m_fullPath = _path;
    }

protected:
    QString m_fullPath;
};

class EmojiWidget : public FramelessDialog {
    Q_OBJECT
public:
    explicit EmojiWidget(QColor _accentColor, bool _isDarkMode, QWidget *parent = nullptr);
    ~EmojiWidget();

protected:
    // Load emojis from path and fill QGridLayout dynamically.
    void loadEmojiGrid(const QString &_path);

protected:
    QPointer<QVBoxLayout> m_widgetLayout;
    QPointer<FlowLayout> m_emojiGrid;

    QPushButton *m_closeBtn;
    QStringList m_lEmojis;

    bool m_isDarkMode;
    QColor m_accentColor;

signals:
    void emojiRequested(const QString &_fpath);
};

} // namespace