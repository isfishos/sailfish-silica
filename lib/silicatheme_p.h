#ifndef SILICATHEME_P_H
#define SILICATHEME_P_H

#include <silicatheme.h>
#include <QColor>
#include <QSize>
#include <QUrl>
#include <MDConfGroup>
#include <MGConfItem>
#include <QScreen>
#include <QImageReader>
#include <QGuiApplication>

namespace Silica {

class ThemePrivate : public QObject
{
    Q_OBJECT

public:
    ThemePrivate();

    Theme::ColorScheme m_colorScheme;
    qreal m_pixelRatio;
    QString m_fontFamilyHeading;
    QString m_fontFamily;
    QColor m_highlightColor;
    QColor m_highlightBackgroundColor;
    QColor m_highlightDimmerColor;
    QColor m_primaryColor;
    QColor m_secondaryColor;
    QColor m_overlayBackgroundColor;
    QColor m_secondaryHighlightColor;
    QColor m_backgroundGlowColor;
    QColor m_errorColor;
    QColor m_wallpaperOverlayColor;
    QColor m_coverOverlayColor;
    QUrl m_backgroundImage;
    QUrl m_homeBackgroundImage;
    QUrl m_patternImage;
    QString m_backgroundMaterial;
    QString m_wallpaperFilter;

    // Font sizes
    int m_fontSizeTiny;
    int m_fontSizeExtraSmall;
    int m_fontSizeSmall;
    int m_fontSizeMedium;
    int m_fontSizeLarge;
    int m_fontSizeExtraLarge;
    int m_fontSizeHuge;

    // Base font sizes
    int m_fontSizeTinyBase;
    int m_fontSizeExtraSmallBase;
    int m_fontSizeSmallBase;
    int m_fontSizeMediumBase;
    int m_fontSizeHugeBase;
    int m_fontSizeLargeBase;
    int m_fontSizeExtraLargeBase;

    // Item sizes
    qreal m_itemSizeExtraSmall;
    qreal m_itemSizeSmall;
    qreal m_itemSizeMedium;
    qreal m_itemSizeLarge;
    qreal m_itemSizeExtraLarge;
    qreal m_itemSizeHuge;

    // Icon sizes
    qreal m_iconSizeExtraSmall;
    qreal m_iconSizeSmall;
    qreal m_iconSizeSmallPlus;
    qreal m_iconSizeMedium;
    qreal m_iconSizeLarge;
    qreal m_iconSizeExtraLarge;
    qreal m_iconSizeLauncher;

    // Button widths
    qreal m_buttonWidthExtraSmall;
    qreal m_buttonWidthSmall;
    qreal m_buttonWidthMedium;
    qreal m_buttonWidthLarge;

    // Cover sizes
    QSize m_coverSizeSmall;
    QSize m_coverSizeLarge;

    // Padding values
    qreal m_paddingSmall;
    qreal m_paddingMedium;
    qreal m_paddingLarge;

    // Line and margins
    qreal m_lineWidth;
    qreal m_horizontalPageMargin;

    // Animation and physics
    qreal m_flickDeceleration;
    qreal m_maximumFlickVelocity;
    float m_pageStackIndicatorWidth;
    float m_highlightBackgroundOpacity;
    int m_startDragDistance;
    int m_minimumPressHighlightTime;

    // Opacity values
    qreal m_opacityFaint;
    qreal m_opacityLow;
    qreal m_opacityHigh;
    qreal m_opacityOverlay;

    // Static colors
    QColor m_lightPrimaryColor;
    QColor m_lightSecondaryColor;
    QColor m_darkPrimaryColor;
    QColor m_darkSecondaryColor;

    QStringList launcherIconDirectories();
    QString highlightText(const QString &text, const QVariant &pattern, const QColor &color) const;

signals:
    void fontSizeTinyChanged();
    void fontSizeExtraSmallChanged(); 
    void fontSizeSmallChanged();
    void fontSizeMediumChanged();
    void fontSizeLargeChanged();
    void fontSizeExtraLargeChanged();
    void fontSizeHugeChanged();

private:
    bool m_autoScaleThemeConfValue;
    MGConfItem m_gcFontSizeCategory;
    MGConfItem m_gcFontSizeMultiplier; 
    MGConfItem m_gcFontSizeThreshold;
    MDConfGroup m_settings;
    QMetaObject m_themeColors;
    QMetaObject m_themeBackground;
    QMetaObject m_lightOnDarkDistanceField;
    QMetaObject m_darkOnLightDistanceField;
    QStringList m_launcherIconDirectories;
    
    void updateFontSizes();
    void commitAmbienceUpdate();
    void processTextChunk(QString &result, QString &remainingText, 
        int matchStart, int matchLength,
        const QString &format) const;

    // Theme parameter handling
    void loadThemeParameters();
    QVariant themeParamValue(const QVariant &defaultValue, const QString &param) const;
    QSize scaleSize(const QSize &size) const;
};

} // namespace Silica

#endif // SILICATHEME_P_H