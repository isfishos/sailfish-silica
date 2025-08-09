// SPDX-License-Identifier: LGPL-2.1-only

#include "profilelistener.h"

ProfileListener::ProfileListener(QObject *parent)
    : QObject(parent)
{
    updateTouchToneVolume();
}

void ProfileListener::updateTouchToneVolume()
{
    // TODO: Implement system profile backend integration
    // For now, use default value
    int newVolume = 50;
    if (m_touchToneVolume != newVolume) {
        m_touchToneVolume = newVolume;
        emit touchToneVolumeChanged();
    }
}
