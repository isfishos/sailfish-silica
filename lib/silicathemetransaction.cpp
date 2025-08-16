// SPDX-License-Identifier: LGPL-2.1-only

#include "silicathemetransaction.h"

namespace Silica
{

ThemeTransaction::ThemeTransaction(QObject *parent)
    : QObject(parent)
{
}

bool ThemeTransaction::deferAmbience() const
{
    return m_deferAmbience;
}

void ThemeTransaction::setDeferAmbience(bool defer)
{
    if (m_deferAmbience != defer) {
        m_deferAmbience = defer;
        emit deferAmbienceChanged();
    }
}

} // namespace Silica
