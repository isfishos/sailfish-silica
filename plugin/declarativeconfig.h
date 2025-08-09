// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVECONFIG_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVECONFIG_H

#include <QObject>
#include <qqml.h>

class DeclarativeConfigApi : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativeConfigApi(QObject *parent = nullptr);

    Q_INVOKABLE void setCompositorWindows(const QList<QObject*> &windows);

    static DeclarativeConfigApi *instance();
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVECONFIG_H
