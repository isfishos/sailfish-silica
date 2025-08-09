// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_PROFILELISTENER_H
#define SAILFISH_SILICA_PLUGIN_PROFILELISTENER_H

#include <QObject>

class ProfileListener : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int touchToneVolume READ touchToneVolume NOTIFY touchToneVolumeChanged)

public:
    explicit ProfileListener(QObject *parent = nullptr);

    int touchToneVolume() const { return m_touchToneVolume; }

Q_SIGNALS:
    void touchToneVolumeChanged();

private:
    void updateTouchToneVolume();

    int m_touchToneVolume = 50; // Default value
};

#endif // SAILFISH_SILICA_PLUGIN_PROFILELISTENER_H
