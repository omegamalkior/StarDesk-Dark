// themed_asset.cpp - see themed_asset.h
#include "themed_asset.h"

#include "theme_manager.h"

#include <QtCore/QFile>

namespace client {
namespace theme {

QString themedAsset(const QString &resourcePath) {
    const ThemeMode mode = ThemeManager::instance().effectiveMode();
    if (mode == ThemeMode::Light || !resourcePath.startsWith(QLatin1String(":/"))) return resourcePath;
    QString alt = resourcePath;
    alt.insert(2, mode == ThemeMode::Amoled ? QLatin1String("amoled/") : QLatin1String("dark/"));
    return QFile::exists(alt) ? alt : resourcePath;
}

}  // namespace theme
}  // namespace client
