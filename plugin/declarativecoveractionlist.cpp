// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativecoveractionlist.h"
#include "declarativecoveraction.h"
#include <QQmlEngine>
#include <QQmlContext>

DeclarativeCoverActionList::DeclarativeCoverActionList(QObject *parent)
    : QObject(parent)
{
}

void DeclarativeCoverActionList::componentComplete()
{
    collectActions();
    exposeToDBus();
}

void DeclarativeCoverActionList::collectActions()
{
    // Find all DeclarativeCoverAction children
    m_actions.clear();
    const QList<QObject*> children = this->children();
    for (QObject *child : children) {
        if (DeclarativeCoverAction *action = qobject_cast<DeclarativeCoverAction*>(child)) {
            m_actions.append(action);
        }
    }
}

void DeclarativeCoverActionList::exposeToDBus()
{
    // TODO: Expose actions to DBus/system as required by platform
    // For now, just collect the actions
    Q_UNUSED(m_actions)
}
