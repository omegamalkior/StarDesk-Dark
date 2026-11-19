// theme_palettes.h - StarDesk colour palettes for every client::theme::ThemeColorToken.
// Generated from the StarDesk 1.5.4.9457 light palette. One entry per token, in enum order.
// Values are QRgb (0xAARRGGBB). Light = the shipping palette; Dark = MuMuPlayer-style dark;
// Amoled = pure-black variant. To add a token: append it to the enum AND to all three arrays.
#pragma once
#include <QtGui/QRgb>
#include <cstddef>

namespace client { namespace theme { namespace palette {

inline constexpr std::size_t kTokenCount = 255;

// Token names, handy for debugging and for the @ThemeColor(name) resolver.
inline constexpr const char *kTokenNames[kTokenCount] = {
    "kTextPrimary",
    "kTextBrand",
    "kTextInversePrimary",
    "kTextInverseSecondary",
    "kTextDeepPrimary",
    "kTextBlackAlpha230",
    "kTextBlackAlpha153",
    "kTextBlackAlpha115",
    "kTextBlackAlpha102",
    "kTextBlackAlpha89",
    "kTextWhiteAlpha179",
    "kTextBrandHover",
    "kTextBrandPressed",
    "kTextBlackAlpha255",
    "kTextBlackAlpha250",
    "kTextBlackAlpha180",
    "kTextBlackAlpha154",
    "kTextBrandAlpha204",
    "kTextGray141",
    "kTextGray98",
    "kTextGray208",
    "kTextRed201",
    "kTextRed240",
    "kTextRed250",
    "kTextGray34",
    "kTextBlueGray83",
    "kTextGray167",
    "kTextRed230",
    "kTextWhiteAlpha178",
    "kTextWhiteAlpha204",
    "kTextWhiteAlpha242",
    "kTextBlackAlpha77",
    "kTextPerformanceWarning",
    "kTextPerformanceDanger",
    "kFillSolidWhite",
    "kFillBrand",
    "kFillBrandLight",
    "kFillBrandLightHover",
    "kFillBrandLightPressed",
    "kFillTransparent",
    "kFillBlackAlpha5",
    "kFillBlackAlpha7",
    "kFillBlackAlpha8",
    "kFillBlackAlpha10",
    "kFillBlackAlpha15",
    "kFillBlackAlpha25",
    "kFillBlackAlpha26",
    "kFillBlackAlpha51",
    "kFillBlackAlpha64",
    "kFillBlackAlpha78",
    "kFillBlackAlpha89",
    "kFillBlackAlpha115",
    "kFillCoolGray",
    "kFillCoolGrayAlpha128",
    "kFillLightGrayAlpha153",
    "kFillWhiteAlpha0",
    "kFillWhiteAlpha26",
    "kFillWhiteAlpha245",
    "kFillBlackAlpha13",
    "kFillBlackAlpha23",
    "kFillBlackAlpha33",
    "kFillBrandHoverAlpha38",
    "kFillBrandHoverAlpha51",
    "kFillLightGrayAlpha204",
    "kFillAlmostWhiteAlpha217",
    "kFillAlmostWhiteAlpha250",
    "kFillMessageRefreshHover",
    "kFillMessageRefreshPressed",
    "kFillAlmostWhite",
    "kFillPaleBlue",
    "kFillPaleBlueAlpha128",
    "kFillGreenAlpha77",
    "kFillBlackAlpha52",
    "kFillLightRed",
    "kFillWebViewProgressTrack",
    "kFillWebViewProgressValue",
    "kFillBlackAlpha153",
    "kFillTooltipBackground",
    "kBorderInverseSubtle",
    "kBorderBlackAlpha15",
    "kBorderBlackAlpha41",
    "kBorderBlackAlpha26",
    "kBorderBlackAlpha51",
    "kBorderBlackAlpha76",
    "kBorderBlackAlpha114",
    "kBorderWhiteAlpha179",
    "kBorderLightGrayAlpha204",
    "kBorderLightGrayAlpha153",
    "kBorderBlackAlpha24",
    "kBorderBlackAlpha35",
    "kBorderBlackAlpha77",
    "kBorderGray204",
    "kBorderGray187",
    "kBorderBlueStrong",
    "kBorderBluePressed",
    "kBorderTransparent",
    "kBorderLightRed",
    "kFillBrandAlpha204",
    "kBorderWhiteAlpha20",
    "kFillBlackAlpha4",
    "kFillBlackAlpha6",
    "kBorderGray117Alpha115",
    "kFillWindowPanel",
    "kFillGray34Alpha153",
    "kFillGray34Alpha179",
    "kFillBlackAlpha77",
    "kBorderRed201",
    "kFillBlackAlpha255",
    "kBorderBlackAlpha1",
    "kBorderBlackAlpha5",
    "kBorderBlackAlpha8",
    "kBorderBlackAlpha16",
    "kBorderBlackAlpha20",
    "kBorderBlackAlpha23",
    "kBorderBlackAlpha25",
    "kBorderBlackAlpha31",
    "kBorderBlackAlpha40",
    "kBorderBlackAlpha52",
    "kBorderBlackAlpha56",
    "kBorderBlackAlpha89",
    "kBorderBlackAlpha102",
    "kBorderBlackAlpha156",
    "kBorderBlackAlpha204",
    "kBorderBlackAlpha230",
    "kBorderBlack",
    "kBorderWinBlue",
    "kBorderProgressBlueDark",
    "kBorderDeepPrimary",
    "kBorderBrandHover",
    "kBorderBrandPressed",
    "kBorderGreenAlpha77",
    "kBorderGray34Alpha153",
    "kBorderSelectedBlue",
    "kBorderLinkHover",
    "kBorderDarkGray47Alpha26",
    "kBorderBrandAlpha204",
    "kBorderBrandAlpha230",
    "kBorderBrand",
    "kBorderGray117Alpha51",
    "kBorderGray117Alpha102",
    "kBorderPaleBlueAlpha128",
    "kBorderLightBlueGrayAlpha26",
    "kBorderGray201",
    "kBorderGray208",
    "kBorderCoolGray",
    "kBorderGray216",
    "kBorderGray217",
    "kBorderGray218Alpha77",
    "kBorderHeaderBlue",
    "kBorderPaleBlue",
    "kBorderGray224",
    "kBorderGray233",
    "kBorderGray237",
    "kBorderGray239",
    "kBorderGray240",
    "kBorderGray243",
    "kBorderGray245",
    "kBorderWindowPanel",
    "kBorderGray249",
    "kBorderGray249Blue",
    "kBorderAlmostWhiteAlpha250",
    "kBorderWhiteAlpha153",
    "kBorderWhite",
    "kFillBlackAlpha56",
    "kFillBlackAlpha76",
    "kFillBlackAlpha102",
    "kFillBlackAlpha154",
    "kFillBlackAlpha156",
    "kFillBlackAlpha230",
    "kFillProgressBlue",
    "kFillDeepPrimary",
    "kFillBrandHover",
    "kFillDarkGray25",
    "kFillLoginBrandPressed",
    "kFillLinkHover",
    "kFillDarkGray47Alpha26",
    "kFillAnimationBlue",
    "kFillLoginBrandHover",
    "kFillLoginBrand",
    "kFillBrandGradientEnd",
    "kFillGray98",
    "kFillGray141",
    "kFillGray167Alt",
    "kFillGray182",
    "kFillPrivacySafe",
    "kFillGray217",
    "kFillGray218Alpha77",
    "kFillGray220",
    "kFillPaleBlueAlpha0",
    "kFillGray224",
    "kFillGray224Alt",
    "kFillGray225",
    "kFillSafeLockGradientStart",
    "kFillGray233",
    "kFillCloudStatusBackground",
    "kFillGray240",
    "kFillSafeLockGradientEnd",
    "kFillGray241Alt",
    "kFillPanelBackground",
    "kFillGray245",
    "kFillGray249",
    "kFillGray249Blue",
    "kFillAnimationGray",
    "kFillAlmostWhite252Alpha250",
    "kFillPrivacyWarning",
    "kFillRed",
    "kFillWhiteAlpha128",
    "kFillWhiteAlpha179",
    "kFillWhiteAlpha204",
    "kShadowBlackAlpha8",
    "kShadowBlackAlpha15",
    "kShadowBlackAlpha26",
    "kShadowBlackAlpha38",
    "kShadowBlackAlpha77",
    "kShadowBlackAlpha230",
    "kShadowGray241",
    "kShadowGray249",
    "kTextBlackAlpha0",
    "kTextBlackAlpha155",
    "kTextBlackAlpha156",
    "kTextGray176",
    "kTextGray182",
    "kTextBrandLight",
    "kTextGray233",
    "kTextBadgeRed",
    "kTextTooltipYellow",
    "kTextWhiteAlpha230",
    "kTextWhiteAlpha102",
    "kFillBrandAlpha166",
    "kFillGray170Alpha179",
    "kFillGray218Alpha230",
    "kFillGray218Alpha51",
    "kFillBlackAlpha20",
    "kBorderGray249Alpha51",
    "kBorderGray249Alpha77",
    "kTextAmber200",
    "kTextGray140",
    "kBorderGray166",
    "kFillTeachingTip",
    "kBorderTeachingTip",
    "kTextBlueGray34",
    "kTextBlueGray176",
    "kFillPaleRed",
    "kFillBlueGray234",
    "kFillBlueGray228",
    "kFillBlueGray248",
    "kFillBlueGray242",
    "kFillBlueGray249",
    "kFillGray252",
    "kBorderBlueGray216",
    "kBorderBlueGray239",
    "kBorderGray242",
    "kBorderGray229",
    "kFillBlueGray248Alt",
    "kBorderBlueGray223",
};

inline constexpr QRgb kLight[kTokenCount] = {
    0xE5000000u,  // kTextPrimary
    0xFF3A7BFCu,  // kTextBrand
    0xFFFFFFFFu,  // kTextInversePrimary
    0xE5FFFFFFu,  // kTextInverseSecondary
    0xFF040B18u,  // kTextDeepPrimary
    0xE6000000u,  // kTextBlackAlpha230
    0x99000000u,  // kTextBlackAlpha153
    0x73000000u,  // kTextBlackAlpha115
    0x66000000u,  // kTextBlackAlpha102
    0x59000000u,  // kTextBlackAlpha89
    0xB3FFFFFFu,  // kTextWhiteAlpha179
    0xFF0B66DDu,  // kTextBrandHover
    0xFF1740B0u,  // kTextBrandPressed
    0xFF000000u,  // kTextBlackAlpha255
    0xFA000000u,  // kTextBlackAlpha250
    0xB4000000u,  // kTextBlackAlpha180
    0x9A000000u,  // kTextBlackAlpha154
    0xCC3A7BFCu,  // kTextBrandAlpha204
    0xFF8A8D91u,  // kTextGray141
    0xFF606266u,  // kTextGray98
    0xFFD0D2D6u,  // kTextGray208
    0xFFC94032u,  // kTextRed201
    0xFFF04438u,  // kTextRed240
    0xFFFA4B5Au,  // kTextRed250
    0xFF222222u,  // kTextGray34
    0xFF223753u,  // kTextBlueGray83
    0xFFA0A2A7u,  // kTextGray167
    0xFFE63E2Au,  // kTextRed230
    0xB2FFFFFFu,  // kTextWhiteAlpha178
    0xCCFFFFFFu,  // kTextWhiteAlpha204
    0xF2FFFFFFu,  // kTextWhiteAlpha242
    0x4D000000u,  // kTextBlackAlpha77
    0xDAF58600u,  // kTextPerformanceWarning
    0xDAF20000u,  // kTextPerformanceDanger
    0xFFFFFFFFu,  // kFillSolidWhite
    0xFF3A7BFCu,  // kFillBrand
    0xFFDEEAFFu,  // kFillBrandLight
    0xFFB5D3FFu,  // kFillBrandLightHover
    0xFF8CB8FFu,  // kFillBrandLightPressed
    0x00000000u,  // kFillTransparent
    0x05000000u,  // kFillBlackAlpha5
    0x07000000u,  // kFillBlackAlpha7
    0x08000000u,  // kFillBlackAlpha8
    0x0A000000u,  // kFillBlackAlpha10
    0x0F000000u,  // kFillBlackAlpha15
    0x19000000u,  // kFillBlackAlpha25
    0x1A000000u,  // kFillBlackAlpha26
    0x33000000u,  // kFillBlackAlpha51
    0x40000000u,  // kFillBlackAlpha64
    0x4E000000u,  // kFillBlackAlpha78
    0x59000000u,  // kFillBlackAlpha89
    0x73000000u,  // kFillBlackAlpha115
    0xFFD7DCE0u,  // kFillCoolGray
    0x80D7DCE0u,  // kFillCoolGrayAlpha128
    0x99F9F9F9u,  // kFillLightGrayAlpha153
    0x00FFFFFFu,  // kFillWhiteAlpha0
    0x1AFFFFFFu,  // kFillWhiteAlpha26
    0xF5FFFFFFu,  // kFillWhiteAlpha245
    0x0D000000u,  // kFillBlackAlpha13
    0x17000000u,  // kFillBlackAlpha23
    0x21000000u,  // kFillBlackAlpha33
    0x260B66DDu,  // kFillBrandHoverAlpha38
    0x330B66DDu,  // kFillBrandHoverAlpha51
    0xCCF9F9F9u,  // kFillLightGrayAlpha204
    0xD9FCFCFCu,  // kFillAlmostWhiteAlpha217
    0xFAFAFAFAu,  // kFillAlmostWhiteAlpha250
    0xFFE4EDF9u,  // kFillMessageRefreshHover
    0xFFCFE2F5u,  // kFillMessageRefreshPressed
    0xFFFDFDFDu,  // kFillAlmostWhite
    0xFFDEF0FFu,  // kFillPaleBlue
    0x80BDE1FFu,  // kFillPaleBlueAlpha128
    0x4D1ADB87u,  // kFillGreenAlpha77
    0x34000000u,  // kFillBlackAlpha52
    0xFFFFCAC5u,  // kFillLightRed
    0xFFEEEEEEu,  // kFillWebViewProgressTrack
    0xFF007BFFu,  // kFillWebViewProgressValue
    0x99000000u,  // kFillBlackAlpha153
    0xFFF1F3F6u,  // kFillTooltipBackground
    0x33FFFFFFu,  // kBorderInverseSubtle
    0x0F000000u,  // kBorderBlackAlpha15
    0x29000000u,  // kBorderBlackAlpha41
    0x1A000000u,  // kBorderBlackAlpha26
    0x33000000u,  // kBorderBlackAlpha51
    0x4C000000u,  // kBorderBlackAlpha76
    0x72000000u,  // kBorderBlackAlpha114
    0xB3FFFFFFu,  // kBorderWhiteAlpha179
    0xCCF9F9F9u,  // kBorderLightGrayAlpha204
    0x99F9F9F9u,  // kBorderLightGrayAlpha153
    0x18000000u,  // kBorderBlackAlpha24
    0x23000000u,  // kBorderBlackAlpha35
    0x4D000000u,  // kBorderBlackAlpha77
    0xFFCCCCCCu,  // kBorderGray204
    0xFFBBBBBBu,  // kBorderGray187
    0xFF2D69B8u,  // kBorderBlueStrong
    0xFF225394u,  // kBorderBluePressed
    0x00000000u,  // kBorderTransparent
    0xFFE6BAB6u,  // kBorderLightRed
    0xCC3A7BFCu,  // kFillBrandAlpha204
    0x14FFFFFFu,  // kBorderWhiteAlpha20
    0x04000000u,  // kFillBlackAlpha4
    0x06000000u,  // kFillBlackAlpha6
    0x73757575u,  // kBorderGray117Alpha115
    0xFFF8FBFDu,  // kFillWindowPanel
    0x99222222u,  // kFillGray34Alpha153
    0xB3222222u,  // kFillGray34Alpha179
    0x4D000000u,  // kFillBlackAlpha77
    0xFFC94032u,  // kBorderRed201
    0xFF000000u,  // kFillBlackAlpha255
    0x01000000u,  // kBorderBlackAlpha1
    0x05000000u,  // kBorderBlackAlpha5
    0x08000000u,  // kBorderBlackAlpha8
    0x10000000u,  // kBorderBlackAlpha16
    0x14000000u,  // kBorderBlackAlpha20
    0x17000000u,  // kBorderBlackAlpha23
    0x19000000u,  // kBorderBlackAlpha25
    0x1F000000u,  // kBorderBlackAlpha31
    0x28000000u,  // kBorderBlackAlpha40
    0x34000000u,  // kBorderBlackAlpha52
    0x38000000u,  // kBorderBlackAlpha56
    0x59000000u,  // kBorderBlackAlpha89
    0x66000000u,  // kBorderBlackAlpha102
    0x9C000000u,  // kBorderBlackAlpha156
    0xCC000000u,  // kBorderBlackAlpha204
    0xE6000000u,  // kBorderBlackAlpha230
    0xFF000000u,  // kBorderBlack
    0xFF0078D4u,  // kBorderWinBlue
    0xFF015EB5u,  // kBorderProgressBlueDark
    0xFF040B18u,  // kBorderDeepPrimary
    0xFF0B66DDu,  // kBorderBrandHover
    0xFF1740B0u,  // kBorderBrandPressed
    0x4D1ADB87u,  // kBorderGreenAlpha77
    0x99222222u,  // kBorderGray34Alpha153
    0xFF246FFEu,  // kBorderSelectedBlue
    0xFF275BD6u,  // kBorderLinkHover
    0x1A2F2F30u,  // kBorderDarkGray47Alpha26
    0xCC3A7BFCu,  // kBorderBrandAlpha204
    0xE63A7BFCu,  // kBorderBrandAlpha230
    0xFF3A7BFCu,  // kBorderBrand
    0x33757575u,  // kBorderGray117Alpha51
    0x66757575u,  // kBorderGray117Alpha102
    0x80BDE1FFu,  // kBorderPaleBlueAlpha128
    0x1AC1CEE8u,  // kBorderLightBlueGrayAlpha26
    0xFFC9C9C9u,  // kBorderGray201
    0xFFD0D2D6u,  // kBorderGray208
    0xFFD7DCE0u,  // kBorderCoolGray
    0xFFD8D9E2u,  // kBorderGray216
    0xFFD9D9D9u,  // kBorderGray217
    0x4DDADADAu,  // kBorderGray218Alpha77
    0xFFDBE9F7u,  // kBorderHeaderBlue
    0xFFDEF0FFu,  // kBorderPaleBlue
    0xFFE0E4EAu,  // kBorderGray224
    0xFFE9ECF1u,  // kBorderGray233
    0xFFEDEEF0u,  // kBorderGray237
    0xFFEFEFEFu,  // kBorderGray239
    0xFFF0F5F8u,  // kBorderGray240
    0xFFF3F3F5u,  // kBorderGray243
    0xFFF5F5F5u,  // kBorderGray245
    0xFFF8FBFDu,  // kBorderWindowPanel
    0xFFF9F9F9u,  // kBorderGray249
    0xFFF9FBFDu,  // kBorderGray249Blue
    0xFAFAFAFAu,  // kBorderAlmostWhiteAlpha250
    0x99FFFFFFu,  // kBorderWhiteAlpha153
    0xFFFFFFFFu,  // kBorderWhite
    0x38000000u,  // kFillBlackAlpha56
    0x4C000000u,  // kFillBlackAlpha76
    0x66000000u,  // kFillBlackAlpha102
    0x9A000000u,  // kFillBlackAlpha154
    0x9C000000u,  // kFillBlackAlpha156
    0xE6000000u,  // kFillBlackAlpha230
    0xFF005FB8u,  // kFillProgressBlue
    0xFF040B18u,  // kFillDeepPrimary
    0xFF0B66DDu,  // kFillBrandHover
    0xFF191919u,  // kFillDarkGray25
    0xFF223FAAu,  // kFillLoginBrandPressed
    0xFF275BD6u,  // kFillLinkHover
    0x1A2F2F30u,  // kFillDarkGray47Alpha26
    0xFF3183FFu,  // kFillAnimationBlue
    0xFF355ACFu,  // kFillLoginBrandHover
    0xFF4B7AF4u,  // kFillLoginBrand
    0xFF5D9EFFu,  // kFillBrandGradientEnd
    0xFF606266u,  // kFillGray98
    0xFF8A8D91u,  // kFillGray141
    0xFFA7A7A9u,  // kFillGray167Alt
    0xFFB6BCC2u,  // kFillGray182
    0xFFCCF0E3u,  // kFillPrivacySafe
    0xFFD9D9D9u,  // kFillGray217
    0x4DDADADAu,  // kFillGray218Alpha77
    0xFFDCDCDCu,  // kFillGray220
    0x00DEF0FFu,  // kFillPaleBlueAlpha0
    0xFFE0E4EAu,  // kFillGray224
    0xFFE0E2E7u,  // kFillGray224Alt
    0xFFE1E4E9u,  // kFillGray225
    0xFFE3F2FFu,  // kFillSafeLockGradientStart
    0xFFE9ECF1u,  // kFillGray233
    0xFFF0F4F7u,  // kFillCloudStatusBackground
    0xFFF0F5F8u,  // kFillGray240
    0xFFF0F8FFu,  // kFillSafeLockGradientEnd
    0xFFF1F2F4u,  // kFillGray241Alt
    0xFFF1F4F9u,  // kFillPanelBackground
    0xFFF5F5F5u,  // kFillGray245
    0xFFF9F9F9u,  // kFillGray249
    0xFFF9FBFDu,  // kFillGray249Blue
    0xFFFBFCFFu,  // kFillAnimationGray
    0xFAFCFCFCu,  // kFillAlmostWhite252Alpha250
    0xFFFEF0E0u,  // kFillPrivacyWarning
    0xFFFF0000u,  // kFillRed
    0x80FFFFFFu,  // kFillWhiteAlpha128
    0xB3FFFFFFu,  // kFillWhiteAlpha179
    0xCCFFFFFFu,  // kFillWhiteAlpha204
    0x08000000u,  // kShadowBlackAlpha8
    0x0F000000u,  // kShadowBlackAlpha15
    0x1A000000u,  // kShadowBlackAlpha26
    0x26000000u,  // kShadowBlackAlpha38
    0x4D000000u,  // kShadowBlackAlpha77
    0xE6000000u,  // kShadowBlackAlpha230
    0xFFF1F3F6u,  // kShadowGray241
    0xFFF9F9F9u,  // kShadowGray249
    0x00000000u,  // kTextBlackAlpha0
    0x9B000000u,  // kTextBlackAlpha155
    0x9C000000u,  // kTextBlackAlpha156
    0xFFB0B3B8u,  // kTextGray176
    0xFFB6BCC2u,  // kTextGray182
    0xFFDEEAFFu,  // kTextBrandLight
    0xFFE9ECF1u,  // kTextGray233
    0xFFFF4444u,  // kTextBadgeRed
    0xFFFFFFE1u,  // kTextTooltipYellow
    0xE6FFFFFFu,  // kTextWhiteAlpha230
    0x66FFFFFFu,  // kTextWhiteAlpha102
    0xA63A7BFCu,  // kFillBrandAlpha166
    0xB3AAAAAAu,  // kFillGray170Alpha179
    0xE6DADADAu,  // kFillGray218Alpha230
    0x33DADADAu,  // kFillGray218Alpha51
    0x14000000u,  // kFillBlackAlpha20
    0x33F9F9F9u,  // kBorderGray249Alpha51
    0x4DF9F9F9u,  // kBorderGray249Alpha77
    0xFFC89300u,  // kTextAmber200
    0xFF8C8C8Cu,  // kTextGray140
    0xFFA6A6A6u,  // kBorderGray166
    0xFFF8FBFDu,  // kFillTeachingTip
    0x73757575u,  // kBorderTeachingTip
    0xFF22282Cu,  // kTextBlueGray34
    0xFFB0B5B8u,  // kTextBlueGray176
    0xFFFDECEAu,  // kFillPaleRed
    0xFFEAF1F5u,  // kFillBlueGray234
    0xFFE4EDF2u,  // kFillBlueGray228
    0xFFF8FAFBu,  // kFillBlueGray248
    0xFFF2F5F7u,  // kFillBlueGray242
    0xFFF9FAFBu,  // kFillBlueGray249
    0xFFFCFCFCu,  // kFillGray252
    0xFFD8DCDFu,  // kBorderBlueGray216
    0xFFEFF0F1u,  // kBorderBlueGray239
    0xFFF2F2F2u,  // kBorderGray242
    0xFFE5E5E5u,  // kBorderGray229
    0xFFF8FBFCu,  // kFillBlueGray248Alt
    0xFFDFE1E3u,  // kBorderBlueGray223
};

inline constexpr QRgb kDark[kTokenCount] = {
    0xE5FFFFFFu,  // kTextPrimary
    0xFF38D0FFu,  // kTextBrand
    0xFFFFFFFFu,  // kTextInversePrimary
    0xE5FFFFFFu,  // kTextInverseSecondary
    0xFFECF0F6u,  // kTextDeepPrimary
    0xE6FFFFFFu,  // kTextBlackAlpha230
    0xC5FFFFFFu,  // kTextBlackAlpha153
    0xA3FFFFFFu,  // kTextBlackAlpha115
    0xA3FFFFFFu,  // kTextBlackAlpha102
    0x71FFFFFFu,  // kTextBlackAlpha89
    0xB3FFFFFFu,  // kTextWhiteAlpha179
    0xFF54D7FFu,  // kTextBrandHover
    0xFF35D0FFu,  // kTextBrandPressed
    0xFFFFFFFFu,  // kTextBlackAlpha255
    0xFAFFFFFFu,  // kTextBlackAlpha250
    0xC5FFFFFFu,  // kTextBlackAlpha180
    0xC5FFFFFFu,  // kTextBlackAlpha154
    0xCC9EE8FFu,  // kTextBrandAlpha204
    0xFFC0C1C3u,  // kTextGray141
    0xFFC0C1C2u,  // kTextGray98
    0xFF5E6066u,  // kTextGray208
    0xFFF7AEA6u,  // kTextRed201
    0xFFF9AFAAu,  // kTextRed240
    0xFFFDACB3u,  // kTextRed250
    0xFFE4E4E4u,  // kTextGray34
    0xFFC4D4E8u,  // kTextBlueGray83
    0xFFC0C1C3u,  // kTextGray167
    0xFFFAADA5u,  // kTextRed230
    0xB2FFFFFFu,  // kTextWhiteAlpha178
    0xCCFFFFFFu,  // kTextWhiteAlpha204
    0xF2FFFFFFu,  // kTextWhiteAlpha242
    0x71FFFFFFu,  // kTextBlackAlpha77
    0xDAFFCB8Cu,  // kTextPerformanceWarning
    0xDAFFC7C7u,  // kTextPerformanceDanger
    0xFF464646u,  // kFillSolidWhite
    0xFF12C8FFu,  // kFillBrand
    0xFF283A40u,  // kFillBrandLight
    0xFF31484Fu,  // kFillBrandLightHover
    0xFF00779Bu,  // kFillBrandLightPressed
    0x00000000u,  // kFillTransparent
    0x05FFFFFFu,  // kFillBlackAlpha5
    0x07FFFFFFu,  // kFillBlackAlpha7
    0x08FFFFFFu,  // kFillBlackAlpha8
    0x0AFFFFFFu,  // kFillBlackAlpha10
    0x0FFFFFFFu,  // kFillBlackAlpha15
    0x19FFFFFFu,  // kFillBlackAlpha25
    0x1AFFFFFFu,  // kFillBlackAlpha26
    0x33FFFFFFu,  // kFillBlackAlpha51
    0x40FFFFFFu,  // kFillBlackAlpha64
    0xC8E8E8E8u,  // kFillBlackAlpha78
    0xFF565656u,  // kFillBlackAlpha89
    0x73FFFFFFu,  // kFillBlackAlpha115
    0xFF323232u,  // kFillCoolGray
    0x805E5E5Eu,  // kFillCoolGrayAlpha128
    0x99363636u,  // kFillLightGrayAlpha153
    0x00FFFFFFu,  // kFillWhiteAlpha0
    0x1AFFFFFFu,  // kFillWhiteAlpha26
    0xF5424242u,  // kFillWhiteAlpha245
    0x0DFFFFFFu,  // kFillBlackAlpha13
    0x17FFFFFFu,  // kFillBlackAlpha23
    0x21FFFFFFu,  // kFillBlackAlpha33
    0x26009DCCu,  // kFillBrandHoverAlpha38
    0x33009DCCu,  // kFillBrandHoverAlpha51
    0xCC363636u,  // kFillLightGrayAlpha204
    0xD9343434u,  // kFillAlmostWhiteAlpha217
    0xFA363636u,  // kFillAlmostWhiteAlpha250
    0xFF3C3C3Cu,  // kFillMessageRefreshHover
    0xFF535353u,  // kFillMessageRefreshPressed
    0xFF404040u,  // kFillAlmostWhite
    0xFF2F2F2Fu,  // kFillPaleBlue
    0x80324D55u,  // kFillPaleBlueAlpha128
    0x4D1ADB87u,  // kFillGreenAlpha77
    0x34FFFFFFu,  // kFillBlackAlpha52
    0xFF5C2826u,  // kFillLightRed
    0xFF4D4D4Du,  // kFillWebViewProgressTrack
    0xFF00ACE0u,  // kFillWebViewProgressValue
    0x99000000u,  // kFillBlackAlpha153
    0xFF474747u,  // kFillTooltipBackground
    0x33FFFFFFu,  // kBorderInverseSubtle
    0x0FFFFFFFu,  // kBorderBlackAlpha15
    0x29FFFFFFu,  // kBorderBlackAlpha41
    0x1AFFFFFFu,  // kBorderBlackAlpha26
    0x33FFFFFFu,  // kBorderBlackAlpha51
    0x4CFFFFFFu,  // kBorderBlackAlpha76
    0x72FFFFFFu,  // kBorderBlackAlpha114
    0xB3454545u,  // kBorderWhiteAlpha179
    0xCC494949u,  // kBorderLightGrayAlpha204
    0x99494949u,  // kBorderLightGrayAlpha153
    0x18FFFFFFu,  // kBorderBlackAlpha24
    0x23FFFFFFu,  // kBorderBlackAlpha35
    0x4DFFFFFFu,  // kBorderBlackAlpha77
    0xFF676767u,  // kBorderGray204
    0xFF6E6E6Eu,  // kBorderGray187
    0xFF009ACAu,  // kBorderBlueStrong
    0xFF007BA0u,  // kBorderBluePressed
    0x00000000u,  // kBorderTransparent
    0xFF683D39u,  // kBorderLightRed
    0xCC12C8FFu,  // kFillBrandAlpha204
    0x14FFFFFFu,  // kBorderWhiteAlpha20
    0x04FFFFFFu,  // kFillBlackAlpha4
    0x06FFFFFFu,  // kFillBlackAlpha6
    0x73757575u,  // kBorderGray117Alpha115
    0xFF323232u,  // kFillWindowPanel
    0x99393939u,  // kFillGray34Alpha153
    0xB3393939u,  // kFillGray34Alpha179
    0x80000000u,  // kFillBlackAlpha77
    0xFFC94032u,  // kBorderRed201
    0xFF000000u,  // kFillBlackAlpha255
    0x01FFFFFFu,  // kBorderBlackAlpha1
    0x05FFFFFFu,  // kBorderBlackAlpha5
    0x08FFFFFFu,  // kBorderBlackAlpha8
    0x10FFFFFFu,  // kBorderBlackAlpha16
    0x14FFFFFFu,  // kBorderBlackAlpha20
    0x17FFFFFFu,  // kBorderBlackAlpha23
    0x19FFFFFFu,  // kBorderBlackAlpha25
    0x1FFFFFFFu,  // kBorderBlackAlpha31
    0x28FFFFFFu,  // kBorderBlackAlpha40
    0x34FFFFFFu,  // kBorderBlackAlpha52
    0x38FFFFFFu,  // kBorderBlackAlpha56
    0x59FFFFFFu,  // kBorderBlackAlpha89
    0x66FFFFFFu,  // kBorderBlackAlpha102
    0x9CFFFFFFu,  // kBorderBlackAlpha156
    0x80000000u,  // kBorderBlackAlpha204
    0xE6FFFFFFu,  // kBorderBlackAlpha230
    0xFFAAAAAAu,  // kBorderBlack
    0xFF008FBBu,  // kBorderWinBlue
    0xFF007BA0u,  // kBorderProgressBlueDark
    0xFFC8CED8u,  // kBorderDeepPrimary
    0xFF009DCCu,  // kBorderBrandHover
    0xFF0086AFu,  // kBorderBrandPressed
    0x4D1ADB87u,  // kBorderGreenAlpha77
    0x99323232u,  // kBorderGray34Alpha153
    0xFF00C4FFu,  // kBorderSelectedBlue
    0xFF00ABDFu,  // kBorderLinkHover
    0x1A3F3F3Fu,  // kBorderDarkGray47Alpha26
    0xCC12C8FFu,  // kBorderBrandAlpha204
    0xE612C8FFu,  // kBorderBrandAlpha230
    0xFF12C8FFu,  // kBorderBrand
    0x33757575u,  // kBorderGray117Alpha51
    0x66757575u,  // kBorderGray117Alpha102
    0x8033464Cu,  // kBorderPaleBlueAlpha128
    0x1A5B5B5Bu,  // kBorderLightBlueGrayAlpha26
    0xFF686868u,  // kBorderGray201
    0xFF626262u,  // kBorderGray208
    0xFF5C5C5Cu,  // kBorderCoolGray
    0xFF5A5A5Au,  // kBorderGray216
    0xFF5E5E5Eu,  // kBorderGray217
    0x4D5C5C5Cu,  // kBorderGray218Alpha77
    0xFF304349u,  // kBorderHeaderBlue
    0xFF304349u,  // kBorderPaleBlue
    0xFF555555u,  // kBorderGray224
    0xFF4F4F4Fu,  // kBorderGray233
    0xFF4F4F4Fu,  // kBorderGray237
    0xFF505050u,  // kBorderGray239
    0xFF242424u,  // kBorderGray240
    0xFF4B4B4Bu,  // kBorderGray243
    0xFF4C4C4Cu,  // kBorderGray245
    0xFF424242u,  // kBorderWindowPanel
    0xFF494949u,  // kBorderGray249
    0xFF464646u,  // kBorderGray249Blue
    0xFA484848u,  // kBorderAlmostWhiteAlpha250
    0x99454545u,  // kBorderWhiteAlpha153
    0xFF4C4C4Cu,  // kBorderWhite
    0x38FFFFFFu,  // kFillBlackAlpha56
    0xC8E8E8E8u,  // kFillBlackAlpha76
    0x66000000u,  // kFillBlackAlpha102
    0x9A000000u,  // kFillBlackAlpha154
    0x9C000000u,  // kFillBlackAlpha156
    0xE6000000u,  // kFillBlackAlpha230
    0xFF007CA2u,  // kFillProgressBlue
    0xFF5B5B5Bu,  // kFillDeepPrimary
    0xFF009DCCu,  // kFillBrandHover
    0xFF4E4E4Eu,  // kFillDarkGray25
    0xFF008AB4u,  // kFillLoginBrandPressed
    0xFF00ABDFu,  // kFillLinkHover
    0x1AC8C8C8u,  // kFillDarkGray47Alpha26
    0xFF0DC6FFu,  // kFillAnimationBlue
    0xFF00AFE5u,  // kFillLoginBrandHover
    0xFF1ACAFFu,  // kFillLoginBrand
    0xFF33CFFFu,  // kFillBrandGradientEnd
    0xFF9B9D9Fu,  // kFillGray98
    0xFF7E8083u,  // kFillGray141
    0xFF828282u,  // kFillGray167Alt
    0xFF828282u,  // kFillGray182
    0xFF393939u,  // kFillPrivacySafe
    0xFF404040u,  // kFillGray217
    0x4D3E3E3Eu,  // kFillGray218Alpha77
    0xFF5D5D5Du,  // kFillGray220
    0x002F2F2Fu,  // kFillPaleBlueAlpha0
    0xFF525252u,  // kFillGray224
    0xFF535353u,  // kFillGray224Alt
    0xFF515151u,  // kFillGray225
    0xFF333333u,  // kFillSafeLockGradientStart
    0xFF464646u,  // kFillGray233
    0xFF3E3E3Eu,  // kFillCloudStatusBackground
    0xFF3E3E3Eu,  // kFillGray240
    0xFF282828u,  // kFillSafeLockGradientEnd
    0xFF404040u,  // kFillGray241Alt
    0xFF373737u,  // kFillPanelBackground
    0xFF3E3E3Eu,  // kFillGray245
    0xFF373737u,  // kFillGray249
    0xFF373737u,  // kFillGray249Blue
    0xFF545454u,  // kFillAnimationGray
    0xFA343434u,  // kFillAlmostWhite252Alpha250
    0xFF483418u,  // kFillPrivacyWarning
    0xFFFF0000u,  // kFillRed
    0x80424242u,  // kFillWhiteAlpha128
    0xB3424242u,  // kFillWhiteAlpha179
    0xCC424242u,  // kFillWhiteAlpha204
    0x08000000u,  // kShadowBlackAlpha8
    0x0F000000u,  // kShadowBlackAlpha15
    0x1A000000u,  // kShadowBlackAlpha26
    0x26000000u,  // kShadowBlackAlpha38
    0x4D000000u,  // kShadowBlackAlpha77
    0xE6000000u,  // kShadowBlackAlpha230
    0xFF121214u,  // kShadowGray241
    0xFF121214u,  // kShadowGray249
    0x00000000u,  // kTextBlackAlpha0
    0xC5FFFFFFu,  // kTextBlackAlpha155
    0xC5FFFFFFu,  // kTextBlackAlpha156
    0xFFC0C1C4u,  // kTextGray176
    0xFFBEC1C4u,  // kTextGray182
    0xFFDEEAFFu,  // kTextBrandLight
    0xFF484D54u,  // kTextGray233
    0xFFFFAAAAu,  // kTextBadgeRed
    0xFFFFFFE1u,  // kTextTooltipYellow
    0xE6FFFFFFu,  // kTextWhiteAlpha230
    0x66FFFFFFu,  // kTextWhiteAlpha102
    0xA612C8FFu,  // kFillBrandAlpha166
    0xB3828282u,  // kFillGray170Alpha179
    0xE65F5F5Fu,  // kFillGray218Alpha230
    0x333E3E3Eu,  // kFillGray218Alpha51
    0x14FFFFFFu,  // kFillBlackAlpha20
    0x33494949u,  // kBorderGray249Alpha51
    0x4D494949u,  // kBorderGray249Alpha77
    0xFFF8B700u,  // kTextAmber200
    0xFFC0C0C0u,  // kTextGray140
    0xFF6E6E6Eu,  // kBorderGray166
    0xFF5B5B5Bu,  // kFillTeachingTip
    0x73757575u,  // kBorderTeachingTip
    0xFFE2E6EAu,  // kTextBlueGray34
    0xFFBFC1C3u,  // kTextBlueGray176
    0xFF292929u,  // kFillPaleRed
    0xFF444444u,  // kFillBlueGray234
    0xFF4A4A4Au,  // kFillBlueGray228
    0xFF373737u,  // kFillBlueGray248
    0xFF3C3C3Cu,  // kFillBlueGray242
    0xFF363636u,  // kFillBlueGray249
    0xFF343434u,  // kFillGray252
    0xFF5C5C5Cu,  // kBorderBlueGray216
    0xFF4F4F4Fu,  // kBorderBlueGray239
    0xFF4D4D4Du,  // kBorderGray242
    0xFF565656u,  // kBorderGray229
    0xFF373737u,  // kFillBlueGray248Alt
    0xFF595959u,  // kBorderBlueGray223
};

inline constexpr QRgb kAmoled[kTokenCount] = {
    0xE5FFFFFFu,  // kTextPrimary
    0xFF629CFFu,  // kTextBrand
    0xFFFFFFFFu,  // kTextInversePrimary
    0xE5FFFFFFu,  // kTextInverseSecondary
    0xFFECF0F6u,  // kTextDeepPrimary
    0xE6FFFFFFu,  // kTextBlackAlpha230
    0xC5FFFFFFu,  // kTextBlackAlpha153
    0x77FFFFFFu,  // kTextBlackAlpha115
    0x77FFFFFFu,  // kTextBlackAlpha102
    0x59FFFFFFu,  // kTextBlackAlpha89
    0xB3FFFFFFu,  // kTextWhiteAlpha179
    0xFF82B2FFu,  // kTextBrandHover
    0xFF5087F0u,  // kTextBrandPressed
    0xFFFFFFFFu,  // kTextBlackAlpha255
    0xFAFFFFFFu,  // kTextBlackAlpha250
    0xC5FFFFFFu,  // kTextBlackAlpha180
    0xC5FFFFFFu,  // kTextBlackAlpha154
    0xCC6FA4FFu,  // kTextBrandAlpha204
    0xFF949598u,  // kTextGray141
    0xFFB4B5B7u,  // kTextGray98
    0xFF5E6066u,  // kTextGray208
    0xFFF06E60u,  // kTextRed201
    0xFFF14F44u,  // kTextRed240
    0xFFFA4B5Au,  // kTextRed250
    0xFFE4E4E4u,  // kTextGray34
    0xFFC4D4E8u,  // kTextBlueGray83
    0xFF88898Du,  // kTextGray167
    0xFFF5695Au,  // kTextRed230
    0xB2FFFFFFu,  // kTextWhiteAlpha178
    0xCCFFFFFFu,  // kTextWhiteAlpha204
    0xF2FFFFFFu,  // kTextWhiteAlpha242
    0x56FFFFFFu,  // kTextBlackAlpha77
    0xDAF58600u,  // kTextPerformanceWarning
    0xDAFF6B6Bu,  // kTextPerformanceDanger
    0xFF0C0C0Cu,  // kFillSolidWhite
    0xFF3A7BFCu,  // kFillBrand
    0xFF080C13u,  // kFillBrandLight
    0xFF0C121Eu,  // kFillBrandLightHover
    0xFF304C80u,  // kFillBrandLightPressed
    0x00000000u,  // kFillTransparent
    0x05FFFFFFu,  // kFillBlackAlpha5
    0x07FFFFFFu,  // kFillBlackAlpha7
    0x08FFFFFFu,  // kFillBlackAlpha8
    0x0AFFFFFFu,  // kFillBlackAlpha10
    0x0FFFFFFFu,  // kFillBlackAlpha15
    0x19FFFFFFu,  // kFillBlackAlpha25
    0x1AFFFFFFu,  // kFillBlackAlpha26
    0x33FFFFFFu,  // kFillBlackAlpha51
    0x40FFFFFFu,  // kFillBlackAlpha64
    0xC8E8E8E8u,  // kFillBlackAlpha78
    0xFF282828u,  // kFillBlackAlpha89
    0x73FFFFFFu,  // kFillBlackAlpha115
    0xFF080808u,  // kFillCoolGray
    0x80141414u,  // kFillCoolGrayAlpha128
    0x990A0A0Au,  // kFillLightGrayAlpha153
    0x00FFFFFFu,  // kFillWhiteAlpha0
    0x1AFFFFFFu,  // kFillWhiteAlpha26
    0xF50D0D0Du,  // kFillWhiteAlpha245
    0x0DFFFFFFu,  // kFillBlackAlpha13
    0x17FFFFFFu,  // kFillBlackAlpha23
    0x21FFFFFFu,  // kFillBlackAlpha33
    0x260B66DDu,  // kFillBrandHoverAlpha38
    0x330B66DDu,  // kFillBrandHoverAlpha51
    0xCC0A0A0Au,  // kFillLightGrayAlpha204
    0xD90A0A0Au,  // kFillAlmostWhiteAlpha217
    0xFA0A0A0Au,  // kFillAlmostWhiteAlpha250
    0xFF0C0C0Cu,  // kFillMessageRefreshHover
    0xFF111111u,  // kFillMessageRefreshPressed
    0xFF000000u,  // kFillAlmostWhite
    0xFF090909u,  // kFillPaleBlue
    0x8028466Eu,  // kFillPaleBlueAlpha128
    0x4D1ADB87u,  // kFillGreenAlpha77
    0x34FFFFFFu,  // kFillBlackAlpha52
    0xFF190B0Au,  // kFillLightRed
    0xFF0F0F0Fu,  // kFillWebViewProgressTrack
    0xFF007BFFu,  // kFillWebViewProgressValue
    0x99000000u,  // kFillBlackAlpha153
    0xFF0E0E0Eu,  // kFillTooltipBackground
    0x33FFFFFFu,  // kBorderInverseSubtle
    0x0FFFFFFFu,  // kBorderBlackAlpha15
    0x29FFFFFFu,  // kBorderBlackAlpha41
    0x1AFFFFFFu,  // kBorderBlackAlpha26
    0x33FFFFFFu,  // kBorderBlackAlpha51
    0x4CFFFFFFu,  // kBorderBlackAlpha76
    0x72FFFFFFu,  // kBorderBlackAlpha114
    0xB3121212u,  // kBorderWhiteAlpha179
    0xCC141414u,  // kBorderLightGrayAlpha204
    0x99141414u,  // kBorderLightGrayAlpha153
    0x18FFFFFFu,  // kBorderBlackAlpha24
    0x23FFFFFFu,  // kBorderBlackAlpha35
    0x4DFFFFFFu,  // kBorderBlackAlpha77
    0xFF252525u,  // kBorderGray204
    0xFF2C2C2Cu,  // kBorderGray187
    0xFF2D69B8u,  // kBorderBlueStrong
    0xFF225394u,  // kBorderBluePressed
    0x00000000u,  // kBorderTransparent
    0xFF683D39u,  // kBorderLightRed
    0xCC3A7BFCu,  // kFillBrandAlpha204
    0x14FFFFFFu,  // kBorderWhiteAlpha20
    0x04FFFFFFu,  // kFillBlackAlpha4
    0x06FFFFFFu,  // kFillBlackAlpha6
    0x73757575u,  // kBorderGray117Alpha115
    0xFF000000u,  // kFillWindowPanel
    0x990B0B0Bu,  // kFillGray34Alpha153
    0xB30B0B0Bu,  // kFillGray34Alpha179
    0x80000000u,  // kFillBlackAlpha77
    0xFFC94032u,  // kBorderRed201
    0xFF000000u,  // kFillBlackAlpha255
    0x01FFFFFFu,  // kBorderBlackAlpha1
    0x05FFFFFFu,  // kBorderBlackAlpha5
    0x08FFFFFFu,  // kBorderBlackAlpha8
    0x10FFFFFFu,  // kBorderBlackAlpha16
    0x14FFFFFFu,  // kBorderBlackAlpha20
    0x17FFFFFFu,  // kBorderBlackAlpha23
    0x19FFFFFFu,  // kBorderBlackAlpha25
    0x1FFFFFFFu,  // kBorderBlackAlpha31
    0x28FFFFFFu,  // kBorderBlackAlpha40
    0x34FFFFFFu,  // kBorderBlackAlpha52
    0x38FFFFFFu,  // kBorderBlackAlpha56
    0x59FFFFFFu,  // kBorderBlackAlpha89
    0x66FFFFFFu,  // kBorderBlackAlpha102
    0x9CFFFFFFu,  // kBorderBlackAlpha156
    0x80000000u,  // kBorderBlackAlpha204
    0xE6FFFFFFu,  // kBorderBlackAlpha230
    0xFFAAAAAAu,  // kBorderBlack
    0xFF0078D4u,  // kBorderWinBlue
    0xFF015EB5u,  // kBorderProgressBlueDark
    0xFFC8CED8u,  // kBorderDeepPrimary
    0xFF0B66DDu,  // kBorderBrandHover
    0xFF1740B0u,  // kBorderBrandPressed
    0x4D1ADB87u,  // kBorderGreenAlpha77
    0x99040404u,  // kBorderGray34Alpha153
    0xFF246FFEu,  // kBorderSelectedBlue
    0xFF275BD6u,  // kBorderLinkHover
    0x1A0E0E0Eu,  // kBorderDarkGray47Alpha26
    0xCC3A7BFCu,  // kBorderBrandAlpha204
    0xE63A7BFCu,  // kBorderBrandAlpha230
    0xFF3A7BFCu,  // kBorderBrand
    0x33757575u,  // kBorderGray117Alpha51
    0x66757575u,  // kBorderGray117Alpha102
    0x802E495Fu,  // kBorderPaleBlueAlpha128
    0x1A1F1F1Fu,  // kBorderLightBlueGrayAlpha26
    0xFF262626u,  // kBorderGray201
    0xFF222222u,  // kBorderGray208
    0xFF1F1F1Fu,  // kBorderCoolGray
    0xFF1E1E1Eu,  // kBorderGray216
    0xFF202020u,  // kBorderGray217
    0x4D202020u,  // kBorderGray218Alpha77
    0xFF111924u,  // kBorderHeaderBlue
    0xFF111924u,  // kBorderPaleBlue
    0xFF1B1B1Bu,  // kBorderGray224
    0xFF181818u,  // kBorderGray233
    0xFF181818u,  // kBorderGray237
    0xFF181818u,  // kBorderGray239
    0xFF161616u,  // kBorderGray240
    0xFF151515u,  // kBorderGray243
    0xFF161616u,  // kBorderGray245
    0xFF101010u,  // kBorderWindowPanel
    0xFF141414u,  // kBorderGray249
    0xFF121212u,  // kBorderGray249Blue
    0xFA131313u,  // kBorderAlmostWhiteAlpha250
    0x99121212u,  // kBorderWhiteAlpha153
    0xFF161616u,  // kBorderWhite
    0x38FFFFFFu,  // kFillBlackAlpha56
    0xC8E8E8E8u,  // kFillBlackAlpha76
    0x66000000u,  // kFillBlackAlpha102
    0x9A000000u,  // kFillBlackAlpha154
    0x9C000000u,  // kFillBlackAlpha156
    0xE6000000u,  // kFillBlackAlpha230
    0xFF005FB8u,  // kFillProgressBlue
    0xFF131313u,  // kFillDeepPrimary
    0xFF0B66DDu,  // kFillBrandHover
    0xFF282828u,  // kFillDarkGray25
    0xFF223FAAu,  // kFillLoginBrandPressed
    0xFF275BD6u,  // kFillLinkHover
    0x1AC8C8C8u,  // kFillDarkGray47Alpha26
    0xFF3183FFu,  // kFillAnimationBlue
    0xFF355ACFu,  // kFillLoginBrandHover
    0xFF4B7AF4u,  // kFillLoginBrand
    0xFF5D9EFFu,  // kFillBrandGradientEnd
    0xFF9B9D9Fu,  // kFillGray98
    0xFF7E8083u,  // kFillGray141
    0xFF2A2A2Au,  // kFillGray167Alt
    0xFF202020u,  // kFillGray182
    0xFF0B0B0Bu,  // kFillPrivacySafe
    0xFF0D0D0Du,  // kFillGray217
    0x4D0C0C0Cu,  // kFillGray218Alpha77
    0xFF131313u,  // kFillGray220
    0x00090909u,  // kFillPaleBlueAlpha0
    0xFF111111u,  // kFillGray224
    0xFF111111u,  // kFillGray224Alt
    0xFF101010u,  // kFillGray225
    0xFF0A0A0Au,  // kFillSafeLockGradientStart
    0xFF0E0E0Eu,  // kFillGray233
    0xFF0C0C0Cu,  // kFillCloudStatusBackground
    0xFF0C0C0Cu,  // kFillGray240
    0xFF070707u,  // kFillSafeLockGradientEnd
    0xFF0D0D0Du,  // kFillGray241Alt
    0xFF000000u,  // kFillPanelBackground
    0xFF0C0C0Cu,  // kFillGray245
    0xFF000000u,  // kFillGray249
    0xFF000000u,  // kFillGray249Blue
    0xFF111111u,  // kFillAnimationGray
    0xFA0A0A0Au,  // kFillAlmostWhite252Alpha250
    0xFF130E06u,  // kFillPrivacyWarning
    0xFFFF0000u,  // kFillRed
    0x800D0D0Du,  // kFillWhiteAlpha128
    0xB30D0D0Du,  // kFillWhiteAlpha179
    0xCC0D0D0Du,  // kFillWhiteAlpha204
    0x08000000u,  // kShadowBlackAlpha8
    0x0F000000u,  // kShadowBlackAlpha15
    0x1A000000u,  // kShadowBlackAlpha26
    0x26000000u,  // kShadowBlackAlpha38
    0x4D000000u,  // kShadowBlackAlpha77
    0xE6000000u,  // kShadowBlackAlpha230
    0xFF000000u,  // kShadowGray241
    0xFF000000u,  // kShadowGray249
    0x00000000u,  // kTextBlackAlpha0
    0xC5FFFFFFu,  // kTextBlackAlpha155
    0xC5FFFFFFu,  // kTextBlackAlpha156
    0xFF86898Du,  // kTextGray176
    0xFF83898Fu,  // kTextGray182
    0xFFDEEAFFu,  // kTextBrandLight
    0xFF484D54u,  // kTextGray233
    0xFFFF4444u,  // kTextBadgeRed
    0xFFFFFFE1u,  // kTextTooltipYellow
    0xE6FFFFFFu,  // kTextWhiteAlpha230
    0x66FFFFFFu,  // kTextWhiteAlpha102
    0xA63A7BFCu,  // kFillBrandAlpha166
    0xB3292929u,  // kFillGray170Alpha179
    0xE6141414u,  // kFillGray218Alpha230
    0x330C0C0Cu,  // kFillGray218Alpha51
    0x14FFFFFFu,  // kFillBlackAlpha20
    0x33141414u,  // kBorderGray249Alpha51
    0x4D141414u,  // kBorderGray249Alpha77
    0xFFC89300u,  // kTextAmber200
    0xFF969696u,  // kTextGray140
    0xFF333333u,  // kBorderGray166
    0xFF131313u,  // kFillTeachingTip
    0x73757575u,  // kBorderTeachingTip
    0xFFE2E6EAu,  // kTextBlueGray34
    0xFF858A8Du,  // kTextBlueGray176
    0xFF070707u,  // kFillPaleRed
    0xFF0D0D0Du,  // kFillBlueGray234
    0xFF0F0F0Fu,  // kFillBlueGray228
    0xFF000000u,  // kFillBlueGray248
    0xFF0C0C0Cu,  // kFillBlueGray242
    0xFF000000u,  // kFillBlueGray249
    0xFF000000u,  // kFillGray252
    0xFF1F1F1Fu,  // kBorderBlueGray216
    0xFF181818u,  // kBorderBlueGray239
    0xFF161616u,  // kBorderGray242
    0xFF1C1C1Cu,  // kBorderGray229
    0xFF000000u,  // kFillBlueGray248Alt
    0xFF1D1D1Du,  // kBorderBlueGray223
};

// Accent roles, for following the Windows accent colour (ThemeManager::setUseSystemAccent).
// -1 = not an accent colour. 0..6 = Windows AccentPalette shade (0 Light3, 1 Light2, 2 Light1,
// 3 Base, 4 Dark1, 5 Dark2, 6 Dark3); 7 = deep tint for backgrounds (uses Dark3).
// Accent buttons follow the Windows dark theme: rest = Light2, hover = Light1, pressed = Base
// (AccentFillColorDefault / Secondary / Tertiary), with dark text on them (TextOnAccentFillColorPrimary).
inline constexpr signed char kAccentRoleDark[kTokenCount] = {-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,-1,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,2,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3,3,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3,3,-1,2,3,-1,-1,1,2,-1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3,-1,2,-1,3,2,-1,1,2,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
inline constexpr signed char kAccentRoleAmoled[kTokenCount] = {-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,-1,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,2,2,-1,-1,-1,-1,-1,-1,-1,7,-1,-1,-1,-1,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,2,3,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3,3,-1,2,3,-1,-1,1,2,-1,1,1,1,-1,-1,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3,-1,2,-1,3,2,-1,1,2,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

}}} // namespace client::theme::palette
