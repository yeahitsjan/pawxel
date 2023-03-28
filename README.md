<p align="center">
  <a href="https://pawxel.rocks">
    <img height="64" width="64" src="https://pawxel.rocks/static/icon_512.png">
  </a>
  <h1 align="center">pawxel<i> - a screenshot tool</i></h1>
</p>

![status](https://img.shields.io/badge/status-beta-blueviolet?style=flat-square) ![qtver](https://img.shields.io/badge/5.14.x+-blue?style=flat-square&logo=qt) ![license](https://img.shields.io/github/license/yeahitsjan/pawxel.svg?style=flat-square) ![downloads](https://img.shields.io/github/downloads/yeahitsjan/pawxel/total.svg?style=flat-square)

[Pawxel](https://pawxel.rocks) is a free and open-source screenshot tool heavily inspired by [Shottr](https://shottr.cc) with a focus on design and simplicity.

![screenshot](https://raw.githubusercontent.com/yeahitsjan/pawxel/develop/.github/screenshots/screenshot-bg.png)
A little screencast is available on the [website](https://pawxel.rocks/). You can grab the latest version of Pawxel from the [releases page](https://github.com/yeahitsjan/pawxel/tags).

To get started you can take a look at a [gentle overview](https://github.com/yeahitsjan/pawxel/blob/develop/resources/docs/overview.md) of Pawxel's UI. There are also [some tips & tricks](https://github.com/yeahitsjan/pawxel/blob/develop/resources/docs/tips.md) who can help you improve working with Pawxel.

## Features

- Rectangle, Circle, Marker and Text items
- Pixel Distance Measurement
- Copy color under cursor
- Autosave to folder
- Emoji's
- GPU-accelerated editor (if enabled in preferences)
- Infinite zoom viewport

...and there is still more to come!

To view a full list of what's working and what's not working during beta, you can visit [the changelog](https://github.com/yeahitsjan/pawxel/blob/develop/resources/docs/changelog.md).

## Install

> Pawxel is only released as an archive. There is no installer or similiar. The tool is and should be lightweight, an installation is not required. The only folder you need to delete after deleting Pawxel from your disk is $HOME/.pawxel.

Grab the latest ``.zip`` or ``.tar`` from the [releases page](https://github.com/yeahitsjan/pawxel/releases).

**Windows**
  
Run ``pawxel.exe`` inside the extracted archive

**Linux**

You can either run ``pawxel.sh`` or take a look into the ``bin/`` folder to find the executable and run it from there directly.

There is also an AUR (archlinux) package available maintained by [@christian-heusel](https://github.com/christian-heusel):
- [`pawxel-git` (AUR)](https://aur.archlinux.org/packages/pawxel-git)

## How to build from source

> _**NOTE**: Pawxel only runs on Windows & Linux. Because it is inspired by Shottr, I recommend that for using on macOS. Actively tested on Windows 10, Windows 11 and Debian-based Linux distros (Linux Mint & Ubuntu). Although untested it should easily run on Windows 8.x and other Linux distros._

To build Pawxel you'll need some dependencies first:

- [Qt 5.14.x or newer](https://download.qt.io/archive/qt/5.14/) ([FramelessHelper](https://github.com/wangwenx190/framelesshelper)'s minimum tested version is 5.14, don't expect it to work below that)
    - Qt6 is currently unsupported, view [#5](https://github.com/yeahitsjan/pawxel/issues/5) to track the progress or help improving
- [Git](https://git-scm.com)
- *on Windows:* MinGW64 (which comes with the installer of Qt)
- *on Linux:* gcc, g++, make (``build-essential`` on debian for example)

Clone the repo and it's submodules:

```
$ git clone https://github.com/yeahitsjan/pawxel.git
$ cd pawxel
$ git submodule init
$ git submodule update
```

You can then either run ``qmake`` and ``make / mingw32-make`` from your command line, or easier, just open up ``pawxel.pro`` with QtCreator and hit ``Build``.

## Report a bug

Some tasks before you create your report:

1. [Search](https://github.com/yeahitsjan/pawxel/issues?q=) through already existing issue reports. If you found a similar (or the exact same one), attach your report there.
2. When creating a report please include the following informations:
   - The platform you use (Windows or Linux) and the pretty name (e.g. Windows 10 or Endeavour OS).
   - The hardware you use (e.g. Intel Core i3-10105F, NVIDIA GTX 1660 Super etc.)
   - The version of Pawxel you are using (e.g. v0.0.1).
   - **if on Linux**, if you either use X11 or Wayland (you can run ``$ echo XDG_SESSION_TYPE`` in a terminal for more informations)
   - The output log of the [Debug mode](https://github.com/yeahitsjan/pawxel/blob/develop/resources/docs/tips.md#debug-mode) *(not necessary, but would be nice to have)*
3. Please give us detailed information about how to reproduce the issue you encounter. You can support that by adding pictures (or screenshots) and screen recordings.

From there on your report will be triaged and you get a response as quickly as possible.

## Thank-you letter! :heart:

Pawxel uses a variety of open-source libraries. To mention them we link them in this part of the README.

- [FramelessHelper](https://github.com/wangwenx190/framelesshelper): an awesome cross-platform window customization framework which builds on top of Qt - is used to achieve a true frameless window on all platforms
- [SingleApplication](https://github.com/itay-grudev/SingleApplication): Pawxel is a single instance application - this library helps achieving that
- [phantomstyle](https://github.com/randrew/phantomstyle): cross-platform style for Qt5 and Qt6 which I use since I started working with Qt
- [QHotkey](https://github.com/Skycoder42/QHotkey): every time you press your hotkey, QHotkey is used to send the signal to the application instance
- [resizer-item](https://github.com/cl0ne/resizer-item): an awesome reference which has been forked and extended to allow resizing items in a QGraphicsView
- [easylogging++](https://github.com/amrayn/easyloggingpp): logging library which can be used together with Qt

## License

Pawxel was closed-source until I reconsidered releasing the source code to the public. It is licensed under the GNU GPLv3. You can take a look at the [LICENSE](https://github.com/yeahitsjan/pawxel/blob/develop/LICENSE) file.