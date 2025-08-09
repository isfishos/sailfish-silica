// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERACTIONLIST_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERACTIONLIST_H

#include <QObject>
#include <QQmlParserStatus>
#include <QList>

class DeclarativeCoverAction;

class DeclarativeCoverActionList : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit DeclarativeCoverActionList(QObject *parent = nullptr);

    void classBegin() override {}
    void componentComplete() override;

private:
    void collectActions();
    void exposeToDBus();

    QList<DeclarativeCoverAction*> m_actions;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERACTIONLIST_H
