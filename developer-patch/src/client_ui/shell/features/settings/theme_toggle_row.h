// theme_toggle_row.h - "Dark mode" row for Settings > General, placed above "Start with Windows".
//
// Self-contained (plain Qt) so it builds as-is; it follows the look of the existing setting rows
// (icon, title, description, "On/Off" label and a switch). If you prefer, swap ThemeSwitch for
// ui_kit::ToggleSwitch - only onToggled() needs connecting.
//
//   // in the code that builds Settings > General (settings_content.cpp):
//   client::settings::insertThemeToggleAbove(startWithWindowsRow);
#pragma once

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QWidget>

class QLabel;

namespace client {
namespace settings {

// Small animated on/off switch drawn with theme colours (track = kFillBrand when on).
class ThemeSwitch : public QAbstractButton {
    Q_OBJECT
public:
    explicit ThemeSwitch(QWidget *parent = nullptr);
    QSize sizeHint() const override { return QSize(52, 28); }

protected:
    void paintEvent(QPaintEvent *event) override;
};

class ThemeToggleRow : public QWidget {
    Q_OBJECT
public:
    explicit ThemeToggleRow(QWidget *parent = nullptr);

private:
    void syncFromTheme();
    void onToggled(bool on);
    void restyle();

    QLabel *icon_ = nullptr;
    QLabel *title_ = nullptr;
    QLabel *description_ = nullptr;
    QLabel *state_ = nullptr;
    ThemeSwitch *switch_ = nullptr;
};

// Inserts the Dark mode row (as its own card) directly above the card holding `startWithWindowsRow`.
// Returns the new row, or nullptr if the layout could not be found.
ThemeToggleRow *insertThemeToggleAbove(QWidget *startWithWindowsRow);

}  // namespace settings
}  // namespace client
