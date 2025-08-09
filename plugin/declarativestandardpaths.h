// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVESTANDARDPATHS_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVESTANDARDPATHS_H

#include <QObject>
#include <QString>
#include <QSet>

class QQmlEngine;

class DeclarativeStandardPaths : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString cache READ cache CONSTANT)
    Q_PROPERTY(QString data READ data CONSTANT)
    Q_PROPERTY(QString documents READ documents CONSTANT)
    Q_PROPERTY(QString download READ download CONSTANT)
    Q_PROPERTY(QString genericData READ genericData CONSTANT)
    Q_PROPERTY(QString music READ music CONSTANT)
    Q_PROPERTY(QString pictures READ pictures CONSTANT)
    Q_PROPERTY(QString videos READ videos CONSTANT)
    Q_PROPERTY(QString home READ home CONSTANT)
    Q_PROPERTY(QString temporary READ temporary CONSTANT)
    Q_PROPERTY(QString qmlImportPath READ qmlImportPath CONSTANT)

public:
    explicit DeclarativeStandardPaths(QObject *parent = nullptr);

    QString cache() const;
    QString data() const;
    QString documents() const;
    QString download() const;
    QString genericData() const;
    QString music() const;
    QString pictures() const;
    QString videos() const;
    QString home() const;
    QString temporary() const;
    QString qmlImportPath() const;

    Q_INVOKABLE QString resolveImport(const QString &page);

    static DeclarativeStandardPaths *instance();

private:
    void initializePaths();
    QString normalizePath(const QString &path);

    QString m_cache;
    QString m_data;
    QString m_documents;
    QString m_download;
    QString m_genericData;
    QString m_music;
    QString m_pictures;
    QString m_videos;
    QString m_home;
    QString m_temporary;
    QString m_qmlImportPath;
    QSet<QString> m_visitedPages;
    static DeclarativeStandardPaths *g_instance;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVESTANDARDPATHS_H
