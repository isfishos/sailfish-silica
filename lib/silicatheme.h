/****************************************************************************************
**
** Copyright (C) 2020 Open Mobile Platform LLC.
** Copyright (C) 2013 Jolla Ltd.
** Contact: Joona Petrell <joona.petrell@jollamobile.com>
** All rights reserved.
**
** This file is part of Sailfish Silica UI component package.
**
** You may use this file under the terms of the GNU Lesser General
** Public License version 2.1 as published by the Free Software Foundation
** and appearing in the file license.lgpl included in the packaging
** of this file.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file license.lgpl included in the packaging
** of this file.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
** 
****************************************************************************************/

#ifndef SILICATHEME_H
#define SILICATHEME_H

#include <silicaglobal.h>

#include <QObject>
#include <QVariant>
#include <QColor>
#include <QSize>
#include <QString>
#include <QUrl>

namespace Silica
{

class ThemePrivate;

class SAILFISH_SILICA_EXPORT Theme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal pixelRatio READ pixelRatio CONSTANT FINAL)
    Q_PROPERTY(ColorScheme colorScheme READ colorScheme NOTIFY colorSchemeChanged FINAL)

    Q_PROPERTY(QString fontFamilyHeading READ fontFamilyHeading NOTIFY fontFamilyHeadingChanged FINAL)
    Q_PROPERTY(QString fontFamily READ fontFamily NOTIFY fontFamilyChanged FINAL)

    Q_PROPERTY(int fontSizeTiny READ fontSizeTiny NOTIFY fontSizeTinyChanged FINAL)
    Q_PROPERTY(int fontSizeExtraSmall READ fontSizeExtraSmall NOTIFY fontSizeExtraSmallChanged FINAL)
    Q_PROPERTY(int fontSizeSmall READ fontSizeSmall NOTIFY fontSizeSmallChanged FINAL)
    Q_PROPERTY(int fontSizeMedium READ fontSizeMedium NOTIFY fontSizeMediumChanged FINAL)
    Q_PROPERTY(int fontSizeLarge READ fontSizeLarge NOTIFY fontSizeLargeChanged FINAL)
    Q_PROPERTY(int fontSizeExtraLarge READ fontSizeExtraLarge NOTIFY fontSizeExtraLargeChanged FINAL)
    Q_PROPERTY(int fontSizeHuge READ fontSizeHuge NOTIFY fontSizeHugeChanged FINAL)

    Q_PROPERTY(int fontSizeTinyBase READ fontSizeTinyBase CONSTANT FINAL)
    Q_PROPERTY(int fontSizeExtraSmallBase READ fontSizeExtraSmallBase CONSTANT FINAL)
    Q_PROPERTY(int fontSizeSmallBase READ fontSizeSmallBase CONSTANT FINAL)
    Q_PROPERTY(int fontSizeMediumBase READ fontSizeMediumBase CONSTANT FINAL)
    Q_PROPERTY(int fontSizeLargeBase READ fontSizeLargeBase CONSTANT FINAL)
    Q_PROPERTY(int fontSizeExtraLargeBase READ fontSizeExtraLargeBase CONSTANT FINAL)
    Q_PROPERTY(int fontSizeHugeBase READ fontSizeHugeBase CONSTANT FINAL)

    Q_PROPERTY(qreal itemSizeExtraSmall READ itemSizeExtraSmall CONSTANT FINAL)
    Q_PROPERTY(qreal itemSizeSmall READ itemSizeSmall CONSTANT FINAL)
    Q_PROPERTY(qreal itemSizeMedium READ itemSizeMedium CONSTANT FINAL)
    Q_PROPERTY(qreal itemSizeLarge READ itemSizeLarge CONSTANT FINAL)
    Q_PROPERTY(qreal itemSizeExtraLarge READ itemSizeExtraLarge CONSTANT FINAL)
    Q_PROPERTY(qreal itemSizeHuge READ itemSizeHuge CONSTANT FINAL)

    Q_PROPERTY(qreal iconSizeExtraSmall READ iconSizeExtraSmall CONSTANT FINAL)
    Q_PROPERTY(qreal iconSizeSmall READ iconSizeSmall CONSTANT FINAL)
    Q_PROPERTY(qreal iconSizeSmallPlus READ iconSizeSmallPlus CONSTANT FINAL)
    Q_PROPERTY(qreal iconSizeMedium READ iconSizeMedium CONSTANT FINAL)
    Q_PROPERTY(qreal iconSizeLarge READ iconSizeLarge CONSTANT FINAL)
    Q_PROPERTY(qreal iconSizeExtraLarge READ iconSizeExtraLarge CONSTANT FINAL)
    Q_PROPERTY(qreal iconSizeLauncher READ iconSizeLauncher CONSTANT FINAL)

    Q_PROPERTY(QStringList launcherIconDirectories READ launcherIconDirectories CONSTANT FINAL)

    Q_PROPERTY(qreal buttonWidthTiny READ buttonWidthTiny CONSTANT FINAL)
    Q_PROPERTY(qreal buttonWidthExtraSmall READ buttonWidthExtraSmall CONSTANT FINAL)
    Q_PROPERTY(qreal buttonWidthSmall READ buttonWidthSmall CONSTANT FINAL)
    Q_PROPERTY(qreal buttonWidthMedium READ buttonWidthMedium CONSTANT FINAL)
    Q_PROPERTY(qreal buttonWidthLarge READ buttonWidthLarge CONSTANT FINAL)

    Q_PROPERTY(QSize coverSizeSmall READ coverSizeSmall CONSTANT FINAL)
    Q_PROPERTY(QSize coverSizeLarge READ coverSizeLarge CONSTANT FINAL)

    Q_PROPERTY(qreal paddingSmall READ paddingSmall CONSTANT FINAL)
    Q_PROPERTY(qreal paddingMedium READ paddingMedium CONSTANT FINAL)
    Q_PROPERTY(qreal paddingLarge READ paddingLarge CONSTANT FINAL)

    Q_PROPERTY(qreal _lineWidth READ lineWidth CONSTANT FINAL)

    Q_PROPERTY(qreal horizontalPageMargin READ horizontalPageMargin CONSTANT FINAL)

    Q_PROPERTY(qreal flickDeceleration READ flickDeceleration CONSTANT FINAL)
    Q_PROPERTY(qreal maximumFlickVelocity READ maximumFlickVelocity CONSTANT FINAL)

    Q_PROPERTY(float pageStackIndicatorWidth READ pageStackIndicatorWidth CONSTANT FINAL)
    Q_PROPERTY(float highlightBackgroundOpacity READ highlightBackgroundOpacity CONSTANT FINAL)
    Q_PROPERTY(QColor highlightColor READ highlightColor NOTIFY highlightColorChanged FINAL)
    Q_PROPERTY(QColor highlightBackgroundColor READ highlightBackgroundColor NOTIFY highlightBackgroundColorChanged FINAL)
    Q_PROPERTY(QColor highlightDimmerColor READ highlightDimmerColor NOTIFY highlightDimmerColorChanged FINAL)
    Q_PROPERTY(QColor primaryColor READ primaryColor NOTIFY primaryColorChanged FINAL)
    Q_PROPERTY(QColor secondaryColor READ secondaryColor NOTIFY secondaryColorChanged FINAL)
    Q_PROPERTY(QColor overlayBackgroundColor READ overlayBackgroundColor NOTIFY overlayBackgroundColorChanged FINAL)
    Q_PROPERTY(QColor secondaryHighlightColor READ secondaryHighlightColor NOTIFY secondaryHighlightColorChanged FINAL)
    Q_PROPERTY(QColor backgroundGlowColor READ backgroundGlowColor NOTIFY backgroundGlowColorChanged FINAL)
    Q_PROPERTY(QColor errorColor READ errorColor NOTIFY errorColorChanged FINAL)
    Q_PROPERTY(QColor _wallpaperOverlayColor READ wallpaperOverlayColor NOTIFY wallpaperOverlayColorChanged)
    Q_PROPERTY(QColor _coverOverlayColor READ coverOverlayColor NOTIFY coverOverlayColorChanged)

    Q_PROPERTY(QUrl backgroundImage READ backgroundImage NOTIFY backgroundImageChanged)
    Q_PROPERTY(QUrl _homeBackgroundImage READ homeBackgroundImage NOTIFY homeBackgroundImageChanged)

    Q_PROPERTY(QUrl _patternImage READ patternImage NOTIFY patternImageChanged)
    Q_PROPERTY(QUrl _backgroundMaterial READ backgroundMaterial NOTIFY backgroundMaterialChanged)
    Q_PROPERTY(QUrl _wallpaperFilter READ wallpaperFilter NOTIFY wallpaperFilterChanged)

    Q_PROPERTY(QColor lightPrimaryColor READ lightPrimaryColor CONSTANT FINAL)
    Q_PROPERTY(QColor lightSecondaryColor READ lightSecondaryColor CONSTANT FINAL)
    Q_PROPERTY(QColor darkPrimaryColor READ darkPrimaryColor CONSTANT FINAL)
    Q_PROPERTY(QColor darkSecondaryColor READ darkSecondaryColor CONSTANT FINAL)

    Q_PROPERTY(int startDragDistance READ startDragDistance CONSTANT FINAL)

    Q_PROPERTY(int minimumPressHighlightTime READ minimumPressHighlightTime CONSTANT FINAL)

    Q_PROPERTY(qreal opacityFaint READ opacityFaint CONSTANT FINAL)
    Q_PROPERTY(qreal opacityLow READ opacityLow CONSTANT FINAL)
    Q_PROPERTY(qreal opacityHigh READ opacityHigh CONSTANT FINAL)
    Q_PROPERTY(qreal opacityOverlay READ opacityOverlay CONSTANT FINAL)

public:
     enum PresenceMode {
         PresenceAvailable,
         PresenceAway,
         PresenceBusy,
         PresenceOffline
     };
     Q_ENUM(PresenceMode)

     enum ColorScheme {
         LightOnDark,
         DarkOnLight
     };
     Q_ENUM(ColorScheme)

    // For C++
    static Theme *instance();

    // For QML plugin
    explicit Theme(QObject *parent = 0);
    ~Theme();

    qreal pixelRatio() const;
    ColorScheme colorScheme() const;
    Q_INVOKABLE void setColorScheme(ColorScheme colorScheme);
    Q_INVOKABLE void _setHighlightColor(QColor color);

    QString fontFamilyHeading() const;
    QString fontFamily() const;
    int fontSizeTiny() const;
    int fontSizeExtraSmall() const;
    int fontSizeSmall() const;
    int fontSizeMedium() const;
    int fontSizeLarge() const;
    int fontSizeExtraLarge() const;
    int fontSizeHuge() const;
    int fontSizeTinyBase() const;
    int fontSizeExtraSmallBase() const;
    int fontSizeSmallBase() const;
    int fontSizeMediumBase() const;
    int fontSizeLargeBase() const;
    int fontSizeExtraLargeBase() const;
    int fontSizeHugeBase() const;
    QSize coverSizeSmall() const;
    QSize coverSizeLarge() const;
    qreal itemSizeExtraSmall() const;
    qreal itemSizeSmall() const;
    qreal itemSizeMedium() const;
    qreal itemSizeLarge() const;
    qreal itemSizeExtraLarge() const;
    qreal itemSizeHuge() const;
    qreal iconSizeExtraSmall() const;
    qreal iconSizeSmall() const;
    qreal iconSizeSmallPlus() const;
    qreal iconSizeMedium() const;
    qreal iconSizeLarge() const;
    qreal iconSizeExtraLarge() const;
    qreal iconSizeLauncher() const;
    qreal buttonWidthTiny() const;
    qreal buttonWidthExtraSmall() const;
    qreal buttonWidthSmall() const;
    qreal buttonWidthMedium() const;
    qreal buttonWidthLarge() const;
    qreal paddingSmall() const;
    qreal paddingMedium() const;
    qreal paddingLarge() const;
    qreal lineWidth() const;
    qreal horizontalPageMargin() const;
    qreal flickDeceleration() const;
    qreal maximumFlickVelocity() const;

    QStringList launcherIconDirectories();

    QColor highlightColor() const;
    QColor highlightBackgroundColor() const;
    QColor highlightDimmerColor() const;
    QColor primaryColor() const;
    QColor secondaryColor() const;
    QColor secondaryHighlightColor() const;
    QColor lightPrimaryColor() const;
    QColor lightSecondaryColor() const;
    QColor darkPrimaryColor() const;
    QColor darkSecondaryColor() const;
    QColor overlayBackgroundColor() const;
    QColor backgroundGlowColor() const;
    QColor errorColor() const;
    QColor wallpaperOverlayColor() const;
    float pageStackIndicatorWidth() const;
    float highlightBackgroundOpacity() const;
    QUrl backgroundImage() const;
    QUrl homeBackgroundImage() const;
    QUrl patternImage() const;
    QString backgroundMaterial() const;
    QString wallpaperFilter() const;
    int startDragDistance() const;
    int minimumPressHighlightTime() const;
    qreal opacityFaint() const;
    qreal opacityLow() const;
    qreal opacityHigh() const;
    qreal opacityOverlay() const;

    Q_INVOKABLE QString highlightText(const QString &text, const QVariant &pattern, const QColor &color);
    Q_INVOKABLE QColor rgba(QColor color, qreal opacity) const;
    Q_INVOKABLE QColor presenceColor(PresenceMode presenceMode) const;
    Q_INVOKABLE QString iconForMimeType(QString mimeType) const;
    Q_INVOKABLE qreal dp(qreal size) const;

    Q_INVOKABLE static QColor highlightFromColor(const QColor &color, Silica::Theme::ColorScheme scheme);
    Q_INVOKABLE static QColor secondaryHighlightFromColor(const QColor &color, Silica::Theme::ColorScheme scheme);
    Q_INVOKABLE static QColor highlightBackgroundFromColor(const QColor &color, Silica::Theme::ColorScheme scheme);
    Q_INVOKABLE static QColor highlightDimmerFromColor(const QColor &color, Silica::Theme::ColorScheme scheme);

    static QString _encodeTextLinks(const QString &text, bool shortenUrl = false);

signals:
    void colorSchemeChanged();
    void fontFamilyHeadingChanged();
    void fontFamilyChanged();
    void fontSizeTinyChanged();
    void fontSizeExtraSmallChanged();
    void fontSizeSmallChanged();
    void fontSizeMediumChanged();
    void fontSizeLargeChanged();
    void fontSizeExtraLargeChanged();
    void fontSizeHugeChanged();
    void highlightColorChanged();
    void highlightBackgroundColorChanged();
    void highlightDimmerColorChanged();
    void primaryColorChanged();
    void secondaryColorChanged();
    void secondaryHighlightColorChanged();
    void backgroundGlowColorChanged();
    void errorColorChanged();
    void backgroundImageChanged();
    void homeBackgroundImageChanged();
    void patternImageChanged();
    void backgroundMaterialChanged();
    void wallpaperFilterChanged();
    void overlayBackgroundColorChanged();
    void wallpaperOverlayColorChanged();
    void coverOverlayColorChanged();

private:
    QColor coverOverlayColor() const;

    ThemePrivate *m_private;
};

}

#endif // SILICATHEME_H
