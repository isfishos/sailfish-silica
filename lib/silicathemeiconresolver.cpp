#include "silicathemeiconresolver.h"
#include "silicathemeiconresolver_p.h"

#include <cmath>
#include <MGConfItem>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

namespace Silica {

IconInfo::IconInfo() : d_ptr(new IconInfoPrivate("", ColorIcon)) {}

IconInfo::IconInfo(const QString &filePath, IconType iconType)
    : d_ptr(new IconInfoPrivate(filePath, iconType)) {}

IconInfo::IconInfo(const IconInfo &other)
    : d_ptr(new IconInfoPrivate(*other.d_ptr)) {}

IconInfo::~IconInfo() {
    delete d_ptr;
}

IconInfo &IconInfo::operator=(const IconInfo &other) {
    if (this != &other) {
        IconInfoPrivate *newD = new IconInfoPrivate(*other.d_ptr);
        delete d_ptr;
        d_ptr = newD;
    }
    return *this;
}

QString IconInfo::filePath() const {
    return d_ptr->filePath;
}

IconInfo::IconType IconInfo::iconType() const {
    return d_ptr->iconType;
}

QString ThemeIconResolverPrivate::themeIconSubDir(qreal pixelRatio) {
    QString subdir;
    MGConfItem scaleConf("/desktop/sailfish/silica/theme_icon_subdir");
    QString confScaleDir = scaleConf.value().toString();

    if (!confScaleDir.isEmpty()) {
        subdir = confScaleDir;
        if (subdir.isEmpty()) {
            qWarning() << "Invalid icon subdir defined. Check your device configuration!";
        }
    }

    if (subdir.isEmpty()) {
        subdir = QString("z%1").arg(pixelRatio, 0, 'f', pixelRatio == floor(pixelRatio) ? 1 : 2);
        if (subdir.endsWith('0')) {
            subdir.chop(1);
        }
    }
    return subdir;
}

void ThemeIconResolverPrivate::addIconRoot(const QString &path) {
    QString root = path;
    if (!root.endsWith(QDir::separator())) {
        root.append(QDir::separator());
    }

    QDir dir(root);
    if (!dir.exists()) {
        qWarning() << "Image dir" << root << "does not exist";
        return;
    }

    QFileInfoList scaleDirList = dir.entryInfoList(QStringList() << QString("z*"), QDir::Dirs);
    QString detectedScaleDir;

    if (scaleDirList.length() > 0) {
        detectedScaleDir = scaleDirList.at(0).fileName();

    } else {
        qWarning() << "Could not find any zN.M subdirs in" << root;
    }

    QString scaleDir = themeIconSubDir(m_pixelRatio);

    if (!scaleDir.isEmpty() && scaleDir != detectedScaleDir) {
        QString p1 = root + scaleDir + "/icons-monochrome/";
        QString p2 = root + scaleDir + "/icons/";
        imageDirNodes.append(ImageDirNode(p1, QStringList() << ".png" << ".jpg", IconInfo::MonochromeIcon));
        imageDirNodes.append(ImageDirNode(p2, QStringList() << ".png" << ".jpg", IconInfo::ColorIcon));

    }

    if (!detectedScaleDir.isEmpty()) {
        QString p1 = root + detectedScaleDir + "/icons-monochrome/";
        QString p2 = root + detectedScaleDir + "/icons/";
        imageDirNodes.append(ImageDirNode(p1, QStringList() << ".png" << ".jpg", IconInfo::MonochromeIcon));
        imageDirNodes.append(ImageDirNode(p2, QStringList() << ".png" << ".jpg", IconInfo::ColorIcon));

    }

    imageDirNodes.append(ImageDirNode(root + "/icons-monochrome/",
                                   QStringList() << ".png" << ".jpg", IconInfo::MonochromeIcon));
    imageDirNodes.append(ImageDirNode(root + "/icons/",
                                   QStringList() << ".png" << ".jpg", IconInfo::ColorIcon));

}

void ThemeIconResolverPrivate::loadDefaultThemeRoots()
{
    // Determine theme root directory
    QString themeRoot;
    QByteArray env = qgetenv("M_THEME_DIR");
    if (!env.isEmpty()) {
        themeRoot = QString::fromLocal8Bit(env);
    } else {
        themeRoot = QStringLiteral("/usr/share/themes");
    }

    // Read configured theme name(s)
    QList<QString> themeNames;
    MGConfItem themeNameConf("/meegotouch/theme/name");
    QString themeName = themeNameConf.value().toString();
    if (!themeName.isEmpty()) themeNames.append(themeName);

    if (!themeNames.contains(QStringLiteral("sailfish-default")))
        themeNames.append(QStringLiteral("sailfish-default"));

    for (const QString &tn : themeNames) {
        QString root = themeRoot + '/' + tn + '/' + QStringLiteral("silica");
        addIconRoot(root);
    }

    // Add a safe hicolor fallback used upstream
    imageDirNodes.append(ImageDirNode(QStringLiteral("/usr/share/icons/hicolor/86x86/apps/"), QStringList() << ".png", IconInfo::ColorIcon));
}

ThemeIconResolver::ThemeIconResolver()
    : d_ptr(new ThemeIconResolverPrivate)
{
    d_ptr->m_pixelRatio = Theme::instance() ? Theme::instance()->pixelRatio() : 1.0;
    d_ptr->loadDefaultTheme = true;
    if (d_ptr->loadDefaultTheme) d_ptr->loadDefaultThemeRoots();
}

ThemeIconResolver::ThemeIconResolver(InitFlags initFlags)
    : d_ptr(new ThemeIconResolverPrivate)
{
    d_ptr->m_pixelRatio = Theme::instance() ? Theme::instance()->pixelRatio() : 1.0;
    d_ptr->loadDefaultTheme = (initFlags & LoadDefaultTheme);
    if (d_ptr->loadDefaultTheme) d_ptr->loadDefaultThemeRoots();
}

ThemeIconResolver::ThemeIconResolver(qreal pixelRatio, InitFlags initFlags)
    : d_ptr(new ThemeIconResolverPrivate)
{
    d_ptr->m_pixelRatio = pixelRatio;
    d_ptr->loadDefaultTheme = (initFlags & LoadDefaultTheme);
    if (d_ptr->loadDefaultTheme) d_ptr->loadDefaultThemeRoots();
}

ThemeIconResolver::~ThemeIconResolver() {
    delete d_ptr;
}

void ThemeIconResolver::addIconRoot(const QString &path) {
    d_ptr->addIconRoot(path);
}

IconInfo ThemeIconResolver::resolveIcon(const QString &id) const {
    return resolveIcon(id, Theme::LightOnDark);
}

IconInfo ThemeIconResolver::resolveIcon(const QString &id, Theme::ColorScheme colorScheme) const {
    if (id.isEmpty()) {
        return IconInfo();
    }

    Q_D(const ThemeIconResolver);

    QString resolvedPath;
    IconInfo::IconType resolvedType = IconInfo::MonochromeIcon;

    const QString colorStr = (colorScheme == Theme::DarkOnLight) ? QStringLiteral("light") : QStringLiteral("dark");

    // Use a cache keyed by id|color
    const QString cacheKey = id + QChar('\x1f') + colorStr; // unit separator as simple delimiter
    if (d->cache.contains(cacheKey)) {
        return d->cache.value(cacheKey);
    }

    // Check icon nodes
    const QList<ImageDirNode> nodes = d->getImageDirNodes();
    for (const ImageDirNode &node : nodes) {
        // Try each suffix for this node
        for (const QString &suffix : node.suffixList) {
            QString candidate = node.path + id + suffix;
            if (QFileInfo::exists(candidate)) {
                resolvedPath = candidate;
                resolvedType = node.iconType;
                break;
            }

            // Try color-suffixed variant (icon-light.png / icon-dark.png)
            QString colorCandidate = node.path + id + QStringLiteral("-") + colorStr + suffix;
            if (QFileInfo::exists(colorCandidate)) {
                resolvedPath = colorCandidate;
                resolvedType = node.iconType;
                break;
            }
        }
        if (!resolvedPath.isEmpty()) break;
    }

    if (!resolvedPath.isEmpty()) {
        IconInfo info(resolvedPath, resolvedType);
        d->cache.insert(cacheKey, info);
        return info;
    }

    return IconInfo(resolvedPath, resolvedType);
}

QString ThemeIconResolver::resolvePath(const QString &id) const {
    return resolvePath(id, Theme::LightOnDark);
}

QString ThemeIconResolver::resolvePath(const QString &id, Theme::ColorScheme colorScheme) const {
    return resolveIcon(id, colorScheme).filePath();
}

void ThemeIconResolver::setTestModeDirectories(const QString &testPath, const QString &testSubDir) {
    qWarning() << "ThemeIconResolver::setTestModeDirectories is deprecated and a no-op";
}

} // namespace Silica