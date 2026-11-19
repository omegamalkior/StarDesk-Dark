# Add Dark and AMOLED themes with a toggle that can switch between them

**`feature/dark-theme` → `main`** · 13 commits · 157 files changed · +3,222 lines

## Summary

Adds a **Dark** theme (neutral greys and cyan accents, in the style of MuMuPlayer) and an **AMOLED** theme (pure black) to the Windows client. Users turn it on from a new **Dark mode** row in *Settings > General*, just above *Start with Windows*. The switch takes effect immediately and is remembered for the next launch. The **Light** theme is unchanged: in Light mode every new function returns today's colours and files.

The client also gets two optional settings:
- *Match Windows*, which follows Windows' light/dark app mode live.
- *Use my Windows accent colour*, which replaces the built-in cyan with the user's Windows accent colour.

This is the source-level version of the *StarDesk Dark* mod, which has been running against the shipping 1.6.0.166 client. Every colour and picture here was checked on screen through that mod.

## Why

StarDesk is light-only. Many people use Windows in dark mode, and a bright remote-desktop window next to dark apps is hard on the eyes at night. The client already routes almost every colour through `client::theme::ThemeColorToken` (255 tokens), so a dark theme is mostly a second set of values plus a way to switch between them.

## What changes

### New code (builds on its own)

| File | What it does |
|---|---|
| `src/client_ui/uikit/theme/theme_palettes.h` | Light / Dark / AMOLED values for all 255 tokens, in enum order, plus each token's accent role. Generated. |
| `src/client_ui/uikit/theme/dark_color_maps.h` | Light colours written straight into stylesheets (46) and SVGs (211), with their dark replacements. Generated; a stop-gap until they become tokens. |
| `src/client_ui/uikit/theme/theme_manager.h/.cpp` | The active mode. Resolves `@ThemeColor(...)`, re-styles every open window on a switch, follows Windows' mode and accent, sets the dark `QPalette`, and gives every window a dark native frame. |
| `src/client_ui/uikit/theme/themed_svg.h/.cpp` | SVG icons and control backgrounds recoloured for the active mode, cached per mode. |
| `src/client_ui/uikit/theme/themed_asset.h/.cpp` | `themedAsset(path)`: the dark / AMOLED version of a PNG or ICO when there is one. |
| `src/client_ui/shell/features/settings/theme_toggle_row.h/.cpp` | The *Dark mode* settings row. |
| `src/client_ui/resources/theme/` + `theme_dark.qrc` | Dark and AMOLED versions of 71 pictures (142 files): the splash screens, logo, the STARDESK wordmark, icons, the QR-code logo and 48 window/dialog backgrounds. Each keeps its original resource path. |
| `docs/theme/` | Design notes, the colour tables as CSV, and the call sites below. |

### Changes to existing files

Listed in `docs/theme/integration-points.diff`, one hunk per call site:

- **`main.cpp`**: `ThemeManager::instance().initialize()` once the `QApplication` exists.
- **ui_kit's token lookup**:
  - `@ThemeColor` resolution goes through `ThemeManager::resolveStyleSheet()`.
  - token → `QColor` goes through `ThemeManager::color()`.
  - Existing themed widgets then switch live with no other changes.
- **Five new tokens**:
  - `kFillTabSelected` / `kFillTabHover`, used by `TabItem`.
  - `kFillSwitchKnobOff`, used by `ToggleSwitch`.
  - `kFillToast`, used by `NrdToastWidget`.
  - `kFillRemoteTitleBar`, used by `VideoContentTitle` (the remote-session title bar).
- **`modal_box_frame.cpp` and other picture loads** go through `themedAsset()`.
- **`nrd_window_effect.cpp`** marks the window dark before turning on Mica.
- **`webview_host_impl.cpp`** asks WebView2 for dark mode (Feedback and the other web pages).
- **Full audit of every stylesheet, colour token, icon and picture** in Dark and AMOLED: remote-screen cursors and warning triangles keep their original colours, white-tile icons get dark tiles, the icon+text button labels and the Delete Account agreement text are light.
- **Every hook-in point was checked against the 1.6.0.166 binary** and is marked `[verified]` in `integration-points.diff` with what was found (which token each function reads, where fixed colours are written).
- **`contact.cpp`**: the icon loader ignores a failed image decode, which is why the Reddit icon shows as an empty space; retry without the resize query and fall back to a built-in icon.
- **Get Started / sign-in, Delete Account, icon+text buttons**: their stylesheets have fixed colours; switch them to `@ThemeColor` tokens (listed in `hardcoded_colors_dark.csv`).
- **`settings_content.cpp`** inserts the new row.
- **CMake**: new sources, the `.qrc`, `Qt5::Svg` and `dwmapi`.

## Problems this fixes that a plain palette swap would not

Each of these showed up on screen while the mod was being built:

- **White frame around every dialog.** Popups draw their content 10 px inside a background picture (`:/uikit/common_modal_bk@4x.png` and others). These pictures are a light panel with a shadow, so 48 of them get dark versions.
- **Light Mica popups.** `NrdWindowEffect` turns on Mica but never sets `DWMWA_USE_IMMERSIVE_DARK_MODE`, so the backdrop and the 1 px edge stay light. `ThemeManager` now sets it on every top-level window.
- **White or black slabs behind plain labels and text boxes.** Examples are the title of the small Confirm/Cancel boxes and the privacy-overlay message box. These widgets have no stylesheet and use `QPalette::Base`. In dark modes `Base` is now transparent.
- **Light remote-session title bar.** `VideoContentTitle` filled the bar with `kBorderCoolGray`, a border colour that stays light grey in dark themes. It now uses its own token, `kFillRemoteTitleBar` (Dark `#242424`, AMOLED black), and matches the main window's title bar. Verified on a running 1.6.0.166.
- **White selected tab with invisible text.** `TabItem` paints with text tokens that must stay white. New tab tokens fix this.
- **Invisible Off-switch knob.** It was drawn with a dark overlay token. A new knob token draws it light grey.
- **Light veil behind message boxes.** `kFillBlackAlpha77` is the dimming layer painted by `GlobalMessageFrame`, `ModalBoxFrame` and others. It stays black (50 %).
- **Unreadable verification code.** It was coloured with a fill token. `resolveStyleSheet()` maps `color: @ThemeColor(kFillBlackAlphaNNN)` to the matching text token.

## Accessibility

The dark palettes are role-based rather than inverted. Values follow Microsoft's Windows dark-theme guidance first, and MuMuPlayer where Windows has no rule.

**Text contrast (WCAG 2.x AA)**
- Normal text is at least **4.5:1** on every surface it can appear on.
- Disabled-style tiers are at least **3:1**.

**Accent buttons**
- A bright accent fill with dark text, as in Windows' own dark theme.

**Status colours**
- Red, amber and danger are lightened in the same hue until they pass.

`docs/theme/token_colors.csv` holds every value, so the checks can be rerun after edits.

## How to test

1. Settings > General → turn on **Dark mode**: every open window switches immediately. Restart: still dark.
2. Walk through these screens in Light, Dark and AMOLED:
   - Home: device cards, *Ready* pill, Quick Launch, Favorite Devices icon.
   - Settings: all tabs, dropdowns and switches, including the Off state.
   - Splash and tray menu.
   - Message boxes and their dimming layer, e.g. *Quit setting?* over *Check Network Adapter Status*.
   - Privacy-overlay message box.
   - *Complete Verification* (QR logo), *Contact us* and *About* (wordmark), *Feedback* (web view).
   - Favorite Devices with no devices (white star on the box).
   - *Get Started* sign-in (dark buttons, white text).
   - File transfer, toasts and tooltips.
   - A remote session: tab bar, selected and hovered tabs, connecting window, in-session menu.
3. *Match Windows*: flip Windows' app mode while StarDesk is open.
4. *Use my Windows accent colour*: change the accent in Windows Settings.
5. Light mode: no visual change from `main`.

## Notes for reviewers

- **Not compiled against your tree.** This was written without access to the StarDesk source. The new files use only the public Qt 5.12+ API and the class and token names visible in the 1.6.0.166 client.
- **Approximate hunks.** The hunks in `integration-points.diff` are anchored by function, not by line. Paths marked *[inferred]* are where we expect the class to live.
- **Generated headers.** `theme_palettes.h` and `dark_color_maps.h` are generated. To add a token later, append one entry to each array in enum order.
- **Web views.** Feedback, message centre, cloud devices and purchase history are WebView2 pages. In dark modes they get Chromium's dark mode through the WebView2 environment (call site in the diff). Pages without their own dark style are darkened automatically, so a real `prefers-color-scheme: dark` style for them would look better.
- **Contact us icons** come from the server (`icon_url`). Black logos such as X disappear on dark; the config needs light variants.

## Applying the branch

```sh
git checkout -b feature/dark-theme
git am patches/*.patch
```

Then place the hunks from `docs/theme/integration-points.diff`.
