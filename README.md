# Pawxel <img src="https://pawxel.rocks/static/icon_512.png" width="28" height="28" align="center">

![status](https://img.shields.io/badge/status-beta-blueviolet?style=flat-square) ![qtver](https://img.shields.io/badge/5.14.x-blue?style=flat-square&logo=qt)

![screenshot](https://pawxel.rocks/static/poster2.png)

> The last screenshot tool you will ever use - I promise.

[Pawxel](https://pawxel.rocks) is a free and open-source screenshot tool heavily inspired by [Shottr](https://shottr.cc) with a focus on design and simplicity.

A little screencast is available on the [website](https://pawxel.rocks/). You can grab the latest version Pawxel from the [releases page](https://github.com/yeahitsjan/pawxel/tags).

There is not much documentation as of now, but I try to build some in the coming months. Still there are [some tips & tricks](https://github.com/yeahitsjan/pawxel/blob/develop/resources/docs/tips.md) who can help you getting started.

## Features

- Pixel Distance Measurement
- Copy color under cursor
- Autosave to folder
- Emoji's
- GPU-accelerated editor (if enabled in preferences)
- Infinite zoom viewport

...and there is still more to come!

To view a full list of what's working and what's not working during beta, you can visit [the changelog](https://github.com/yeahitsjan/pawxel/blob/develop/resources/docs/changelog.md).

## How to build

> ***NOTE**: Pawxel only runs on Windows & Linux. Because it is inspired by Shottr, I recommend that for using on macOS. Actively tested on Windows 10, Windows 11 and Debian-based Linux distros (Linux Mint & Ubuntu). Although unsupported it should easily run on Windows 8.x and other Linux distros.*

To build Pawxel you'll need some dependencies first:

- [Qt 5.14.x](https://download.qt.io/archive/qt/5.14/)
- [Git](https://git-scm.com)
- *on Windows* MinGW64 (which comes with the installer of Qt)
- *on Linux* gcc, make (``build-essential`` on debian for example)

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
3. Please give us a detailed information about how to reproduce the issue you encounter. You can support that by adding pictures (or screenshots) and screen recordings.

From there on your report will be triaged and you get a response as quickly as possible.

## License

Pawxel was closed-source until I reconsidered releasing the source code to the public. It is licensed under the GNU GPLv3. You can take a look at the [LICENSE](https://github.com/yeahitsjan/pawxel/blob/develop/LICENSE) file.