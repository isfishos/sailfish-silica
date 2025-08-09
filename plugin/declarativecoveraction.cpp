// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativecoveraction.h"

DeclarativeCoverAction::DeclarativeCoverAction(QObject *parent)
    : QObject(parent)
{
}

void DeclarativeCoverAction::setIconSource(const QUrl &source)
{
    if (m_iconSource != source) {
        m_iconSource = source;
        emit iconSourceChanged();
    }
}

void DeclarativeCoverAction::trigger()
{
    emit triggered();
}
