// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTACKACTION_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTACKACTION_H

#include <QObject>

class DeclarativePageStackAction : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativePageStackAction(QObject *parent = nullptr);

    enum Action {
        Push,
        Replace,
        Pop
    };
    Q_ENUM(Action)

    enum OperationType {
        Animated,
        Immediate
    };
    Q_ENUM(OperationType)
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTACKACTION_H
