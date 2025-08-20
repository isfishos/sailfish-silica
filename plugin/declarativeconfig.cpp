// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativeconfig.h"
#include <QQmlEngine>

DeclarativeConfigApi::DeclarativeConfigApi(QObject *parent)
    : QObject(parent)
{
}

void DeclarativeConfigApi::setCompositorWindows(const QList<QObject*> &windows)
{
    // TODO: Forward to compositor if available
    Q_UNUSED(windows)
}

// Singleton instance
static DeclarativeConfigApi *g_configInstance = nullptr;

DeclarativeConfigApi *DeclarativeConfigApi::instance()
{
    if (!g_configInstance) {
        g_configInstance = new DeclarativeConfigApi();
    }
    return g_configInstance;
}

bool DeclarativeConfigApi::wayland() const
{
    return true;
}

bool DeclarativeConfigApi::desktop() const
{
    return true;
}

bool DeclarativeConfigApi::layoutGrid() const
{
    return false;
}

double DeclarativeConfigApi::sailfishVersion() const
{
    return 4.6; // placeholder
}
