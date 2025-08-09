// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVETEXTEDITOR_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVETEXTEDITOR_H

#include <QObject>

class DeclarativeTextEditor : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativeTextEditor(QObject *parent = nullptr);

    enum BackgroundStyle {
        UnderlineBackground,
        FilledBackground,
        NoBackground
    };
    Q_ENUM(BackgroundStyle)
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVETEXTEDITOR_H
