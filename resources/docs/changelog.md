# Changelog

## 0.1.4

![type](https://img.shields.io/badge/type-beta-blueviolet?style=flat-square) ![qtver](https://img.shields.io/badge/qt-5.14.2-blue?style=flat-square&logo=qt) ![qtver](https://img.shields.io/badge/qt-5.15.2-blue?style=flat-square&logo=qt)

*29 November 2023*

:rocket: Features / Improvements:
- Display indicator when multiple screens captured
    - as requested in #12 (https://github.com/yeahitsjan/pawxel/issues/12)
- Always restore capture
    - The last capture is now safely stored even after a reboot
    - Message Box appears if there is no recent capture available
- New preview window shortcuts (thanks to @paipeng05)
    - `Ctrl+R`: retake screenshot
    - `Ctrl+E`: launch editor
    - `Ctrl+W`: close preview window
    - `Esc`: close preview window
    - Also, all buttons inside the preview window now got a tooltip with their corresponding shortcut!
- While Snipping, you can now exit the Snip area by pressing either Esc or if you left click without dragging (thanks to @paipeng05)
    
:bug: Bugfixes:
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Fix preview window ignoring theme after retake
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Fix segfault when editor is visible during snip area requested
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Remove hardcoded fonts

## 0.1.3

*13 April 2023*

![type](https://img.shields.io/badge/type-beta-blueviolet?style=flat-square) ![qtver](https://img.shields.io/badge/qt-5.14.2-blue?style=flat-square&logo=qt) ![qtver](https://img.shields.io/badge/qt-5.15.2-blue?style=flat-square&logo=qt)

:rocket: Features / Improvements:
- Fullscreen screenshot with multiple screens
    - You are now greeted by a selection window if more than one screen is attached
- Quick shortcuts in Preview Window
    - You can now press Ctrl+C to copy or Ctrl+S to save inside the small preview window

:bug: Bugfixes:
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Show visually which page in preferences is selected

:spiral_notepad: Known Bugs:
- [Dual Monitor support not working as expected (#9)](https://github.com/yeahitsjan/pawxel/issues/9)
    - Currently I work on a [branch](https://github.com/yeahitsjan/pawxel/tree/fix-multi-screen) to fix this, but did not get any feedback yet. I try to get this working as fast as possible.

<hr>

## 0.1.2

*22 March 2023*

![type](https://img.shields.io/badge/type-beta-blueviolet?style=flat-square) ![qtver](https://img.shields.io/badge/qt-5.14.2-blue?style=flat-square&logo=qt)

:rocket: Features / Improvements:
- Add specific Windows resource (RC) file to display version / copyright informations inside Windows properties dialog
- Store last capture inside ``temp`` instead of keeping it inside the application
  - This reduces the memory usage while we completely remove the item from the stack
  - In the future there will be an option to enable / disable this behavior

:bug: Bugfixes:
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Remove artifacts from old snip area when taking another screenshot
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Reduce memory usage after editor is not visible
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Reduce memory usage when GL-acceleration enabled but not visible

<hr>

## 0.1.1

*07 March 2023*

> **This update contains a highly recommended fix of a segfault after the editor window has been closed in systray mode. Please ignore the previous 0.1.0 version!**

![type](https://img.shields.io/badge/type-beta-blueviolet?style=flat-square) ![qtver](https://img.shields.io/badge/qt-5.14.2-blue?style=flat-square&logo=qt)

:rocket: Features / Improvements:
- Make About dialog accessible from SysTray

:bug: Bugfixes:
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Fix segfault on copy after close
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Fix resize handle disappearing after close

<hr>

## 0.1.0

*07 March 2023*

![type](https://img.shields.io/badge/type-beta-blueviolet?style=flat-square) ![qtver](https://img.shields.io/badge/qt-5.14.2-blue?style=flat-square&logo=qt)

:rocket: Features / Improvements:
- **Emojis**
  - Emojis are now available; In the future you will be able to add your own emojis in a custom library. Currently there is only a selected amount of emojis, besides the possibility to add own in the future, there will be some more from time to time :smile:
  - Currently Emojis aren't scalable. You can add, move and delete them but you will not be able to get a resize handle. The item needs a reimplementation which you can track down [here](https://github.com/yeahitsjan/pawxel/issues/3).
- **Autostart**
  - ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows); *Linux still WIP! (expected 0.1.x)*
  - You can now enable / disable autostart using the Preferences dialog
  - **INFO:** This writes a reg key into ``HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run`` on Windows machines. Make sure to delete it (or disable from preferences) before you delete Pawxel from your machine.
- Save last color used on item
  - The last color used is now saved and re-applied to all new items added to the scene (except Marker).
- Exit out Snip mode by right-clicking
  - If you accidentially press your snip shortcut, you can now exit out of the Snip area by just pressing the right button of your mouse.

:bug: Bugfixes:
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Fix graphical issues with color dialog (on all items) in dark mode
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Fix graphical issues with font dialog (on text item) in dark mode
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Fix tooltip not visible on systray icon
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Fix memory not clearing up fully after editor window close

<hr>

## 0.0.4

*02 March 2023*

![type](https://img.shields.io/badge/type-beta-blueviolet?style=flat-square) ![qtver](https://img.shields.io/badge/qt-5.14.2-blue?style=flat-square&logo=qt)

:rocket: Features / Improvements:
- Move relevant parts from source code back into external module ``FHNotifier``
  - https://github.com/yeahitsjan/FHNotifier/commit/c75a526dc53054829a76a1774356cd364dd91872
- *Due to this,* Update ``FHNotifier`` submodule
- Allow resizer item emitting signal when losing focus
  - https://github.com/yeahitsjan/resizer-item/commit/2cd19915e510b39f345b44711fe44d5e22ac68ec
- *Due to this*, Update ``resizer-item`` submodule

:bug: Bugfixes:
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Fix resize handle not hiding when losing focus

<hr>

## 0.0.3

*02 February 2023*

![type](https://img.shields.io/badge/type-beta-blueviolet?style=flat-square) ![qtver](https://img.shields.io/badge/qt-5.14.2-blue?style=flat-square&logo=qt)

:rocket: Features / Improvements:
- Use a fallback font if the default UI font (Josefin Sans) is not available
  - Linux = Cantarell
  - Windows = Segoe UI
- More useful debug output (class + function)
- Remove unused code, reduce code size

:bug: Bugfixes:
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Fix marker height not resizable
- **Prepare** fix resize handle not hiding (fix not implemented as I try out some things via debug output)

<hr>

## 0.0.2

*30 January 2023*

![type](https://img.shields.io/badge/type-beta-blueviolet?style=flat-square) ![qtver](https://img.shields.io/badge/qt-5.14.2-blue?style=flat-square&logo=qt)

:bug: Bugfixes:
- ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Remove strange artifacts when using the snip tool
- ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Change font and font sizes to fit system styles
- ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Fix hiding message box in preferences dialog
- ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux) Some code cleanup

<hr>

## 0.0.1

*30 January 2023*

![type](https://img.shields.io/badge/type-beta-blueviolet?style=flat-square) ![qtver](https://img.shields.io/badge/qt-5.14.2-blue?style=flat-square&logo=qt)

**The changelog entry for the first public beta is a bit different from the entries that will follow in the future. It is intended as a reference for all testers.**

> **WARNING**: *This entry does not evolve. Breaking changes get an extra entry.*

Disabled features:
- **Autostart**
  - Currently not activated because I want to provide a stable tested version first. Checking the box does not change the non-existent function at the moment. You could still create a shortcut / service that starts Pawxel on it's own on the respective platform.
- **Blur**
  - The blur algorithm isn't perfect and sometimes break the whole logic (blurring different part than selected). I currently rework that and assume that it will be ready during the public beta phase. I am sorry for that but I can't publish work that could break the whole app. I think it is not required to have this feature enabled to test out at this phase of the application.
- **Emoji's**
  - I couldn't implement a good and stylish way to display emoji's in a list. Instead of just pushing out the bad idea's I had until today, I will work on that during the beta phase.
- **Window movement on Linux**
  - Window movement currently seems broken on Linux. This could be not directly related to Pawxel. Take a look into [this issue (#4)](https://github.com/yeahitsjan/pawxel/issues/4) for more informations. Currently you are not able to move the editor window.
- **Version check**
  - As already told in the [FAQ](https://pawxel.rocks/faq.html), Pawxel isn't checking for newer versions in beta / nightly versions. The feature isn't even implemented yet and not a priority as of now.

Known Bugs:
- **Snip Area is not fullscreen after display change**
  - ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux)
  - The sniparea does not get resized at the moment if you change your displays at runtime (ex. Windows display settings). This only affects the applications tray mode, as the application isn't restarted. Issue is [already open (#2)](https://github.com/yeahitsjan/pawxel/issues/2).
- **No global shortcuts on Wayland**
  - ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux)
  - It isn't a *bug*, Wayland just disallows global shortcuts. You shouldn't be affected by that on X11. The idea is to implement command line options so Pawxel can be called from there and get an shortcut assigned in Wayland itself. You can discuss that, [here (#1)](https://github.com/yeahitsjan/pawxel/issues/1).
- **Resize handle does not disappear when clicking into the scene**
  - ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux)
  - When resizing an item, the handle isn't disappearing when clicking into the scene. When clicking on another item to resize, the handle bar moves, as expected, to the new item. Also, it isn't rendered when copying or saving the screenshot. So behaviour is correct, hiding should work, but doesn't. The issue is noted (Trello) but not listed on Github.
- **Rotation of items**
  - ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux)
  - The items which should rotate (Distance, Text & Marker), don't rotate at the moment. The implementation does not work as expected on both platforms. This is going to be fixed during beta. Issue is tracked (Trello) but not listed on GitHub.
- **Graphical glitches**
  - ![platform](https://img.shields.io/badge/-windows-darkblue?style=flat-square&logo=windows) ![platform](https://img.shields.io/badge/-linux-black?style=flat-square&logo=linux)
  - A few graphical glitches can appear here and there. The styling is not yet perfectly optimized, so elements like the selection box or the color & font dialog are sometimes rendered incorrectly. This will be fixed in the upcoming beta versions before the first release. The issue is up for tracking (Trello) but I would like to hear from you, were there are some other graphical glitches!

Things we'll need to discuss:
- **Fullscreen screenshot (over multiple monitors)**
  - Pawxel just does a fullscreen screenshot of the current monitor only. This is expected behaviour. Although, we need to discuss it. I follow the same design guideline as Apple does: fullscreen screenshot is just the current monitor, not all.
- **More detailed color info**
  - At the moment, Pawxel just copies the color directly to the clipboard. I think about a floating widget (just like the ones when editing items), that is displayed when pressed ``Alt`` and display the color more detailed (ex. CMYK, RGB, named?).