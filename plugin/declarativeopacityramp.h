// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEOPACITYRAMP_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEOPACITYRAMP_H

#include <QObject>

class DeclarativeOpacityRamp : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativeOpacityRamp(QObject *parent = nullptr);

    enum Direction {
        LeftToRight,
        RightToLeft,
        TopToBottom,
        BottomToTop,
        BothSides,
        BothEnds
    };
    Q_ENUM(Direction)
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEOPACITYRAMP_H
