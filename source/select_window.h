// Pawxel - Copyright (c) 2022-2024 Jan Kowalewicz. All rights reserved.
// Contact: jan@nitroosit.de; jan.kowalewicz@web.de
#pragma once

#include <FramelessDialog>
#include <QObject>
#include <QWidget>

#include <QPointer>

#include <QHBoxLayout>

#include <QPixmap>

FRAMELESSHELPER_USE_NAMESPACE

namespace pawxel {

class SelectWindow : public FramelessDialog {
    Q_OBJECT
public:
    explicit SelectWindow(bool _isDarkMode, QWidget *parent = nullptr);
    ~SelectWindow();

protected:
    QPointer<QHBoxLayout> m_windowLayout;

private:
    bool m_isDarkMode;

public slots:
    void onMultiPixmaps(QList<QPixmap> _lPixs);

signals:
    void userSelectedScreen(QPixmap _pix);
};

} // namespace