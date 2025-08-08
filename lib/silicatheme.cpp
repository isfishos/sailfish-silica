#include "silicatheme.h"
#include "silicatheme_p.h"
#include "silicascreen.h"
#include "silicathemeiconresolver.h"
#include "themecolors.h"

#include <QDir>
#include <QFont>
#include <QPair>
#include <QRegularExpression>
#include <QtMath>

Q_GLOBAL_STATIC(Silica::Theme, themeInstance)
Q_GLOBAL_STATIC(Silica::ThemePrivate, themePrivateInstance)

namespace Silica {

ThemePrivate *ThemePrivate::instance()
{
    return themePrivateInstance();
}

ThemePrivate::ThemePrivate()
    : m_colorScheme(Theme::LightOnDark)
    , m_pixelRatio(-1.0)
    , m_fontFamilyHeading("Sail Sans Pro Light")
    , m_fontFamily("Sail Sans Pro Light")
    // GConf items
    , m_settings("/desktop/jolla/theme")
    , m_gcFontSizeCategory("/desktop/jolla/theme/font/sizeCategory")
    , m_gcFontSizeMultiplier("/desktop/jolla/theme/font/sizeMultiplier")
    , m_gcFontSizeThreshold("/desktop/jolla/theme/font/sizeThreshold")
    , m_themeColors(new ThemeColors(this))
    // Font sizes
    , m_fontSizeTiny(20)
    , m_fontSizeExtraSmall(24)
    , m_fontSizeSmall(28)
    , m_fontSizeMedium(32)
    , m_fontSizeLarge(40)
    , m_fontSizeExtraLarge(50)
    , m_fontSizeHuge(64)
    // Base font sizes
    , m_fontSizeTinyBase(20)
    , m_fontSizeExtraSmallBase(24)
    , m_fontSizeSmallBase(28)
    , m_fontSizeMediumBase(32)
    , m_fontSizeLargeBase(40)
    , m_fontSizeExtraLargeBase(50)
    , m_fontSizeHugeBase(64)
    // Item sizes
    , m_itemSizeExtraSmall(70)
    , m_itemSizeSmall(80)
    , m_itemSizeMedium(100)
    , m_itemSizeLarge(110)
    , m_itemSizeExtraLarge(135)
    , m_itemSizeHuge(180)
    // Icon sizes
    , m_iconSizeExtraSmall(24)
    , m_iconSizeSmall(32)
    , m_iconSizeSmallPlus(48)
    , m_iconSizeMedium(64)
    , m_iconSizeLarge(96)
    , m_iconSizeExtraLarge(128)
    , m_iconSizeLauncher(86)
    // Button widths
    , m_buttonWidthExtraSmall(148)
    , m_buttonWidthSmall(234)
    , m_buttonWidthMedium(292)
    , m_buttonWidthLarge(444)
    // Cover sizes
    , m_coverSizeSmall(148, 237)
    , m_coverSizeLarge(234, 374)
    // Padding values
    , m_paddingSmall(6)
    , m_paddingMedium(12)
    , m_paddingLarge(24)
    // Line and margins
    , m_lineWidth(2)
    , m_horizontalPageMargin(24)  // Same as paddingLarge
    // Animation and physics
    , m_flickDeceleration(1500)
    , m_maximumFlickVelocity(5000)
    , m_pageStackIndicatorWidth(37)
    , m_highlightBackgroundOpacity(0.3)
    , m_startDragDistance(20)  // Keeping existing value
    , m_minimumPressHighlightTime(64)
    // Opacity values
    , m_opacityFaint(0.2)
    , m_opacityLow(0.4)
    , m_opacityHigh(0.6)
    , m_opacityOverlay(0.8)
    // Static primary colors
    , m_lightPrimaryColor(255, 255, 255)
    , m_lightSecondaryColor(186, 186, 186)
    , m_darkPrimaryColor(0, 0, 0)
    , m_darkSecondaryColor(69, 69, 69)
{
    QObject::connect(&m_gcFontSizeCategory, &MGConfItem::valueChanged,
                    this, &ThemePrivate::updateFontSizes);
    QObject::connect(&m_gcFontSizeMultiplier, &MGConfItem::valueChanged,
                    this, &ThemePrivate::updateFontSizes);
    QObject::connect(&m_gcFontSizeThreshold, &MGConfItem::valueChanged,
                    this, &ThemePrivate::updateFontSizes);

    m_themeColors.data()->resolveMetaObject();

    // Load auto-scale configuration
    MGConfItem autoScaleConfiguration("/desktop/sailfish/silica/auto_scale_values");
    if (autoScaleConfiguration.value().isValid()) {
        m_autoScaleThemeConfValue = autoScaleConfiguration.value().toBool();
    }

    MGConfItem ratioConfiguration("/desktop/sailfish/silica/theme_pixel_ratio");
    if (ratioConfiguration.value().isValid()) {
        m_pixelRatio = ratioConfiguration.value().toFloat();
        if (m_pixelRatio <= 0.0) {
            qWarning() << "Invalid pixel ratio defined. Check your device configuration!";
        }
    } else {
        qWarning() << "No pixel ratio defined. Check your device configuration!";
    }

    // Set default pixel ratio if needed
    if (m_pixelRatio <= 0.0) {
        qWarning() << "Defaulting to pixel ratio 1.0";
        m_pixelRatio = 1.0;
    }

    // Load theme parameters
    loadThemeParameters();

    // Set default font
    QFont standardFont(m_fontFamily);
    standardFont.setPixelSize(m_fontSizeMedium);
    QGuiApplication::setFont(standardFont);

    updateFontSizes();
}


QVariant ThemePrivate::themeParamValue(const QVariant &defaultValue, const QString &param) const
{
    QVariant value = m_settings.property(param.toLatin1().constData());
    if (!value.isValid()) {
        return defaultValue;
    }
    return value;
}

static int percentOf(int value, int percent)
{
    return qRound(value * percent / 100.0);
}

// Static helper function for ensuring even pixel values
static int even(qreal value)
{
    return 2 * qFloor(value/2.0);
}

QSize ThemePrivate::scaleSize(const QSize &size) const
{
    return QSize(even(size.width() * m_pixelRatio),
                 even(size.height() * m_pixelRatio));
}

void ThemePrivate::loadThemeParameters()
{
    // Load font families
    m_fontFamily = themeParamValue(m_fontFamily, QStringLiteral("font_family")).toString();
    m_fontFamilyHeading = themeParamValue(m_fontFamilyHeading, QStringLiteral("font_family_heading")).toString();

    // Load base font sizes
    m_fontSizeTinyBase = themeParamValue(m_fontSizeTinyBase, QStringLiteral("font_size_tiny")).toInt();
    m_fontSizeExtraSmallBase = themeParamValue(m_fontSizeExtraSmallBase, QStringLiteral("font_size_extra_small")).toInt();
    m_fontSizeSmallBase = themeParamValue(m_fontSizeSmallBase, QStringLiteral("font_size_small")).toInt();
    m_fontSizeMediumBase = themeParamValue(m_fontSizeMediumBase, QStringLiteral("font_size_medium")).toInt();
    m_fontSizeLargeBase = themeParamValue(m_fontSizeLargeBase, QStringLiteral("font_size_large")).toInt();
    m_fontSizeExtraLargeBase = themeParamValue(m_fontSizeExtraLargeBase, QStringLiteral("font_size_extra_large")).toInt();
    m_fontSizeHugeBase = themeParamValue(m_fontSizeHugeBase, QStringLiteral("font_size_huge")).toInt();

    // Load item sizes
    m_itemSizeExtraSmall = themeParamValue(m_itemSizeExtraSmall, QStringLiteral("item_size_extra_small")).toReal();
    m_itemSizeSmall = themeParamValue(m_itemSizeSmall, QStringLiteral("item_size_small")).toReal();
    m_itemSizeMedium = themeParamValue(m_itemSizeMedium, QStringLiteral("item_size_medium")).toReal();
    m_itemSizeLarge = themeParamValue(m_itemSizeLarge, QStringLiteral("item_size_large")).toReal();
    m_itemSizeExtraLarge = themeParamValue(m_itemSizeExtraLarge, QStringLiteral("item_size_extra_large")).toReal();
    m_itemSizeHuge = themeParamValue(m_itemSizeHuge, QStringLiteral("item_size_huge")).toReal();

    // Load icon sizes
    m_iconSizeExtraSmall = themeParamValue(m_iconSizeExtraSmall, QStringLiteral("icon_size_extra_small")).toReal();
    m_iconSizeSmall = themeParamValue(m_iconSizeSmall, QStringLiteral("icon_size_small")).toReal();
    m_iconSizeSmallPlus = themeParamValue(m_iconSizeSmallPlus, QStringLiteral("icon_size_small_plus")).toReal();
    m_iconSizeMedium = themeParamValue(m_iconSizeMedium, QStringLiteral("icon_size_medium")).toReal();
    m_iconSizeLarge = themeParamValue(m_iconSizeLarge, QStringLiteral("icon_size_large")).toReal();
    m_iconSizeExtraLarge = themeParamValue(m_iconSizeExtraLarge, QStringLiteral("icon_size_extra_large")).toReal();

    // Special handling for launcher icon
    int iconSizeLauncher = themeParamValue(0, QStringLiteral("icon_size_launcher")).toInt();
    if (iconSizeLauncher > 0) {
        m_iconSizeLauncher = iconSizeLauncher;
    } else {
        QImageReader launcherIcon(ThemeIconResolver(m_pixelRatio).resolvePath("icon-launcher-default", m_colorScheme));
        if (!launcherIcon.canRead() || launcherIcon.size().width() == 0) {
            m_iconSizeLauncher = even(m_iconSizeLauncher * m_pixelRatio);
            qWarning() << "Failed to fetch the default launcher icon";
        } else {
            m_iconSizeLauncher = launcherIcon.size().width();
        }
    }

    // Load padding values
    m_paddingSmall = themeParamValue(m_paddingSmall, "padding_small").toReal();
    m_paddingMedium = 2 * m_paddingSmall;
    m_paddingLarge = 4 * m_paddingSmall;

    // Load line width
    m_lineWidth = themeParamValue(m_lineWidth, "line_width").toReal();

    // Calculate page margins based on screen size
    int horizontalPageMargin = themeParamValue(0, "horizontal_page_margin").toInt();
    if (horizontalPageMargin == 0) {
        // No conf value was set for horizontal page margin
        if (Screen::instance()->sizeCategory() <= Screen::Medium) {
            m_horizontalPageMargin = even(m_horizontalPageMargin * Screen::instance()->widthRatio());
        } else {
            m_horizontalPageMargin = 2 * m_paddingLarge;
        }
    } else {
        m_horizontalPageMargin = horizontalPageMargin;
    }

    // Load animation parameters
    m_pageStackIndicatorWidth = themeParamValue(m_pageStackIndicatorWidth, "page_stack_indicator_width").toFloat();
    m_flickDeceleration = themeParamValue(m_flickDeceleration, "flick_deceleration").toReal();
    m_maximumFlickVelocity = themeParamValue(m_maximumFlickVelocity, "maximum_flickVelocity").toReal();
}

void ThemePrivate::processTextChunk(QString &result, QString &remainingText,
                                  int matchStart, int matchLength,
                                  const QString &format) const
{
    // Add text before match
    if (matchStart > 0) {
        result.append(remainingText.left(matchStart).toHtmlEscaped());
    }

    // Add highlighted match
    QString match = remainingText.mid(matchStart, matchLength);
    result.append(format.arg(match.toHtmlEscaped()));

    // Update remaining text
    remainingText = remainingText.mid(matchStart + matchLength);
}

QString ThemePrivate::highlightText(const QString &text, const QVariant &pattern, const QColor &color) const
{
    if (text.isEmpty() || !pattern.isValid()) {
        return text.toHtmlEscaped();
    }

    QString result;
    QString remainingText = text;
    QString format = "<font color=\"%1\">%2</font>";
    format = format.arg(color.name());

    while (!remainingText.isEmpty()) {
        int matchStart = -1;
        int matchLength = 0;

        if (pattern.type() == QVariant::RegExp) {
            QRegExp rx = pattern.toRegExp();
            matchStart = rx.indexIn(remainingText);
            if (matchStart >= 0) {
                matchLength = rx.matchedLength();
            }
        }
        else if (pattern.type() == QVariant::RegularExpression) {
            QRegularExpression re = pattern.toRegularExpression();
            QRegularExpressionMatch match = re.match(remainingText);
            if (match.hasMatch()) {
                matchStart = match.capturedStart();
                matchLength = match.capturedLength();
            }
        }
        else {
            QString pat = pattern.toString();
            if (!pat.isEmpty()) {
                matchStart = remainingText.indexOf(pat, 0, Qt::CaseInsensitive);
                if (matchStart >= 0) {
                    matchLength = pat.length();
                }
            }
        }

        if (matchStart < 0) {
            // No more matches - add remaining text and break
            result.append(remainingText.toHtmlEscaped());
            break;
        }

        processTextChunk(result, remainingText, matchStart, matchLength, format);
    }

    return result;
}

void ThemePrivate::updateFontSizes()
{
    // Get config values with empty/invalid checks
    QString category = m_gcFontSizeCategory.value().toString();
    double fontSizeMultiplier = 1.0;
    int fontSizeThreshold = 0;

    if (m_gcFontSizeMultiplier.value().isValid()) {
        fontSizeMultiplier = m_gcFontSizeMultiplier.value().toDouble();
    }

    if (m_gcFontSizeThreshold.value().isValid()) {
        fontSizeThreshold = m_gcFontSizeThreshold.value().toInt();
    }

    // Adjust base multiplier based on category
    if (!category.isEmpty()) {
        if (category == QLatin1String("normal")) {
            fontSizeMultiplier = 1.0;
        } else if (category == QLatin1String("large")) {
            fontSizeMultiplier = 1.12;
        } else if (category == QLatin1String("huge")) {
            fontSizeMultiplier = 1.25;
        } else {
            qWarning() << "Invalid font size category defined:" << category;
        }
    }

    // Only apply changes if multiplier is valid
    if (fontSizeMultiplier > 0.0) {
        // Apply base multiplier to smaller sizes
        m_fontSizeTiny = qRound(m_fontSizeTinyBase * fontSizeMultiplier);
        m_fontSizeExtraSmall = qRound(m_fontSizeExtraSmallBase * fontSizeMultiplier);
        m_fontSizeSmall = qRound(m_fontSizeSmallBase * fontSizeMultiplier);
        m_fontSizeMedium = qRound(m_fontSizeMediumBase * fontSizeMultiplier);

        // Progressively reduce the scaling effect for larger sizes
        if (fontSizeMultiplier > 1.0) {
            fontSizeMultiplier = 1.0 + 0.7*(fontSizeMultiplier - 1.0);
        }
        m_fontSizeLarge = qRound(m_fontSizeLargeBase * fontSizeMultiplier);

        if (fontSizeMultiplier > 1.0) {
            fontSizeMultiplier = 1.0 + 0.7*(fontSizeMultiplier - 1.0);
        }
        m_fontSizeExtraLarge = qRound(m_fontSizeExtraLargeBase * fontSizeMultiplier);

        if (fontSizeMultiplier > 1.0) {
            fontSizeMultiplier = 1.0 + 0.7*(fontSizeMultiplier - 1.0);
        }
        m_fontSizeHuge = qRound(m_fontSizeHugeBase * fontSizeMultiplier);
    }

    // Apply threshold constraints if set
    if (fontSizeThreshold > 0) {
        m_fontSizeTiny = qMin(fontSizeThreshold, m_fontSizeTiny);
        m_fontSizeExtraSmall = qMin(fontSizeThreshold, m_fontSizeExtraSmall);
        m_fontSizeSmall = qMin(fontSizeThreshold, m_fontSizeSmall);
        m_fontSizeMedium = qMin(fontSizeThreshold, m_fontSizeMedium);
        m_fontSizeLarge = qMin(fontSizeThreshold, m_fontSizeLarge);
        m_fontSizeExtraLarge = qMin(fontSizeThreshold, m_fontSizeExtraLarge);
        m_fontSizeHuge = qMin(fontSizeThreshold, m_fontSizeHuge);
    }

    // Emit change signals
    emit fontSizeTinyChanged();
    emit fontSizeExtraSmallChanged();
    emit fontSizeSmallChanged();
    emit fontSizeMediumChanged();
    emit fontSizeLargeChanged();
    emit fontSizeExtraLargeChanged();
    emit fontSizeHugeChanged();
}

QStringList ThemePrivate::launcherIconDirectories()
{
    if (m_launcherIconDirectories.isEmpty()) {
        // Add apkd directory first
        m_launcherIconDirectories.append(QLatin1String("/var/lib/apkd/"));

        // Base directory for hicolor icons
        QDir iconDir("/usr/share/icons/hicolor");
        QRegularExpression sizeRegex("^(\\d+)x(?:\\d+)$");

        // Get list of icon directories
        QStringList dirs = iconDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

        // Store directories sorted by distance from launcher icon size
        QList<QPair<int, QString>> sortedDirs;

        // Parse each directory
        for (const QString& dir : dirs) {
            QRegularExpressionMatch match = sizeRegex.match(dir);
            if (match.hasMatch()) {
                int size = match.captured(1).toInt();
                int distance = size - m_iconSizeLauncher;
                QString path = iconDir.absoluteFilePath(dir) + "/apps/";

                // Insert keeping sorted by distance from launcher size
                int i;
                for(i = 0; i < sortedDirs.size(); i++) {
                    const int curDist = sortedDirs[i].first;
                    if ((distance >= -1 && distance < curDist) ||
                        (distance < -1 && distance > curDist)) {
                        sortedDirs.insert(i, qMakePair(distance, path));
                        break;
                    }
                }
                if (i == sortedDirs.size()) {
                    sortedDirs.append(qMakePair(distance, path));
                }
            }
        }

        // Build final list preserving the sort order
        for (const auto& dir : sortedDirs) {
            m_launcherIconDirectories << dir.second;
        }
    }

    return m_launcherIconDirectories;
}

static Theme *s_instance = nullptr;

Theme *Theme::instance()
{
    if (!s_instance) {
        s_instance = new Theme();
    }
    return s_instance;
}

Theme::Theme(QObject *parent)
    : QObject(parent)
    , m_private(new ThemePrivate)
{
    // Connect font size change signals
    connect(m_private, &ThemePrivate::fontSizeTinyChanged,
            this, &Theme::fontSizeTinyChanged);
    connect(m_private, &ThemePrivate::fontSizeExtraSmallChanged,
            this, &Theme::fontSizeExtraSmallChanged);
    connect(m_private, &ThemePrivate::fontSizeSmallChanged,
            this, &Theme::fontSizeSmallChanged);
    connect(m_private, &ThemePrivate::fontSizeMediumChanged,
            this, &Theme::fontSizeMediumChanged);
    connect(m_private, &ThemePrivate::fontSizeLargeChanged,
            this, &Theme::fontSizeLargeChanged);
    connect(m_private, &ThemePrivate::fontSizeExtraLargeChanged,
            this, &Theme::fontSizeExtraLargeChanged);
    connect(m_private, &ThemePrivate::fontSizeHugeChanged,
            this, &Theme::fontSizeHugeChanged);
}

Theme::~Theme()
{
    delete m_private;
}

qreal Theme::pixelRatio() const
{
    return m_private->m_pixelRatio;
}

Theme::ColorScheme Theme::colorScheme() const
{
    return m_private->m_colorScheme;
}

void Theme::setColorScheme(ColorScheme colorScheme)
{
    if (m_private->m_colorScheme != colorScheme) {
        m_private->m_colorScheme = colorScheme;
        emit colorSchemeChanged();
    }
}

// Basic property getters
QString Theme::fontFamilyHeading() const { return m_private->m_fontFamilyHeading; }
QString Theme::fontFamily() const { return m_private->m_fontFamily; }
QColor Theme::highlightColor() const { return m_private->m_highlightColor; }
QColor Theme::highlightBackgroundColor() const { return m_private->m_highlightBackgroundColor; }
QColor Theme::highlightDimmerColor() const { return m_private->m_highlightDimmerColor; }
QColor Theme::primaryColor() const { return m_private->m_primaryColor; }
QColor Theme::secondaryColor() const { return m_private->m_secondaryColor; }
QColor Theme::overlayBackgroundColor() const { return m_private->m_overlayBackgroundColor; }
QColor Theme::secondaryHighlightColor() const { return m_private->m_secondaryHighlightColor; }
QColor Theme::backgroundGlowColor() const { return m_private->m_backgroundGlowColor; }
QColor Theme::errorColor() const { return m_private->m_errorColor; }
QColor Theme::wallpaperOverlayColor() const { return m_private->m_wallpaperOverlayColor; }
QColor Theme::coverOverlayColor() const { return m_private->m_coverOverlayColor; }
QUrl Theme::backgroundImage() const { return m_private->m_backgroundImage; }
QUrl Theme::homeBackgroundImage() const { return m_private->m_homeBackgroundImage; }
QUrl Theme::patternImage() const { return m_private->m_patternImage; }
QString Theme::backgroundMaterial() const { return m_private->m_backgroundMaterial; }
QString Theme::wallpaperFilter() const { return m_private->m_wallpaperFilter; }

// Font size getters
int Theme::fontSizeTiny() const { return m_private->m_fontSizeTiny; }
int Theme::fontSizeExtraSmall() const { return m_private->m_fontSizeExtraSmall; }
int Theme::fontSizeSmall() const { return m_private->m_fontSizeSmall; }
int Theme::fontSizeMedium() const { return m_private->m_fontSizeMedium; }
int Theme::fontSizeLarge() const { return m_private->m_fontSizeLarge; }
int Theme::fontSizeExtraLarge() const { return m_private->m_fontSizeExtraLarge; }
int Theme::fontSizeHuge() const { return m_private->m_fontSizeHuge; }

// Base font sizes
int Theme::fontSizeTinyBase() const { return m_private->m_fontSizeTinyBase; }
int Theme::fontSizeExtraSmallBase() const { return m_private->m_fontSizeExtraSmallBase; }
int Theme::fontSizeSmallBase() const { return m_private->m_fontSizeSmallBase; }
int Theme::fontSizeMediumBase() const { return m_private->m_fontSizeMediumBase; }
int Theme::fontSizeLargeBase() const { return m_private->m_fontSizeLargeBase; }
int Theme::fontSizeExtraLargeBase() const { return m_private->m_fontSizeExtraLargeBase; }
int Theme::fontSizeHugeBase() const { return m_private->m_fontSizeHugeBase; }

// Item sizes
qreal Theme::itemSizeExtraSmall() const { return m_private->m_itemSizeExtraSmall; }
qreal Theme::itemSizeSmall() const { return m_private->m_itemSizeSmall; }
qreal Theme::itemSizeMedium() const { return m_private->m_itemSizeMedium; }
qreal Theme::itemSizeLarge() const { return m_private->m_itemSizeLarge; }
qreal Theme::itemSizeExtraLarge() const { return m_private->m_itemSizeExtraLarge; }
qreal Theme::itemSizeHuge() const { return m_private->m_itemSizeHuge; }

// Icon sizes
qreal Theme::iconSizeExtraSmall() const { return m_private->m_iconSizeExtraSmall; }
qreal Theme::iconSizeSmall() const { return m_private->m_iconSizeSmall; }
qreal Theme::iconSizeSmallPlus() const { return m_private->m_iconSizeSmallPlus; }
qreal Theme::iconSizeMedium() const { return m_private->m_iconSizeMedium; }
qreal Theme::iconSizeLarge() const { return m_private->m_iconSizeLarge; }
qreal Theme::iconSizeExtraLarge() const { return m_private->m_iconSizeExtraLarge; }
qreal Theme::iconSizeLauncher() const { return m_private->m_iconSizeLauncher; }

// Button widths
qreal Theme::buttonWidthTiny() const { return m_private->m_itemSizeExtraSmall; }
qreal Theme::buttonWidthExtraSmall() const { return m_private->m_buttonWidthExtraSmall; }
qreal Theme::buttonWidthSmall() const { return m_private->m_buttonWidthSmall; }
qreal Theme::buttonWidthMedium() const { return m_private->m_buttonWidthMedium; }
qreal Theme::buttonWidthLarge() const { return m_private->m_buttonWidthLarge; }

// Cover sizes
QSize Theme::coverSizeSmall() const { return m_private->m_coverSizeSmall; }
QSize Theme::coverSizeLarge() const { return m_private->m_coverSizeLarge; }

// Padding values
qreal Theme::paddingSmall() const { return m_private->m_paddingSmall; }
qreal Theme::paddingMedium() const { return m_private->m_paddingMedium; }
qreal Theme::paddingLarge() const { return m_private->m_paddingLarge; }

// Line and margins
qreal Theme::lineWidth() const { return m_private->m_lineWidth; }
qreal Theme::horizontalPageMargin() const { return m_private->m_horizontalPageMargin; }

// Animation and physics related
qreal Theme::flickDeceleration() const { return m_private->m_flickDeceleration; }
qreal Theme::maximumFlickVelocity() const { return m_private->m_maximumFlickVelocity; }
float Theme::pageStackIndicatorWidth() const { return m_private->m_pageStackIndicatorWidth; }
float Theme::highlightBackgroundOpacity() const { return m_private->m_highlightBackgroundOpacity; }
int Theme::startDragDistance() const { return m_private->m_startDragDistance; }
int Theme::minimumPressHighlightTime() const { return m_private->m_minimumPressHighlightTime; }

// Opacity values
qreal Theme::opacityFaint() const { return m_private->m_opacityFaint; }
qreal Theme::opacityLow() const { return m_private->m_opacityLow; }
qreal Theme::opacityHigh() const { return m_private->m_opacityHigh; }
qreal Theme::opacityOverlay() const { return m_private->m_opacityOverlay; }

// Theme colors
QColor Theme::lightPrimaryColor() const { return m_private->m_lightPrimaryColor; }
QColor Theme::lightSecondaryColor() const { return m_private->m_lightSecondaryColor; }
QColor Theme::darkPrimaryColor() const { return m_private->m_darkPrimaryColor; }
QColor Theme::darkSecondaryColor() const { return m_private->m_darkSecondaryColor; }

QStringList Theme::launcherIconDirectories()
{
    return QStringList() << "/usr/share/icons/hicolor/86x86/apps"
                        << "/usr/share/icons/hicolor/108x108/apps"
                        << "/usr/share/icons/hicolor/128x128/apps"
                        << "/usr/share/icons/hicolor/256x256/apps";
    // For now return hardcoded paths - later can switch to: m_private->launcherIconDirectories();
}

// Theme color calculation methods
QColor Theme::highlightFromColor(const QColor &color, ColorScheme scheme)
{
    QColor c = color.isValid() ? color : (scheme == LightOnDark ? QColor(0, 174, 255) : QColor(0, 122, 204));
    qreal h, s, l, a;
    c.getHslF(&h, &s, &l, &a);
    if (scheme == LightOnDark) {
        s = qMin(1.0, s * 1.1);
        l = qMin(1.0, l * 1.15);
    } else {
        s = qMin(1.0, s * 0.9);
        l = qMax(0.0, l * 0.65);
    }
    QColor out;
    out.setHslF(h, s, l, a);
    return out;
}

QColor Theme::secondaryHighlightFromColor(const QColor &color, ColorScheme scheme)
{
    QColor c = highlightFromColor(color, scheme);
    qreal h, s, l, a;
    c.getHslF(&h, &s, &l, &a);
    s = qMin(1.0, s * 0.6);
    l = qMin(1.0, l * 0.95);
    QColor out;
    out.setHslF(h, s, l, a);
    return out;
}

QColor Theme::highlightBackgroundFromColor(const QColor &color, ColorScheme scheme)
{
    QColor c = highlightFromColor(color, scheme);
    c.setAlphaF(Theme::instance()->highlightBackgroundOpacity());
    return c;
}

QColor Theme::highlightDimmerFromColor(const QColor &color, ColorScheme scheme)
{
    QColor c = highlightFromColor(color, scheme);
    qreal h, s, l, a;
    c.getHslF(&h, &s, &l, &a);
    l = qMax(0.0, l * 0.5);
    QColor out;
    out.setHslF(h, s, l, a);
    out.setAlphaF(qMin(1.0, Theme::instance()->highlightBackgroundOpacity() * 1.5));
    return out;
}

QString Theme::_encodeTextLinks(const QString &text, bool shortenUrl)
{
    if (text.isEmpty()) return text;
    QString out;
    out.reserve(text.size() + 16);
    QRegularExpression urlRe("(https?://|www\\.)[\\w\-._~:/?#\\[\\]@!$&'()*+,;=%]+",
                             QRegularExpression::CaseInsensitiveOption);
    QRegularExpression emailRe("[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,}",
                               QRegularExpression::CaseInsensitiveOption);
    auto encode = [&](const QString &input, const QRegularExpression &re, bool isUrl) {
        QString result;
        int last = 0;
        auto it = re.globalMatch(input);
        while (it.hasNext()) {
            auto m = it.next();
            result += input.midRef(last, m.capturedStart() - last).toString().toHtmlEscaped();
            QString link = m.captured();
            QString href = link;
            if (isUrl && href.startsWith("www.", Qt::CaseInsensitive)) {
                href.prepend("http://");
            }
            QString label = link;
            if (shortenUrl && isUrl) {
                // Keep scheme/host, elide path
                int slash = label.indexOf('/', label.startsWith("http") ? label.indexOf("://") + 3 : 0);
                if (slash > 0 && slash < label.size()-1) {
                    label = label.left(slash+1) + "…";
                }
            }
            result += QStringLiteral("<a href=\"") + href.toHtmlEscaped() + QStringLiteral("\">")
                    + label.toHtmlEscaped() + QStringLiteral("</a>");
            last = m.capturedEnd();
        }
        result += input.midRef(last).toString().toHtmlEscaped();
        return result;
    };

    // First URLs, then emails on the remaining text prevents nested anchors
    QString tmp = encode(text, urlRe, true);
    return encode(tmp, emailRe, false);
}

void Theme::_setHighlightColor(QColor color)
{
    if (m_private->m_highlightColor != color) {
        m_private->m_highlightColor = color;
        emit highlightColorChanged();
    }
}

QString Theme::highlightText(const QString &text, const QVariant &pattern, const QColor &color)
{
    return m_private->highlightText(text, pattern, color);
}

QColor Theme::rgba(QColor color, qreal opacity) const
{
    QColor result(color);
    result.setAlphaF(opacity);
    return result;
}

QColor Theme::presenceColor(PresenceMode presenceMode) const
{
    switch (presenceMode) {
    case PresenceAvailable:
        return QColor("#00ff23");  // Bright green for active status
    case PresenceAway:
        return QColor("#ffa600");  // Orange for away status
    case PresenceBusy:
        return QColor("#ff0f00");  // Red for busy/do not disturb
    default:
        return QColor("#999999");  // Gray for offline/unknown status
    }
}

QString Theme::iconForMimeType(QString mimeType) const
{
    static QHash<QString,QString> mimeIconMap = {
        // Android packages
        { "application/vnd.android.package-archive", "apk" },

        // Archives
        { "application/x-7z-compressed", "archive-folder" },
        { "application/x-bzip-compressed-tar", "archive-folder" },
        { "application/x-compressed-tar", "archive-folder" },
        { "application/x-java-archive", "archive-folder" },
        { "application/x-tar", "archive-folder" },
        { "application/x-xz-compressed-tar", "archive-folder" },
        { "application/zip", "archive-folder" },
        { "application/vnd.comicbook+zip", "archive-folder" },

        // Folders
        { "inode/directory", "folder" },

        // Formatted documents
        { "application/msword", "formatted" },
        { "application/rtf", "formatted" },
        { "application/vnd.ms-works", "formatted" },
        { "application/vnd.oasis.opendocument.text", "formatted" },
        { "application/vnd.oasis.opendocument.text-master", "formatted" },
        { "application/vnd.oasis.opendocument.text-template", "formatted" },
        { "application/vnd.openxmlformats-officedocument.wordprocessingml.document", "formatted" },
        { "application/vnd.openxmlformats-officedocument.wordprocessingml.template", "formatted" },
        { "application/x-iwork-pages-sffpages", "formatted" },
        { "application/x-mswrite", "formatted" },

        // Notes
        { "text/plain", "note" },
        { "text/x-vnote", "note" },

        // PDF documents
        { "application/pdf", "pdf" },

        // Presentations
        { "application/vnd.ms-powerpoint", "presentation" },
        { "application/vnd.oasis.opendocument.presentation", "presentation" },
        { "application/vnd.oasis.opendocument.presentation-template", "presentation" },
        { "application/vnd.openxmlformats-officedocument.presentationml.presentation", "presentation" },
        { "application/vnd.openxmlformats-officedocument.presentationml.template", "presentation" },
        { "application/x-iwork-keynote-sffkey", "presentation" },
        { "application/x-kpresenter", "presentation" },

        // RPM packages
        { "application/x-rpm", "rpm" },

        // Spreadsheets
        { "application/vnd.ms-excel", "spreadsheet" },
        { "application/vnd.oasis.opendocument.spreadsheet", "spreadsheet" },
        { "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", "spreadsheet" },
        { "application/vnd.openxmlformats-officedocument.spreadsheetml.template", "spreadsheet" },
        { "application/x-iwork-numbers-sffnumbers", "spreadsheet" },
        { "application/x-kspread", "spreadsheet" },
        { "text/csv", "spreadsheet" },

        // Contact cards
        { "text/vcard", "vcard" },
        { "text/x-vcard", "vcard" }
    };

    QString iconType;

    // First check exact matches
    if (mimeIconMap.contains(mimeType)) {
        iconType = mimeIconMap.value(mimeType);
    }
    // Then check mime type categories
    else if (mimeType.startsWith("audio/")) {
        iconType = "audio";
    }
    else if (mimeType.startsWith("image/")) {
        iconType = "image";
    }
    else if (mimeType.startsWith("video/")) {
        iconType = "video";
    }
    else {
        iconType = "other";
    }

    return QLatin1String("image://theme/icon-m-file-") + iconType;
}

qreal Theme::dp(qreal size) const
{
    return size * m_private->m_pixelRatio;
}

} // namespace Silica
