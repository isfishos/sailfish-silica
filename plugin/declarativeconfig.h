// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVECONFIG_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVECONFIG_H

#include <QObject>
#include <qqml.h>

class DeclarativeConfigApi : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool wayland READ wayland CONSTANT)
    Q_PROPERTY(bool desktop READ desktop CONSTANT)
    Q_PROPERTY(bool layoutGrid READ layoutGrid CONSTANT)
    Q_PROPERTY(double sailfishVersion READ sailfishVersion CONSTANT)
public:
    explicit DeclarativeConfigApi(QObject *parent = nullptr);

    Q_INVOKABLE void setCompositorWindows(const QList<QObject*> &windows);

    static DeclarativeConfigApi *instance();

    bool wayland() const;
    bool desktop() const;
    bool layoutGrid() const;
    double sailfishVersion() const;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVECONFIG_H
