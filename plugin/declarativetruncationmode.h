// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVETRUNCATIONMODE_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVETRUNCATIONMODE_H

#include <QObject>

class DeclarativeTruncationMode : public QObject
{
    Q_OBJECT

public:
    enum Mode {
        None = 0,
        Elide = 1,
        Fade = 2
    };
    Q_ENUM(Mode)

    explicit DeclarativeTruncationMode(QObject *parent = nullptr);
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVETRUNCATIONMODE_H
