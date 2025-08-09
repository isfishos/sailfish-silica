// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_QUICKSCROLLDIRECTION_H
#define SAILFISH_SILICA_PLUGIN_QUICKSCROLLDIRECTION_H

#include <QObject>

class QuickScrollDirection : public QObject
{
    Q_OBJECT

public:
    enum Direction {
        NoDirection = 0x00,
        Up = 0x01,
        Down = 0x02,
        Left = 0x04,
        Right = 0x08,
        Vertical = Up | Down,
        Horizontal = Left | Right,
        All = Vertical | Horizontal
    };
    Q_ENUM(Direction)
    Q_DECLARE_FLAGS(Directions, Direction)

    explicit QuickScrollDirection(QObject *parent = nullptr);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QuickScrollDirection::Directions)

#endif // SAILFISH_SILICA_PLUGIN_QUICKSCROLLDIRECTION_H
