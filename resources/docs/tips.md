# Tips and Tricks

## **Debug Mode**

If you encounter problems using Pawxel, the debug mode could help you either create an [issue](https://github.com/yeahitsjan/pwx-rel/issues) or guide you, what's going wrong. To enable the debug mode you'll need to modify your preferences manually as it isn't available as a standard entry from the preferences dialog.

Open your ``prefs.ini`` from ``~/.pawxel/`` and add the following entry at the bottom:
```ini
$debug_mode = true
```

> ***NOTE:** To disable debug mode you'll need to fully remove this line from the configuration file. Just changing from ``true`` to ``false`` won't bring the expected effect.*

> ***NOTE:** Enabling debug mode increases the memory usage from around 8MB to up to 30MB (when idle) as specific informations are collected. If you see suspicious memory usage please first make sure you disabled the debug mode before you create an issue.*

If you are using Pawxel in systray mode you'll need to restart the application for the changes to apply. The next time you start Pawxel, a ``debug.log`` is created inside ``~/.pawxel/``. You can use that file for troubleshooting.

## **(Default) shortcuts**

When using Pawxel in systray mode (normal from preferences), it automatically registers global shortcuts during launch. The default ones are:

- ``Ctrl+Space`` - request the snip tool
- ``Ctrl+F11`` - do a fullscreen screenshot

If these interfer with the applications you use or you prefer different shortcuts, you can simply change them:

1. Open up the ``Preferences`` either from the System tray or inside the Editor window.
2. Click the Keyboard icon on top of the dialog.
3. Click onto any shortcut you want to change and press the shortcut you want on your keyboard.
4. The new shortcut is automatically saved and changed. You can directly close the dialog.

## **Editing Shortcuts**

Pawxel gives you a variety of tools to edit your screenshot. To not interfer with the visuals and keeping the application as simple as possible, I minimized the way you interact with items on your screenshots.

To **add** an item onto the screenshot:
1. Locate the Action bar (left) of the Editor window.
2. Click on the item you want to add to the screenshot (they all got tooltips).
3. The item is added to the screenshot at a default position.

**Move** items on the screenshot:
1. Left click on the item you want to move around.
2. Drag the item with the mouse to the position you want.

**Resize** items:
1. Double click the item you want to resize.
2. A resize handle should appear around the item *(does not work on all items)*.
3. Resize the item via the resize handle.

**Edit** items:
1. Right click the item you want to edit.
2. Depending on the item (either graphical or text) a small widget pops up:
   1. *Graphical*: You can change the color by clicking the big color button. Increase / decrease width of the border by dragging the scrollbar.
   2. *Text*: You can change the text by typing inside the edit box. Change the color over the big color button and change font & font size by clicking on the current font's name.
3. Click somewhere on the Editor window / screenshot. The widget closes automatically when it looses focus.

**Delete** an item:
1. Right click the item you want to delete.
2. Press ``Delete`` or ``Del`` on your keyboard.

Here is a list of tools and their features:
- **Rectangle** - move, resize, edit, delete
- **Circle** - move, resize, edit, delete
- **Text** - move, edit, delete
- **Marker** - move, resize, edit, delete
- **Distance** - move, resize, edit, delete
- **Blur** - delete