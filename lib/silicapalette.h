/****************************************************************************************
**
** Copyright (C) 2019 Jolla Ltd.
** Contact: Andrew den Exter <andrew.den.exter@jolla.com>
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

#ifndef SAILFISH_SILICA_PALETTE_H
#define SAILFISH_SILICA_PALETTE_H

#include <silicatheme.h>

QT_BEGIN_NAMESPACE
class QQuickItem;
QT_END_NAMESPACE

namespace Silica
{

class PalettePrivate;
class SAILFISH_SILICA_EXPORT Palette : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Palette)
    Q_DISABLE_COPY(Palette)
    Q_PROPERTY(Silica::Theme::ColorScheme colorScheme READ colorScheme WRITE setColorScheme RESET resetColorScheme NOTIFY colorSchemeChanged)
    Q_PROPERTY(QColor primaryColor READ primaryColor WRITE setPrimaryColor RESET resetPrimaryColor NOTIFY primaryColorChanged)
    Q_PROPERTY(QColor secondaryColor READ secondaryColor WRITE setSecondaryColor RESET resetSecondaryColor NOTIFY secondaryColorChanged)
    Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor RESET resetHighlightColor NOTIFY highlightColorChanged)
    Q_PROPERTY(QColor secondaryHighlightColor READ secondaryHighlightColor WRITE setSecondaryHighlightColor RESET resetSecondaryHighlightColor NOTIFY secondaryHighlightColorChanged)
    Q_PROPERTY(QColor highlightBackgroundColor READ highlightBackgroundColor WRITE setHighlightBackgroundColor RESET resetHighlightBackgroundColor NOTIFY highlightBackgroundColorChanged)
    Q_PROPERTY(QColor highlightDimmerColor READ highlightDimmerColor WRITE setHighlightDimmerColor RESET resetHighlightDimmerColor NOTIFY highlightDimmerColorChanged)
    Q_PROPERTY(QColor overlayBackgroundColor READ overlayBackgroundColor WRITE setOverlayBackgroundColor RESET resetOverlayBackgroundColor NOTIFY overlayBackgroundColorChanged)
    Q_PROPERTY(QColor backgroundGlowColor READ backgroundGlowColor WRITE setBackgroundGlowColor RESET resetBackgroundGlowColor NOTIFY backgroundGlowColorChanged)
    Q_PROPERTY(QColor errorColor READ errorColor WRITE setErrorColor RESET resetErrorColor NOTIFY errorColorChanged)
    Q_PROPERTY(QColor _wallpaperOverlayColor READ wallpaperOverlayColor NOTIFY wallpaperOverlayColorChanged)
    Q_PROPERTY(QColor _coverOverlayColor READ coverOverlayColor NOTIFY coverOverlayColorChanged)
public:
    explicit Palette(QObject *parent = nullptr);
    ~Palette();

    Theme::ColorScheme colorScheme() const;
    void setColorScheme(Theme::ColorScheme scheme);
    void resetColorScheme();

    QColor primaryColor() const;
    void setPrimaryColor(const QColor &color);
    void resetPrimaryColor();

    QColor secondaryColor() const;
    void setSecondaryColor(const QColor &color);
    void resetSecondaryColor() const;

    QColor highlightColor() const;
    void setHighlightColor(const QColor &color);
    void resetHighlightColor();

    QColor secondaryHighlightColor() const;
    void setSecondaryHighlightColor(const QColor &color);
    void resetSecondaryHighlightColor();

    QColor highlightBackgroundColor();
    void setHighlightBackgroundColor(const QColor &color);
    void resetHighlightBackgroundColor();

    QColor highlightDimmerColor();
    void setHighlightDimmerColor(const QColor &color);
    void resetHighlightDimmerColor();

    QColor overlayBackgroundColor() const;
    void setOverlayBackgroundColor(const QColor &color);
    void resetOverlayBackgroundColor();

    QColor backgroundGlowColor() const;
    void setBackgroundGlowColor(const QColor &color);
    void resetBackgroundGlowColor();

    QColor errorColor() const;
    void setErrorColor(const QColor &color);
    void resetErrorColor();

signals:
    void colorSchemeChanged();
    void primaryColorChanged();
    void secondaryColorChanged();
    void highlightColorChanged();
    void secondaryHighlightColorChanged();
    void highlightBackgroundColorChanged();
    void highlightDimmerColorChanged();
    void overlayBackgroundColorChanged();
    void backgroundGlowColorChanged();
    void errorColorChanged();
    void wallpaperOverlayColorChanged();
    void coverOverlayColorChanged();

private:
    QColor wallpaperOverlayColor() const;
    QColor coverOverlayColor() const;

    QScopedPointer<PalettePrivate> d_ptr;
};

}

#endif // SILICATHEME_P_H
