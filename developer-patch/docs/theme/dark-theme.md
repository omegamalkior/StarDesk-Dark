# Dark theme (Light / Dark / AMOLED)

Everything needed to build a **Light / Dark / AMOLED** theme into the StarDesk Windows client,
with a **live toggle in Settings > General** (above "Start with Windows").

This is the source-level version of the *StarDesk Dark* mod. The mod swaps colours in memory at
launch; this package does the same thing properly inside the client, so it can be maintained and
shipped with future versions.

> Written against the public Qt 5.12+ API and the behaviour of StarDesk client 1.6.0.166
> (Qt 5 Widgets + ui_kit + `client::theme::ThemeColorToken`, 255 tokens). It has not been compiled
> against the StarDesk source tree, which is not public; names of StarDesk classes and widgets
> below are the ones visible in the shipping client.

## What's in here

| Path | What it is |
|---|---|
| `src/client_ui/uikit/theme/theme_palettes.h` | Light, Dark (MuMuPlayer style) and AMOLED colours for all 255 `ThemeColorToken`s, in enum order. Generated. |
| `src/client_ui/uikit/theme/dark_color_maps.h` | The light colours that are written straight into stylesheets (46) and SVG artwork (211), with Dark and AMOLED replacements. Generated. |
| `src/client_ui/uikit/theme/theme_manager.h/.cpp` | Owns the active mode, resolves `@ThemeColor(kToken)`, re-styles every open window on a switch, saves the choice (`QSettings: appearance/theme`). |
| `src/client_ui/uikit/theme/themed_svg.h/.cpp` | Loads SVG icons/backgrounds recoloured for the active mode (black glyphs to white, light boxes to dark, blue to cyan). Also used for `url(:/...svg)` in stylesheets. |
| `src/client_ui/uikit/theme/themed_asset.h/.cpp` | `themedAsset(path)`: the dark / AMOLED version of a picture when the active mode has one. |
| `src/client_ui/shell/features/settings/theme_toggle_row.h/.cpp` | The "Dark mode" settings row with an On/Off switch, plus `insertThemeToggleAbove()`. |
| `src/client_ui/resources/theme/` (`theme_dark.qrc`) | Dark versions of every picture that was drawn for a white UI, at the same resource path as the original: splash screens, title-bar logo, window/taskbar icon, app icon, the QR-code logo (`:/.../qrcode_logo*.png`), and 48 window/dialog background pictures (`:/uikit/common_modal_*bk@4x.png`, `:/remote_boot/common_box_bk@2x.png`, `:/common/message_box/message_box_bk*.png`, menus, toasts, connecting/controlling status windows, file transfer, safe lock, second screen, guides, login, upgrade, recharge). |
| `docs/theme/token_colors.csv` | The three palettes as a spreadsheet, for design review. |
| `docs/theme/hardcoded_colors_dark.csv` | Every hard-coded colour found, with its dark replacement (the to-do list for moving them to tokens). |

## Integrating

The new files build on their own. The call sites in existing files are in
`docs/theme/integration-points.diff`, one hunk per place:

1. **Build**: add the four `.cpp` files and `resources/theme/theme_dark.qrc` to the client target,
   link `Qt5::Svg` and `dwmapi`.
2. **Start-up**: `client::theme::ThemeManager::instance().initialize();` in `main()` after the
   `QApplication` exists and before the first window. It loads the saved choice (default Light).
3. **Token lookup**: wherever ui_kit resolves `@ThemeColor(...)` (the code that stores
   `_uiKitThemeStyleSheetSource` / `_uiKitThemeStyleSheetResolved`), call
   `ThemeManager::instance().resolveStyleSheet(source)`; wherever a token becomes a `QColor` for
   painting, call `ThemeManager::instance().color(token)`. Existing themed widgets then switch live.
4. **Plain `setStyleSheet` calls** with hard-coded colours (`docs/theme/hardcoded_colors_dark.csv`):
   use `ThemeManager::instance().setThemedStyleSheet(w, qss)`, or better, move them to tokens.
5. **SVG icons and control backgrounds**: load through `ThemedSvg::icon()` / `ThemedSvg::pixmap()`;
   `url(:/...svg)` in stylesheets is handled by `resolveStyleSheet()`.
6. **Pictures**: wrap PNG/ICO resource paths in `themedAsset()` (splash, logo, QR logo and the 48
   window/dialog backgrounds; see the commit adding them). Reload pictures that are set once at
   construction on `ThemeManager::modeChanged`.
7. **Custom painting**: read `ThemeManager::instance().color(...)` inside `paintEvent`.
8. **New tokens** for the remote-session tabs and title bar, the Off-switch knob and video toasts
   (values in the diff), then use them in `TabItem`, `VideoContentTitle`, `ToggleSwitch` and
   `NrdToastWidget`.
9. **Settings**: `client::settings::insertThemeToggleAbove(startWithWindowsRow);` in
   `settings_content.cpp`. For AMOLED or Match Windows, call `setMode(ThemeMode::Amoled / System)`
   from a dropdown instead of the switch.
10. **Native window parts** are handled by `ThemeManager`: dark `QPalette` (bare window backgrounds
    = `kFillWindowPanel`, plain labels and text boxes see-through) and `DWMWA_USE_IMMERSIVE_DARK_MODE`
    on every top-level window, which also makes `NrdWindowEffect`'s Mica backdrop dark.

## Accessibility (contrast)

The dark palettes are role-based, not inverted: every token keeps its job (surface, text tier,
accent, overlay) and gets a dark-theme value chosen for that job, the way Windows' own dark theme and
MuMuPlayer do it. All text tokens were then checked against WCAG 2.x AA on every surface they can
land on (sidebar, window, panel and card, including StarDesk's faint whole-window tint):

* Normal text: at least **4.5:1**. Disabled-style tiers (`kTextBlackAlpha77/89`, `kTextGray208/233`
  when used as text): at least **3:1** - WCAG exempts disabled text, but it should still be findable.
* Text tiers follow Windows' dark text ramp: primary near-white, secondary about `#C5FFFFFF`,
  tertiary just above the 4.5:1 line, disabled about 3:1.
* Status colours (red / amber / danger) are lightened in the same hue until they pass on dark.
* Accent buttons follow the Windows dark theme first (then MuMuPlayer where Windows says nothing):
  a bright accent fill (`kFillBrand` = Light2-like cyan) with **dark** text on it
  (`TextOnAccentFillColorPrimary`). Hard-coded accent-button stylesheets already switch to dark
  text. Where code draws `kTextInversePrimary` on an accent fill, add a `kTextOnAccent` token
  (dark in Dark/AMOLED) - `kTextInversePrimary` itself must stay white for text over photos.
* `kTextGray208` and `kTextGray233` are near-white in the light theme and are used as pill/badge
  backgrounds, so they keep dark values and are not treated as text.

`docs/theme/token_colors.csv` has every value, so an automated check (or an accessibility scanner pass
over screenshots) can be rerun after palette edits.

## Following Windows (light/dark mode and accent colour)

* `ThemeMode::System` follows *Settings > Personalization > Colors > Choose your app mode*
  (`AppsUseLightTheme`), and switches live: `ThemeManager` installs a native event filter for
  `WM_SETTINGCHANGE` with `"ImmersiveColorSet"`, the Win32 counterpart of
  `UISettings.ColorValuesChanged` from Microsoft's "Support Dark and Light themes in Win32 apps".
* `setUseSystemAccent(true)` replaces the built-in cyan (Dark) / blue (AMOLED) with the user's
  Windows accent colour. Each accent token has a role in `kAccentRoleDark` / `kAccentRoleAmoled`
  (which `AccentPalette` shade to use). Accent text uses the lightest shade; button fills use Light2 / Light1 /
  Base for rest / hover / pressed with dark text, exactly like Windows' accent buttons in dark mode.
* Suggested Settings UI: a three-way "Appearance: Light / Dark / Match Windows" choice plus a
  "Use my Windows accent colour" checkbox. The existing toggle row still works for Light/Dark.

## Maintaining it

* **New token**: append it to `ThemeColorToken` and add one entry to each array in
  `theme_palettes.h` (same position). Arrays are in enum order and commented with token names.
* **New artwork**: draw it with tokens or `currentColor` where possible. If it must contain fixed
  colours, `ThemedSvg` already handles black glyphs, white boxes and the blue accent; anything else
  goes into `dark_color_maps.h`.
* **Light theme is untouched**: in Light mode every function returns the original colours and files.

## Notes from building the mod

* **Window tint.** Many surfaces sit under small `kFillBlackAlpha*` overlays (hover states and a
  faint window tint). The Dark palette turns low-alpha black overlays into low-alpha *white* ones so
  hovers stay visible on dark backgrounds, which lifts the main surfaces by about 10 levels. The Dark
  palette's base greys are set about 10 levels darker to compensate, aiming for MuMuPlayer's
  `#2F2F2F` sidebar and `#3C3C3C` content. If you give the window tint its own token, you can drop
  that compensation.
* **Text on accent buttons.** MuMuPlayer puts dark text on its cyan buttons. In StarDesk,
  `kTextInversePrimary` is also used for white text over photos ("Connect Desktop", "Ready"), so it
  stays white. Only the hard-coded accent-button stylesheets switch to dark text. A dedicated
  `kTextOnAccent` token would make this cleaner.
* **Off switches and scrollbars.** The off-state outline/knob of `ToggleSwitch` and the scrollbar
  handles use mid-alpha black fills (`kFillBlackAlpha64`-`78`, `115`). `kFillBlackAlpha89` is the
  exception: it is the device status pill ("Ready"), so it gets a subtle grey fill (`#565656`) behind
  white text. Note the pill is painted at reduced opacity and ignores the token's alpha.
* **Remote-session tab bar.** `video_ui::TabItem` paints the selected tab with `kTextInversePrimary`
  and the hovered tab with `kTextGray182` - text tokens that must stay light in dark themes, which
  made the selected tab a white block with invisible white text. Give the tab its own tokens
  (e.g. `kFillTabSelected`, `kFillTabHover`); the mod points them at `kFillSolidWhite` /
  `kFillPanelBackground` (Dark) and `kBorderGray240` / `kFillSolidWhite` (AMOLED). The bar behind the
  tabs is a separate widget; see "Remote-session title bar" below.
* **Off-switch knob.** `ToggleSwitch` draws the Off knob with `kFillBlackAlpha153`, a dark overlay
  that disappears on a dark track. Give the knob its own token; the mod draws it with `kTextGray140`
  (`#C0C0C0`) for the knob only, matching Windows' and MuMuPlayer's light Off knob.
* **"Ready" device status pill** is filled with `kFillDarkGray25` (`#4E4E4E` in Dark) behind white
  text; `kFillBlackAlpha89` is set to `#565656` for the same reason.
* **Dimming layers.** `kFillBlackAlpha77` is not a hover fill: `GlobalMessageFrame`, `ModalBoxFrame`,
  `ErrorMsgBox`, `QuitQuestionBox` and `VideoMessageFrame` paint it over the window behind a message box.
  It must stay black - the mod uses 50 % black (`#80000000`), like `kBorderBlackAlpha204` behind dialogs.
  Light dimming over a dark UI looked like a white veil.
* **Scrollbar handles and switch outlines** use the mid-alpha black fills (`kFillBlackAlpha64`, `76`,
  `78`, `115`); in Dark/AMOLED they are light or white at the same alpha so they stay visible.
* **Video toasts** (`NrdToastWidget`, e.g. "Optimizing route...") use `kFillSolidWhite`; the mod uses
  `kBorderGray240` instead so they read as a dark flyout (Windows' flyout grey).
* **Text coloured with fill tokens.** A few stylesheets use `color: @ThemeColor(kFillBlackAlpha230)`
  (verification code, some counters). `resolveStyleSheet()` maps those to the matching `kTextBlackAlpha*`
  token in dark modes; better to fix the stylesheets themselves.
* **Glyphs inside icons.** Pale fills drawn *on* a coloured icon (the star in the Favorite Device
  bookmark, `fill="#DEF0FF"`) are glyphs, not backgrounds: they stay light (white) in dark modes.
* **Web views** (Feedback, message centre, cloud devices, purchase history and the skeleton pages
  under `bin/html/skeleton`) are WebView2 pages. In dark modes pass
  `--force-dark-mode --enable-features=WebContentsForceDark` as additional browser arguments and set
  the profile's `PreferredColorScheme` (see `integration-points.diff`): pages with their own dark
  style use it, the rest get Chromium's automatic dark rendering. Long term, give those pages a real
  `prefers-color-scheme: dark` style.
* **Dialog frames.** The background pictures behind popups are recoloured to `#363636` (AMOLED
  `#090909`), slightly darker than the dialog surface: the popup content is drawn under a faint dim
  and the picture is not, so this is what makes the 10 px frame disappear into the dialog.
* **Wordmark.** The "STARDESK" wordmark next to the star (Contact us, About:
  `:/stardesk/about_logo_en_new.png`, `logo_with_name.png`) has white letters and the dark star in
  dark modes.
* **Fixed-colour stylesheets.** The Get Started / sign-in windows, Delete Account and the icon+text
  buttons ("Continue with Google / Email") write their colours straight into the stylesheet text
  (`rgba(0,0,0,0.9)`, `rgba(255,255,255,255)`, `#3A7BFC`). Switch them to `@ThemeColor(...)` tokens.
* **White flash when a window opens.** Qt registers its window classes with
  `GetSysColorBrush(COLOR_WINDOW)` (white), and Windows shows that brush until the first frame is
  drawn. In dark themes give StarDesk's windows a dark class brush before they are shown, e.g. in
  `ThemeManager::eventFilter` on `QEvent::WinIdChange`:
  `SetClassLongPtrW(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)darkBrush)` with a brush of `kFillWindowPanel`.
* **Icons that keep their original colours.** Cursors drawn over the remote screen, the yellow warning
  triangles, and artwork that is already dark (status pills, the pressed pin, screensaver upload
  buttons) are excluded from SVG recolouring; icons with a white tile behind a glyph (floating ball,
  popup arrows, the "forbidden" badge, the lock-screen box) get a dark tile (`themed_svg.cpp`).
* **Remote-session title bar.** The strip behind the remote-session tabs is `video_ui::VideoContentTitle`,
  which fills itself with `kBorderCoolGray` - a *border* token, light grey in dark themes so borders
  stay visible. Give the bar its own token, `kFillRemoteTitleBar` (Light `#D7DCE0` as today, Dark
  `#242424`, AMOLED `#000000`). Dark uses `#242424` rather than the main title bar's `#323232` because
  the remote-session window is shown brighter than the main window on HDR displays; on the test
  display (Dell AW3423DW, HDR on) `#242424` there looks the same as `#323232` in the main window.
  It is not Windows' caption colour: setting `DWMWA_CAPTION_COLOR` does not change the bar.
* **Remote icons.** The Contact us links (Discord, Reddit, Facebook, X) come from the server
  (`windows_contact_config`, `icon_url`). Monochrome black logos such as X disappear on a dark
  background; the config needs a light variant (e.g. `icon_url_dark`) for dark modes. Separately, `ContactViewItem::slotIconPixmapReply` ignores a failed image decode, which is why the Reddit icon shows as an empty space; see `integration-points.diff`.

## Quick test checklist

Home (device card bar, Quick Launch, Ready badge, Connect Desktop) - Settings (all tabs, dropdowns,
switches, Language row) - splash at start-up - file transfer window - remote-support dialogs -
toasts and tooltips - message boxes and their dimming layer (e.g. "Quit setting?" over Check Network
Adapter Status), the privacy-overlay message box, Complete Verification (QR logo) - Favorite Devices
icon - each in Light, Dark and AMOLED, switching live from the new row. Then Match Windows (flip
Windows' app mode while StarDesk is open), the accent checkbox, and a remote session window
(selected / hovered tab, tab bar, connecting window).
