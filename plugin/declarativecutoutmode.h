// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVECUTOUTMODE_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVECUTOUTMODE_H

#include <QObject>

class DeclarativeCutoutMode : public QObject
{
    Q_OBJECT

public:
    enum CutoutMode {
        FullScreen = 0,
        AvoidLandscapeCutout = 1
    };
    Q_ENUM(CutoutMode)

    explicit DeclarativeCutoutMode(QObject *parent = nullptr);
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVECUTOUTMODE_H
