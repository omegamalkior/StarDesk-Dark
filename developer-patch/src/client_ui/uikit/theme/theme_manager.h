// theme_manager.h - Light / Dark / AMOLED theming for the StarDesk client, switchable live.
//
// Drop-in companion to client::theme::ThemeColorToken. It owns the active palette, resolves
// "@ThemeColor(kToken)" stylesheets, and re-styles every open window when the mode changes,
// so the Settings toggle takes effect immediately without restarting the client.
//
// Requires Qt 5.12+ (Core, Gui, Widgets, Svg).
#pragma once

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtCore/QAbstractNativeEventFilter>

class QWidget;

namespace client {
namespace theme {

// System follows Windows' "Choose your app mode" (Light / Dark) and changes live with it.
enum class ThemeMode { Light = 0, Dark = 1, Amoled = 2, System = 3 };

class ThemeManager : public QObject {
    Q_OBJECT
public:
    static ThemeManager &instance();

    // Call once from main(), after QApplication is created and before the first window is shown.
    // Loads the saved choice (default: Light) and applies it.
    void initialize();

    ThemeMode mode() const { return mode_; }            // what the user picked (may be System)
    ThemeMode effectiveMode() const { return effective_; } // what is on screen right now
    bool isDark() const { return effective_ != ThemeMode::Light; }

    // Use the Windows accent colour (Settings > Personalization > Colors) instead of the built-in
    // cyan / blue, using the same shades Windows uses in dark mode (accent buttons = Light2).
    void setUseSystemAccent(bool on);
    bool useSystemAccent() const { return useAccent_; }

    // Re-reads the Windows light/dark mode and accent colour. Called automatically when Windows
    // broadcasts WM_SETTINGCHANGE("ImmersiveColorSet") - the Win32 counterpart of
    // UISettings.ColorValuesChanged - so System mode and the accent follow Windows live.
    void refreshFromSystem();

    // Switches the whole client live and remembers the choice for next launch.
    void setMode(ThemeMode mode);
    void setDark(bool dark) { setMode(dark ? ThemeMode::Dark : ThemeMode::Light); }

    // Colour of a token in the active palette. Works with the existing enum:
    //   ThemeManager::instance().color(ThemeColorToken::kFillSolidWhite)
    QColor color(int tokenIndex) const;
    template <typename Token> QColor color(Token token) const { return color(static_cast<int>(token)); }

    // Turns "background: @ThemeColor(kFillSolidWhite);" into concrete colours for the active mode,
    // and (in dark modes) also swaps the hard-coded light colours listed in dark_color_maps.h.
    QString resolveStyleSheet(const QString &source) const;

    // Use instead of QWidget::setStyleSheet for anything containing @ThemeColor(...) or a
    // hard-coded colour. The source text is remembered so the widget re-styles itself on a switch.
    // (Uses the same "_uiKitThemeStyleSheetSource/Resolved" properties ui_kit already stores.)
    void setThemedStyleSheet(QWidget *widget, const QString &source);

    // Re-applies the active mode to every existing widget. setMode() calls this for you.
    void reapplyAll();

signals:
    // Emitted after a switch. Custom-painted widgets and anything caching pixmaps
    // (icons made with ThemedSvg, the splash) should refresh here.
    void modeChanged(client::theme::ThemeMode mode);

    // Gives one top-level window a dark (or light) native frame: DWMWA_USE_IMMERSIVE_DARK_MODE, which also
    // makes Mica / acrylic backdrops (NrdWindowEffect) dark. Called for every window on show and on a switch.
    void applyNativeFrame(QWidget *window) const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    ThemeManager();
    void applyApplicationPalette();
    const unsigned int *table() const;

    void rebuildTable();
    void applyEverything();

    ThemeMode mode_ = ThemeMode::Light;
    ThemeMode effective_ = ThemeMode::Light;
    bool useAccent_ = false;
    bool initialized_ = false;
    unsigned int table_[255] = {};        // active palette, with the accent applied
    unsigned char accent_[8][4] = {};     // Windows AccentPalette, Light3 .. Dark3
    bool haveAccent_ = false;
    QHash<QString, int> tokenIndex_;   // "kFillSolidWhite" -> index
};

}  // namespace theme
}  // namespace client
