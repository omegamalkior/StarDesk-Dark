// themed_asset.h - picks the dark / AMOLED version of a picture when one exists.
#pragma once

#include <QtCore/QString>

namespace client {
namespace theme {

// ":/uikit/common_modal_bk@4x.png" -> ":/dark/uikit/common_modal_bk@4x.png" in Dark mode,
// ":/amoled/..." in AMOLED mode, or the original path in Light mode / when there is no dark version.
// Use it wherever a PNG/ICO resource path is turned into a QPixmap / QIcon / border-image.
QString themedAsset(const QString &resourcePath);

}  // namespace theme
}  // namespace client
