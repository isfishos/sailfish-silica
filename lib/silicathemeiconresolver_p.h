#ifndef SILICA_THEMEICONRESOLVER_P_H
#define SILICA_THEMEICONRESOLVER_P_H

#include <silicathemeiconresolver.h>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QMap>

namespace Silica {

struct ImageDirNode {
    QString path;
    QStringList suffixList;
    IconInfo::IconType iconType;

    ImageDirNode(const QString &path, const QStringList &suffixes, IconInfo::IconType type)
        : path(path), suffixList(suffixes), iconType(type) {}
};

class IconInfoPrivate {
public:
    QString filePath;
    IconInfo::IconType iconType;
   
    IconInfoPrivate(const QString &filePath, IconInfo::IconType iconType)
        : filePath(filePath), iconType(iconType) {}
    
    IconInfoPrivate(const IconInfoPrivate &other)
        : filePath(other.filePath), iconType(other.iconType) {}
};

class ThemeIconResolverPrivate {
public:
    qreal m_pixelRatio;
    QStringList iconRoots;
    QHash<QString, IconInfo> cache;
    bool loadDefaultTheme;

    void addIconRoot(const QString &path);

private:
    static QString themeIconSubDir(qreal pixelRatio);
    QList<ImageDirNode> imageDirNodes;
};

} // namespace Silica

#endif