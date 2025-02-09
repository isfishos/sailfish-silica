/****************************************************************************************
**
** Copyright (C) 2015 Jolla Ltd.
** Contact: Matt Vogt <matthew.vogt@jollamobile.com>
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

#ifndef SILICASCREEN_H
#define SILICASCREEN_H

#include <silicaglobal.h>
#include <QObject>
#include <QRect>

namespace Silica
{

class ScreenPrivate;
class RoundedCornerPrivate;

class SAILFISH_SILICA_EXPORT RoundedCorner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x NOTIFY positionChanged)
    Q_PROPERTY(int y READ y NOTIFY positionChanged)
    Q_PROPERTY(int radius READ radius NOTIFY radiusChanged)

public:
    RoundedCorner(QObject *parent = nullptr);
    RoundedCorner(int x, int y, int radius, QObject *parent = nullptr);
    ~RoundedCorner();

    int x() const;
    int y() const;
    void setPosition(int x, int y);

    int radius() const;
    void setRadius(int radius);

signals:
    void positionChanged();
    void radiusChanged();

private:
    RoundedCornerPrivate *d_ptr;
};

class SAILFISH_SILICA_EXPORT Screen : public QObject
{
    Q_OBJECT
    Q_ENUMS(SizeCategory)
    Q_PROPERTY(int width READ width FINAL CONSTANT)
    Q_PROPERTY(int height READ height FINAL CONSTANT)
    Q_PROPERTY(qreal widthRatio READ widthRatio FINAL CONSTANT)
    Q_PROPERTY(SizeCategory sizeCategory READ sizeCategory FINAL CONSTANT)
    Q_PROPERTY(bool hasCutouts READ hasCutouts NOTIFY cutoutsChanged FINAL)
    Q_PROPERTY(QRect topCutout READ topCutout NOTIFY cutoutsChanged FINAL)
    Q_PROPERTY(RoundedCorner* topLeftCorner READ topLeftCorner CONSTANT)
    Q_PROPERTY(RoundedCorner* topRightCorner READ topRightCorner CONSTANT)
    Q_PROPERTY(RoundedCorner* bottomLeftCorner READ bottomLeftCorner CONSTANT)
    Q_PROPERTY(RoundedCorner* bottomRightCorner READ bottomRightCorner CONSTANT)

public:
    enum SizeCategory {
        Small,
        Medium,
        Large,
        ExtraLarge
    };

    // For C++
    static Screen *instance();

    // For QML plugin
    explicit Screen(QObject *parent = 0);
    ~Screen();

    int width() const;
    int height() const;
    qreal widthRatio() const;
    SizeCategory sizeCategory() const;

    bool hasCutouts() const;
    QRect topCutout() const;

    RoundedCorner* topLeftCorner();
    RoundedCorner* topRightCorner();
    RoundedCorner* bottomLeftCorner();
    RoundedCorner* bottomRightCorner();

signals:
    void cutoutsChanged();

private:
    ScreenPrivate *m_private;
};

}

#endif // SILICASCREEN_H
