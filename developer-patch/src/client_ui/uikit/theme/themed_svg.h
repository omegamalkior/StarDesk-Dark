// themed_svg.h - loads StarDesk's SVG artwork in the colours of the active theme.
//
// StarDesk's icons and control backgrounds (dropdowns, inputs, the device-card bar) are SVGs drawn
// in light-theme colours: black glyphs, white/light-grey boxes, blue accents. In Dark/AMOLED mode
// this recolours them on load with the same rules the StarDesk Dark mod uses, so no second set of
// SVG files has to be maintained. Results are cached per theme and the cache is dropped on a switch.
//
//   label->setPixmap(ThemedSvg::pixmap(":/uikit/combobox_bg_normal.svg", QSize(170, 32)));
//   button->setIcon(ThemedSvg::icon(":/settings/language.svg"));
//
// Stylesheets that reference artwork with url(:/...svg) are handled automatically:
// ThemeManager::resolveStyleSheet() points them at a recoloured copy (see themedFile()).
#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

namespace client {
namespace theme {

class ThemedSvg {
public:
    // Recolour raw SVG text for the active theme (returns it unchanged in Light mode).
    static QByteArray recolor(const QByteArray &svg);

    static QByteArray load(const QString &resourcePath);                  // recoloured SVG text
    static QPixmap pixmap(const QString &resourcePath, const QSize &size, qreal dpr = 1.0);
    static QIcon icon(const QString &resourcePath);

    // Path of a recoloured copy of a ":/...svg" resource in the app's cache folder (written on first
    // use). In Light mode it returns the resource path unchanged. Used for stylesheet url(...).
    static QString themedFile(const QString &resourcePath);

    static void clearCache();
};

}  // namespace theme
}  // namespace client
