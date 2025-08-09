// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVETABVIEWACTION_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVETABVIEWACTION_H

#include <QObject>

class DeclarativeTabViewAction : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativeTabViewAction(QObject *parent = nullptr);

    enum OperationType {
        Animated,
        Immediate
    };
    Q_ENUM(OperationType)
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVETABVIEWACTION_H
