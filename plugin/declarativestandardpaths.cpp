// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativestandardpaths.h"
#include <QStandardPaths>
#include <QQmlEngine>
#include <QDir>
#include <QCoreApplication>

DeclarativeStandardPaths *DeclarativeStandardPaths::g_instance = nullptr;

DeclarativeStandardPaths::DeclarativeStandardPaths(QObject *parent)
    : QObject(parent)
{
    initializePaths();
}

QString DeclarativeStandardPaths::cache() const
{
    return m_cache;
}

QString DeclarativeStandardPaths::data() const
{
    return m_data;
}

QString DeclarativeStandardPaths::documents() const
{
    return m_documents;
}

QString DeclarativeStandardPaths::download() const
{
    return m_download;
}

QString DeclarativeStandardPaths::genericData() const
{
    return m_genericData;
}

QString DeclarativeStandardPaths::music() const
{
    return m_music;
}

QString DeclarativeStandardPaths::pictures() const
{
    return m_pictures;
}

QString DeclarativeStandardPaths::videos() const
{
    return m_videos;
}

QString DeclarativeStandardPaths::home() const
{
    return m_home;
}

QString DeclarativeStandardPaths::temporary() const
{
    return m_temporary;
}

QString DeclarativeStandardPaths::qmlImportPath() const
{
    return m_qmlImportPath;
}

QString DeclarativeStandardPaths::resolveImport(const QString &page)
{
    if (page.isEmpty()) {
        return QString();
    }

    // Check if we've already visited this page to prevent import cycles
    if (m_visitedPages.contains(page)) {
        return QString();
    }

    m_visitedPages.insert(page);

    // TODO: Implement proper import path resolution using QQmlEngine
    // For now, return a simple path
    return page;
}

DeclarativeStandardPaths *DeclarativeStandardPaths::instance()
{
    if (!g_instance) {
        g_instance = new DeclarativeStandardPaths();
    }
    return g_instance;
}

void DeclarativeStandardPaths::initializePaths()
{
    m_cache = normalizePath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    m_data = normalizePath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    m_documents = normalizePath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    m_download = normalizePath(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    m_genericData = normalizePath(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
    m_music = normalizePath(QStandardPaths::writableLocation(QStandardPaths::MusicLocation));
    m_pictures = normalizePath(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    m_videos = normalizePath(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
    m_home = normalizePath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    m_temporary = normalizePath(QStandardPaths::writableLocation(QStandardPaths::TempLocation));

    // Build qmlImportPath from QQmlEngine import paths
    QStringList importPaths;
    importPaths << QCoreApplication::applicationDirPath();
    m_qmlImportPath = importPaths.join(":");
}

QString DeclarativeStandardPaths::normalizePath(const QString &path)
{
    QString normalized = QDir::cleanPath(path);
    if (!normalized.endsWith('/')) {
        normalized += '/';
    }
    return normalized;
}
