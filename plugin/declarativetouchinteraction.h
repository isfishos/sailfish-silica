// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVETOUCHINTERACTION_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVETOUCHINTERACTION_H

#include <QObject>

class DeclarativeTouchInteraction : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativeTouchInteraction(QObject *parent = nullptr);

    enum Direction {
        Left,
        Up,
        Right,
        Down
    };
    Q_ENUM(Direction)

    enum Mode {
        Swipe,
        EdgeSwipe,
        Pull
    };
    Q_ENUM(Mode)
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVETOUCHINTERACTION_H
