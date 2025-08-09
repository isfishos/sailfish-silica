// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGENAVIGATION_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGENAVIGATION_H

#include <QObject>

class DeclarativePageNavigation : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativePageNavigation(QObject *parent = nullptr);

    enum Navigation {
        NoNavigation,
        Back,
        Forward
    };
    Q_ENUM(Navigation)

    enum Direction {
        NoDirection,
        Up,
        Down,
        Left,
        Right
    };
    Q_ENUM(Direction)

    enum Style {
        Horizontal,
        Vertical
    };
    Q_ENUM(Style)
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGENAVIGATION_H
