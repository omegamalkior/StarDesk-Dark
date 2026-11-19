// themed_svg.cpp - see themed_svg.h
#include "themed_svg.h"

#include "dark_color_maps.h"
#include "theme_manager.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QHash>
#include <QtCore/QRegularExpression>
#include <QtCore/QStandardPaths>
#include <QtGui/QPainter>
#include <QtSvg/QSvgRenderer>

namespace client {
namespace theme {

namespace {
QHash<QString, QByteArray> &svgCache() { static QHash<QString, QByteArray> c; return c; }

const char *boxColor(ThemeMode m) { return m == ThemeMode::Amoled ? "#222" : "#555"; }
}  // namespace

QByteArray ThemedSvg::recolor(const QByteArray &svg) {
    const ThemeMode mode = ThemeManager::instance().effectiveMode();
    if (mode == ThemeMode::Light) return svg;
    const bool amoled = (mode == ThemeMode::Amoled);
    QString s = QString::fromUtf8(svg);

    // 1. Control backgrounds: white boxes at least 40x16 and mostly opaque
    //    (dropdowns, inputs, buttons, the device-card bar) -> dark box colour.
    static const QRegularExpression rectRe(QStringLiteral("<rect\\b[^>]*>"));
    static const QRegularExpression wRe(QStringLiteral("\\bwidth=\"([\\d.]+)\""));
    static const QRegularExpression hRe(QStringLiteral("\\bheight=\"([\\d.]+)\""));
    static const QRegularExpression opRe(QStringLiteral("fill-opacity=\"([\\d.]+)\""));
    QString out;
    int pos = 0;
    auto it = rectRe.globalMatch(s);
    while (it.hasNext()) {
        const auto m = it.next();
        QString tag = m.captured(0);
        const auto w = wRe.match(tag), h = hRe.match(tag), op = opRe.match(tag);
        const bool isBox = w.hasMatch() && h.hasMatch() && w.captured(1).toDouble() >= 40 && h.captured(1).toDouble() >= 16;
        const bool opaque = !op.hasMatch() || op.captured(1).toDouble() >= 0.5;
        if (isBox && opaque && tag.contains(QLatin1String("fill=\"white\"")))
            tag.replace(QLatin1String("fill=\"white\""), QStringLiteral("fill=\"%1\"").arg(QLatin1String(boxColor(mode))));
        out += s.midRef(pos, m.capturedStart() - pos);
        out += tag;
        pos = m.capturedEnd();
    }
    out += s.midRef(pos);
    s = out;

    // 2. Frosted panels (white gradient over a backdrop blur, e.g. the bar under the device picture).
    if (s.contains(QLatin1String("backdrop-filter")))
        s.replace(QLatin1String("stop-color=\"white\""), QStringLiteral("stop-color=\"%1\"").arg(QLatin1String(amoled ? "#000" : "#222")));

    // 2b. common_box_bk.svg (:/device/common, :/device/alldevice): the box background's white top half
    //     showed through as a light band behind the box title.
    s.replace(QLatin1String("V70H41V48Z\" fill=\"white\""),
              QStringLiteral("V70H41V48Z\" fill=\"%1\"").arg(QLatin1String(amoled ? "#0c0c0c" : "#464646")));

    // 3. Every other hard-coded colour: black glyphs -> white, light fills -> dark, blue -> cyan.
    for (int i = 0; i < maps::kSvgSwapsCount; ++i) {
        const auto &sw = maps::kSvgSwaps[i];
        s.replace(QLatin1String(sw.from), QLatin1String(amoled ? sw.amoled : sw.dark));
    }
    return s.toUtf8();
}

namespace {
// Artwork that must not be recoloured: cursors drawn over the remote screen, Windows-style warning
// triangles (black "!" on yellow), and pieces that are already dark (status pills, pressed pin,
// screensaver upload buttons).
bool keepOriginal(const QString &p) {
    static const char *const kKeep[] = {
        "/control/viewing_cursor_", "/common/message_box/warning_icon.svg", "/device/alldevice/error_icon.svg",
        "/multi_screen/mac_no_support_warning.svg", "/notify_center/file_transfer_warning.svg",
        "/device/desktop/device_online.svg", "/device/desktop/device_occupied.svg", "/device/desktop/device_connected.svg",
        "/device/common/pinned_clicked.svg", "/control/privacy_screen/upload_custom_screensaver_"};
    for (const char *k : kKeep) if (p.contains(QLatin1String(k))) return true;
    return false;
}
// Icons with a white tile behind a glyph (floating-ball hover/pressed, popup arrows, "forbidden" badge,
// the lock-screen box): the glyph turns light, so the tile has to turn dark as well.
QByteArray darkTiles(const QString &p, QByteArray svg) {
    static const QRegularExpression tiles(QStringLiteral("/control/ball_(hover|clicked|collapse_\\w+_(hover|clicked))\\.svg$|"
                                                         "/common/popup/popup_|/apps_manager/forbidden\\.svg$|/safe_lock/client_lock_on_normal\\.svg$"));
    if (!ThemeManager::instance().isDark() || !tiles.match(p).hasMatch()) return svg;
    const bool amoled = ThemeManager::instance().effectiveMode() == ThemeMode::Amoled;
    const bool hover = p.contains(QLatin1String("hover"));
    const QByteArray tile = amoled ? (hover ? "#222222" : "#111111") : (hover ? "#555555" : "#444444");
    const int defs = svg.indexOf("<defs");                          // leave clip paths alone
    QByteArray head = defs < 0 ? svg : svg.left(defs), tail = defs < 0 ? QByteArray() : svg.mid(defs);
    head.replace("fill=\"white\"", "fill=\"" + tile + "\"");
    return head + tail;
}
}  // namespace

QByteArray ThemedSvg::load(const QString &resourcePath) {
    const QString key = QString::number(static_cast<int>(ThemeManager::instance().effectiveMode())) + resourcePath;
    auto &cache = svgCache();
    auto hit = cache.constFind(key);
    if (hit != cache.constEnd()) return *hit;
    QFile f(resourcePath);
    QByteArray data;
    if (f.open(QIODevice::ReadOnly)) {
        QByteArray raw = f.readAll();
        // The star on the empty Favorite Devices box is a glyph, not a background: keep it white
        // (like the sidebar's Favorite Device star) instead of letting the pale fill go dark.
        if (ThemeManager::instance().isDark() && resourcePath.endsWith(QLatin1String("devicelist/empty_favorites.svg"))) {
            const int at = raw.lastIndexOf("fill=\"#DEF3FF\"");
            if (at >= 0) raw.replace(at, 14, "fill=\"#FFFFFF\"");
        }
        data = keepOriginal(resourcePath) ? raw : darkTiles(resourcePath, recolor(raw));
    }
    cache.insert(key, data);
    return data;
}

QPixmap ThemedSvg::pixmap(const QString &resourcePath, const QSize &size, qreal dpr) {
    QSvgRenderer renderer(load(resourcePath));
    QPixmap pm(size * dpr);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    renderer.render(&p);
    p.end();
    pm.setDevicePixelRatio(dpr);
    return pm;
}

QIcon ThemedSvg::icon(const QString &resourcePath) {
    QIcon icon;
    QSvgRenderer renderer(load(resourcePath));
    const QSize base = renderer.defaultSize().isValid() ? renderer.defaultSize() : QSize(24, 24);
    for (qreal dpr : {1.0, 1.5, 2.0}) icon.addPixmap(pixmap(resourcePath, base, dpr));
    return icon;
}

QString ThemedSvg::themedFile(const QString &resourcePath) {
    const ThemeMode mode = ThemeManager::instance().effectiveMode();
    if (mode == ThemeMode::Light) return resourcePath;
    // versioned folder, so artwork changed by an update is never served from an old copy
    const QString folder = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QStringLiteral("/theme/") +
                           QCoreApplication::applicationVersion() +
                           (mode == ThemeMode::Amoled ? QStringLiteral("/amoled/") : QStringLiteral("/dark/"));
    const QString file = folder + QString(resourcePath).remove(0, 2);   // ":/uikit/x.svg" -> ".../uikit/x.svg"
    if (!QFileInfo::exists(file)) {
        QDir().mkpath(QFileInfo(file).absolutePath());
        QFile f(file);
        if (f.open(QIODevice::WriteOnly)) f.write(load(resourcePath));
    }
    return QDir::fromNativeSeparators(file);
}

void ThemedSvg::clearCache() { svgCache().clear(); }

}  // namespace theme
}  // namespace client
