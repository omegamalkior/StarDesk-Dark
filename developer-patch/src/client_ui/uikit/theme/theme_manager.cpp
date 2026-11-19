// theme_manager.cpp - see theme_manager.h
#include "theme_manager.h"

#include "dark_color_maps.h"
#include "theme_palettes.h"
#include "themed_svg.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtGui/QPalette>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyle>
#include <QtWidgets/QWidget>
#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include <cmath>

#ifdef Q_OS_WIN
#include <windows.h>
#include <dwmapi.h>      // link dwmapi.lib
#endif

namespace client {
namespace theme {

namespace {
// ui_kit already stores these two properties on themed widgets; we reuse them so existing
// code that set a themed stylesheet is picked up automatically on a theme switch.
const char kSourceProp[] = "_uiKitThemeStyleSheetSource";
const char kResolvedProp[] = "_uiKitThemeStyleSheetResolved";
const char kSettingsKey[] = "appearance/theme";   // 0 = Light, 1 = Dark, 2 = AMOLED, 3 = System
const char kAccentKey[] = "appearance/useSystemAccent";


bool systemPrefersDark() {
#ifdef Q_OS_WIN
    QSettings s(QStringLiteral("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"),
                QSettings::NativeFormat);
    return s.value(QStringLiteral("AppsUseLightTheme"), 1).toInt() == 0;
#else
    return false;
#endif
}

// Watches for Windows' theme / accent broadcast and tells the manager to refresh.
class SystemThemeWatcher : public QAbstractNativeEventFilter {
public:
    bool nativeEventFilter(const QByteArray &type, void *message, long *) override {
#ifdef Q_OS_WIN
        if (type == "windows_generic_MSG") {
            const MSG *msg = static_cast<const MSG *>(message);
            if (msg->message == WM_SETTINGCHANGE && msg->lParam &&
                lstrcmpW(reinterpret_cast<LPCWSTR>(msg->lParam), L"ImmersiveColorSet") == 0)
                QMetaObject::invokeMethod(&ThemeManager::instance(), [] { ThemeManager::instance().refreshFromSystem(); },
                                          Qt::QueuedConnection);
        }
#else
        Q_UNUSED(type) Q_UNUSED(message)
#endif
        return false;
    }
};

QString qssColor(QRgb c) {
    // Qt stylesheets accept #AARRGGBB
    return QStringLiteral("#%1").arg(c, 8, 16, QLatin1Char('0')).toUpper();
}
}  // namespace

ThemeManager &ThemeManager::instance() {
    static ThemeManager manager;
    return manager;
}

ThemeManager::ThemeManager() {
    for (int i = 0; i < static_cast<int>(palette::kTokenCount); ++i)
        tokenIndex_.insert(QString::fromLatin1(palette::kTokenNames[i]), i);
}

void ThemeManager::initialize() {
    if (initialized_) return;
    initialized_ = true;
    QSettings settings;
    const int saved = settings.value(QLatin1String(kSettingsKey), 0).toInt();
    mode_ = (saved >= 0 && saved <= 3) ? static_cast<ThemeMode>(saved) : ThemeMode::Light;
    useAccent_ = settings.value(QLatin1String(kAccentKey), false).toBool();
    static SystemThemeWatcher watcher;
    QCoreApplication::instance()->installNativeEventFilter(&watcher);
    qApp->installEventFilter(this);   // dark native frame for every window as it is shown
    refreshFromSystem();   // reads Windows' mode + accent, builds the table, applies it
}

void ThemeManager::refreshFromSystem() {
    haveAccent_ = false;
#ifdef Q_OS_WIN
    DWORD size = sizeof accent_;
    haveAccent_ = RegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Accent",
                               L"AccentPalette", RRF_RT_REG_BINARY, nullptr, accent_, &size) == ERROR_SUCCESS && size >= 28;
#endif
    const ThemeMode before = effective_;
    applyEverything();
    if (effective_ != before) emit modeChanged(effective_);
}

void ThemeManager::rebuildTable() {
    effective_ = (mode_ == ThemeMode::System) ? (systemPrefersDark() ? ThemeMode::Dark : ThemeMode::Light) : mode_;
    const unsigned int *base = effective_ == ThemeMode::Dark ? palette::kDark
                             : effective_ == ThemeMode::Amoled ? palette::kAmoled : palette::kLight;
    for (std::size_t i = 0; i < palette::kTokenCount; ++i) table_[i] = base[i];
    if (!useAccent_ || !haveAccent_ || effective_ == ThemeMode::Light) return;
    const signed char *roles = effective_ == ThemeMode::Amoled ? palette::kAccentRoleAmoled : palette::kAccentRoleDark;
    for (std::size_t i = 0; i < palette::kTokenCount; ++i) {
        const int role = roles[i];
        if (role < 0) continue;
        const int shade = (role == 7) ? 6 : qMin(role, 6);
        const unsigned char *c = accent_[shade];
        table_[i] = (table_[i] & 0xFF000000u) | (unsigned(c[0]) << 16) | (unsigned(c[1]) << 8) | c[2];
    }
}

void ThemeManager::applyEverything() {
    rebuildTable();
    ThemedSvg::clearCache();
    applyApplicationPalette();
    reapplyAll();
}

void ThemeManager::setUseSystemAccent(bool on) {
    if (on == useAccent_) return;
    useAccent_ = on;
    QSettings().setValue(QLatin1String(kAccentKey), on);
    applyEverything();
    emit modeChanged(effective_);
}

const unsigned int *ThemeManager::table() const { return table_; }

QColor ThemeManager::color(int tokenIndex) const {
    if (tokenIndex < 0 || tokenIndex >= static_cast<int>(palette::kTokenCount)) return QColor();
    return QColor::fromRgba(table()[tokenIndex]);
}

QString ThemeManager::resolveStyleSheet(const QString &source) const {
    QString out = source;

    // 0. Some stylesheets colour *text* with a fill token (e.g. the verification code uses
    //    "color: @ThemeColor(kFillBlackAlpha230)"). Fill tokens stay dark in dark modes, so that text
    //    disappeared; use the matching text token instead. Best fixed at the source.
    if (effective_ != ThemeMode::Light) {
        static const QRegularExpression textFill(QStringLiteral("(color:\\s*)@ThemeColor\\(kFillBlackAlpha(\\d+)\\)"));
        QString fixed; int at = 0;
        auto t = textFill.globalMatch(out);
        while (t.hasNext()) {
            const auto m = t.next();
            const QString textToken = QStringLiteral("kTextBlackAlpha") + m.captured(2);
            fixed += out.midRef(at, m.capturedStart() - at);
            fixed += tokenIndex_.contains(textToken) ? m.captured(1) + QStringLiteral("@ThemeColor(%1)").arg(textToken) : m.captured(0);
            at = m.capturedEnd();
        }
        fixed += out.midRef(at);
        out = fixed;
    }

    // 1. light colours that are written directly into stylesheets (dark modes only)
    if (effective_ != ThemeMode::Light) {
        for (int i = 0; i < maps::kStyleSheetSwapsCount; ++i) {
            const auto &s = maps::kStyleSheetSwaps[i];
            out.replace(QLatin1String(s.from), QLatin1String(effective_ == ThemeMode::Amoled ? s.amoled : s.dark));
        }
    }

    // 2. artwork referenced as url(:/...svg) -> recoloured copy (dark modes only)
    if (effective_ != ThemeMode::Light) {
        static const QRegularExpression urlRe(QStringLiteral("url\\(\\s*\"?(:/[^\")]+\\.svg)\"?\\s*\\)"));
        QString withUrls;
        int pos = 0;
        auto u = urlRe.globalMatch(out);
        while (u.hasNext()) {
            const auto m = u.next();
            withUrls += out.midRef(pos, m.capturedStart() - pos);
            withUrls += QStringLiteral("url(\"%1\")").arg(ThemedSvg::themedFile(m.captured(1)));
            pos = m.capturedEnd();
        }
        withUrls += out.midRef(pos);
        out = withUrls;
    }

    // 3. @ThemeColor(kToken) -> #AARRGGBB for the active palette
    static const QRegularExpression re(QStringLiteral("@ThemeColor\\((k[A-Za-z0-9]+)\\)"));
    QString result;
    result.reserve(out.size());
    int last = 0;
    auto it = re.globalMatch(out);
    while (it.hasNext()) {
        const auto m = it.next();
        result += out.midRef(last, m.capturedStart() - last);
        const int idx = tokenIndex_.value(m.captured(1), -1);
        result += (idx >= 0) ? qssColor(table()[idx]) : m.captured(0);
        last = m.capturedEnd();
    }
    result += out.midRef(last);
    return result;
}

void ThemeManager::setThemedStyleSheet(QWidget *widget, const QString &source) {
    if (!widget) return;
    const QString resolved = resolveStyleSheet(source);
    widget->setProperty(kSourceProp, source);
    widget->setProperty(kResolvedProp, resolved);
    widget->setStyleSheet(resolved);
}

void ThemeManager::setMode(ThemeMode mode) {
    if (mode == mode_ && initialized_) return;
    mode_ = mode;
    initialized_ = true;
    QSettings settings;
    settings.setValue(QLatin1String(kSettingsKey), static_cast<int>(mode));
    applyEverything();
    emit modeChanged(effective_);
}

void ThemeManager::reapplyAll() {
    const auto widgets = QApplication::allWidgets();
    for (QWidget *w : widgets) {
        const QVariant source = w->property(kSourceProp);
        if (source.isValid()) {
            const QString resolved = resolveStyleSheet(source.toString());
            w->setProperty(kResolvedProp, resolved);
            w->setStyleSheet(resolved);
        }
        w->update();   // custom-painted widgets read color() again in paintEvent
    }
}

void ThemeManager::applyApplicationPalette() {
    // Widgets that StarDesk doesn't style itself (plain QLabel titles, QLineEdits without a stylesheet,
    // bare window backgrounds) are drawn from QPalette.
    // Light mode keeps Qt's normal palette.
    if (!qApp) return;
    QPalette p = qApp->style() ? qApp->style()->standardPalette() : QPalette();
    if (isDark()) {
        const auto tok = [this](const char *name) { return color(tokenIndex_.value(QLatin1String(name), -1)); };
        const QColor window = tok("kFillWindowPanel");      // #323232 - the title bar / sidebar grey
        const QColor surface = tok("kFillSolidWhite");       // card and dialog surface
        const QColor accent = tok("kFillBrand");
        p.setColor(QPalette::Window, window);                // bare window backgrounds
        p.setColor(QPalette::Base, Qt::transparent);         // labels / text boxes show the surface they sit on
        p.setColor(QPalette::AlternateBase, QColor(255, 255, 255, 10));
        p.setColor(QPalette::Button, surface);
        p.setColor(QPalette::ToolTipBase, surface);
        p.setColor(QPalette::Dark, window);
        p.setColor(QPalette::Mid, surface.lighter(120));
        p.setColor(QPalette::WindowText, Qt::white);
        p.setColor(QPalette::Text, Qt::white);
        p.setColor(QPalette::ButtonText, Qt::white);
        p.setColor(QPalette::ToolTipText, Qt::white);
        p.setColor(QPalette::PlaceholderText, QColor(255, 255, 255, 140));
        p.setColor(QPalette::Highlight, accent);
        p.setColor(QPalette::HighlightedText, Qt::black);   // Windows dark theme: dark text on the bright accent (TextOnAccentFillColorPrimary)
        p.setColor(QPalette::Link, accent);
    }
    qApp->setPalette(p);
    for (QWidget *w : QApplication::topLevelWidgets())
        if (w->testAttribute(Qt::WA_WState_Created)) applyNativeFrame(w);
}

void ThemeManager::applyNativeFrame(QWidget *window) const {
#ifdef Q_OS_WIN
    if (!window || !window->isWindow()) return;
    const BOOL dark = isDark() ? TRUE : FALSE;
    DwmSetWindowAttribute(reinterpret_cast<HWND>(window->winId()), 20 /*DWMWA_USE_IMMERSIVE_DARK_MODE*/, &dark, sizeof dark);
#else
    Q_UNUSED(window)
#endif
}

bool ThemeManager::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::Show && watched->isWidgetType()) {
        auto *w = static_cast<QWidget *>(watched);
        if (w->isWindow()) applyNativeFrame(w);
    }
    return QObject::eventFilter(watched, event);
}

}  // namespace theme
}  // namespace client
