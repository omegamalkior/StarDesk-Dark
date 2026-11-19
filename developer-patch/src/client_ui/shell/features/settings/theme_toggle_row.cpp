// theme_toggle_row.cpp - see theme_toggle_row.h
#include "theme_toggle_row.h"

#include "client_ui/uikit/theme/theme_manager.h"

#include <QtCore/QByteArray>
#include <QtCore/QSignalBlocker>
#include <QtGui/QColor>
#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtSvg/QSvgRenderer>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

using client::theme::ThemeManager;
using client::theme::ThemeMode;

namespace client {
namespace settings {

namespace {
// Colour of a theme token by name, e.g. tokenColor("kFillBrand"). Goes through the stylesheet
// resolver so this file doesn't depend on the enum; in the real codebase you can use
// ThemeManager::instance().color(ThemeColorToken::kFillBrand) instead.
QColor tokenColor(const char *name) {
    return QColor(ThemeManager::instance().resolveStyleSheet(QStringLiteral("@ThemeColor(%1)").arg(QLatin1String(name))));
}

// Simple crescent-moon icon (20x20), drawn in the current text colour.
QPixmap moonIcon(const QColor &c) {
    const QByteArray svg = QByteArrayLiteral(
        "<svg width='20' height='20' viewBox='0 0 20 20' xmlns='http://www.w3.org/2000/svg'>"
        "<path d='M15.5 12.6A6.6 6.6 0 0 1 7.4 4.5a6.6 6.6 0 1 0 8.1 8.1Z' fill='none' stroke='COLOR'"
        " stroke-width='1.4' stroke-linejoin='round'/></svg>").replace("COLOR", c.name().toLatin1());
    QSvgRenderer r(svg);
    QPixmap pm(40, 40);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    r.render(&p);
    p.end();
    pm.setDevicePixelRatio(2.0);
    return pm;
}
}  // namespace

// ---------------------------------------------------------------------------------------------
ThemeSwitch::ThemeSwitch(QWidget *parent) : QAbstractButton(parent) {
    setCheckable(true);
    setCursor(Qt::PointingHandCursor);
    setFixedSize(sizeHint());
}

void ThemeSwitch::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    const QRectF track = QRectF(rect()).adjusted(1, 3, -1, -3);
    const qreal r = track.height() / 2;
    if (isChecked()) {
        p.setPen(Qt::NoPen);
        p.setBrush(tokenColor("kFillBrand"));
    } else {
        p.setPen(QPen(tokenColor("kBorderBlackAlpha77"), 1.2));
        p.setBrush(Qt::transparent);
    }
    p.drawRoundedRect(track, r, r);
    const qreal d = track.height() - 8;
    const qreal x = isChecked() ? track.right() - 4 - d : track.left() + 4;
    p.setPen(Qt::NoPen);
    p.setBrush(isChecked() ? tokenColor("kTextInversePrimary") : tokenColor("kTextBlackAlpha153"));
    p.drawEllipse(QRectF(x, track.top() + 4, d, d));
}

// ---------------------------------------------------------------------------------------------
ThemeToggleRow::ThemeToggleRow(QWidget *parent) : QWidget(parent) {
    setObjectName(QStringLiteral("widgetThemeMode"));
    setAttribute(Qt::WA_StyledBackground, true);

    icon_ = new QLabel(this);
    title_ = new QLabel(tr("Dark mode"), this);
    title_->setObjectName(QStringLiteral("titleLabelThemeMode"));
    description_ = new QLabel(tr("Switch StarDesk between the light and dark theme. Takes effect immediately."), this);
    description_->setObjectName(QStringLiteral("infoLabelThemeMode"));
    description_->setWordWrap(true);
    state_ = new QLabel(this);
    state_->setObjectName(QStringLiteral("stateLabelThemeMode"));
    switch_ = new ThemeSwitch(this);

    auto *text = new QVBoxLayout;
    text->setSpacing(4);
    text->addWidget(title_);
    text->addWidget(description_);

    auto *row = new QHBoxLayout(this);
    row->setContentsMargins(24, 18, 24, 18);
    row->setSpacing(16);
    row->addWidget(icon_, 0, Qt::AlignTop);
    row->addLayout(text, 1);
    row->addWidget(state_, 0, Qt::AlignVCenter);
    row->addWidget(switch_, 0, Qt::AlignVCenter);

    connect(switch_, &QAbstractButton::toggled, this, &ThemeToggleRow::onToggled);
    connect(&ThemeManager::instance(), &ThemeManager::modeChanged, this, [this] { syncFromTheme(); });
    syncFromTheme();
}

void ThemeToggleRow::syncFromTheme() {
    const QSignalBlocker block(switch_);
    switch_->setChecked(ThemeManager::instance().isDark());
    state_->setText(switch_->isChecked() ? tr("On") : tr("Off"));
    restyle();
}

void ThemeToggleRow::onToggled(bool on) {
    state_->setText(on ? tr("On") : tr("Off"));
    // Live switch: every open window re-styles itself; the choice is saved for the next launch.
    ThemeManager::instance().setMode(on ? ThemeMode::Dark : ThemeMode::Light);
}

void ThemeToggleRow::restyle() {
    // Same card look as #widgetLanguage / #widgetAuth in Settings > General.
    ThemeManager::instance().setThemedStyleSheet(this, QStringLiteral(
        "#widgetThemeMode{border-radius:4px;background-color:@ThemeColor(kFillSolidWhite);"
        "border:1px solid @ThemeColor(kBorderBlackAlpha24);}"
        "#titleLabelThemeMode{font-size:14px;color:@ThemeColor(kTextPrimary);background:transparent;}"
        "#infoLabelThemeMode{font-size:12px;color:@ThemeColor(kTextBlackAlpha153);background:transparent;}"
        "#stateLabelThemeMode{font-size:14px;color:@ThemeColor(kTextPrimary);background:transparent;}"));
    icon_->setPixmap(moonIcon(tokenColor("kTextPrimary")));
    switch_->update();
}

// ---------------------------------------------------------------------------------------------
ThemeToggleRow *insertThemeToggleAbove(QWidget *startWithWindowsRow) {
    if (!startWithWindowsRow) return nullptr;
    // Climb to the card that holds the "Start with Windows" row (the first ancestor with its own
    // rounded-card stylesheet), then insert our card into the same vertical list just above it.
    QWidget *card = startWithWindowsRow;
    for (QWidget *w = startWithWindowsRow; w && w->parentWidget(); w = w->parentWidget()) {
        if (w->styleSheet().contains(QLatin1String("border-radius")) ||
            w->property("_uiKitThemeStyleSheetSource").toString().contains(QLatin1String("border-radius"))) {
            card = w;
            break;
        }
    }
    QWidget *list = card->parentWidget();
    auto *box = list ? qobject_cast<QBoxLayout *>(list->layout()) : nullptr;
    if (!box || box->indexOf(card) < 0) return nullptr;
    auto *row = new ThemeToggleRow(list);
    box->insertWidget(box->indexOf(card), row);
    return row;
}

}  // namespace settings
}  // namespace client
