StarDesk Dark - easy patch
==========================

What it does
------------
Makes StarDesk open in a dark theme automatically:
  * your normal StarDesk shortcuts (desktop, Start menu, taskbar) open StarDesk in dark mode
  * StarDesk switches to dark mode a few seconds after you sign in to Windows
  * adds "StarDesk Dark" and "StarDesk Dark Settings" to the Start menu
  * if StarDesk is already open in light mode, the shortcut closes it and reopens it dark;
    if it is already dark, the shortcut just brings it to the front

Three choices in Start > StarDesk Dark Settings:
  * Dark - MuMuPlayer style (neutral greys, cyan accents)
  * AMOLED - pure black
  * Match Windows - Dark when Windows is in dark mode, StarDesk's normal look when it's light
There's also a "Use my Windows accent colour" checkbox, which swaps the cyan/blue for the accent
colour you picked in Windows (Settings > Personalization > Colors). A changed Windows mode or accent
takes effect the next time you open StarDesk Dark.

What gets darkened
------------------
  * all of StarDesk's colours (windows, lists, buttons, switches, tabs, text)
  * popup and dialog backgrounds, message boxes, menus, toasts, notification frames, the connecting /
    controlling status windows, file transfer, safe lock, second screen, login and upgrade windows
  * icons that were drawn for a white background (the star logo, QR-code logo, splash screens)
  * Windows' own parts: popup window edges and backdrops follow dark mode, and plain text boxes
    and labels that StarDesk doesn't colour itself get a dark background instead of white
  * the remote-session window: tab bar and title bar match the main window's dark grey
  * StarDesk's web pages (Feedback, message centre, cloud devices) are shown in dark mode
  * the STARDESK name next to the logo (Contact us, About) is white, with the dark star
  * the Get Started sign-in screen: dark buttons with light text
  * the floating control ball, popup arrows, lock-screen box, remote-boot button, guest avatar and
    trial popup are dark; the Delete Account agreement text is light
  * no white flash when StarDesk's windows open (they start dark)
  * things that should keep their look do: remote-screen cursors and the yellow warning signs
  * Contact us icons are downloaded at full size (the server's shrunk copy of Reddit's icon showed blank)
Screenshots and illustrations inside StarDesk (setup guides, network-card help pictures, QR codes)
are left as they are.

Colours follow Microsoft's Windows dark-theme guidance first (text levels, subtle fills, bright
accent buttons with dark text), and MuMuPlayer's look where Microsoft has no rule (greys, the
remote-session tab bar). All text meets the WCAG AA contrast guidelines (4.5:1 for normal text) on
every background it appears on.


Nothing inside StarDesk is modified on disk. Each time StarDesk starts, the launcher swaps its
colours in memory, so StarDesk updates keep working and you can go back to light at any time.

Install
-------
1. Double-click "Install StarDesk Dark.cmd".
2. Click Yes when Windows asks for permission (needed to copy into the StarDesk folder).
3. Answer Y if you want StarDesk to open in dark mode right away.

Uninstall
---------
Double-click "Uninstall StarDesk Dark.cmd". Your original shortcuts are put back and the dark
launcher is removed. Restart StarDesk to see it light again.

After a StarDesk update
-----------------------
The dark theme keeps working on newer versions as long as StarDesk keeps its colour system. If an
update reinstalls StarDesk's shortcuts and they open light again, run the installer once more.
If an update changes StarDesk's colours too much, the launcher says so and opens StarDesk normally
(light) instead of breaking anything.

Files
-----
  StarDeskDark.exe              the dark launcher (also the dark star icon)
  Install StarDesk Dark.cmd     installer (runs install.ps1 as administrator)
  Uninstall StarDesk Dark.cmd   uninstaller (runs uninstall.ps1 as administrator)
  install.ps1 / uninstall.ps1   the actual install steps, readable in Notepad

Tested against StarDesk 1.6.0.166 for Windows (64-bit, Windows 10 and 11).

Things it keeps in %LOCALAPPDATA%\StarDesk Dark
  * a tiny background helper. It sleeps until StarDeskDark.exe is replaced by a newer version, then
    restarts StarDesk and brings its window back up, so the update shows right away.
  * platforms\qwindows.dll - a copy of StarDesk's own Qt "Windows" component with its dark-mode
    check set to "dark", so plain boxes StarDesk doesn't colour itself are dark too, and new windows
    start dark instead of flashing white. StarDesk's own
    copy is not touched; the copy is refreshed automatically when StarDesk is updated.
The uninstaller stops the helper and removes that folder.
