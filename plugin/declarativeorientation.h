// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEORIENTATION_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEORIENTATION_H

#include <QObject>
#include <Qt>

class DeclarativeOrientation : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativeOrientation(QObject *parent = nullptr);

    enum Orientation {
        Portrait = 1,
        Landscape = 2,
        PortraitInverted = 4,
        LandscapeInverted = 8
    };
    Q_ENUM(Orientation)
    Q_DECLARE_FLAGS(Orientations, Orientation)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DeclarativeOrientation::Orientations)

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEORIENTATION_H
