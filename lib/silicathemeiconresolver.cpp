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
    }

    QFileInfoList scaleDirList = dir.entryInfoList(QStringList() << QString("z*"), QDir::Dirs);
    QString detectedScaleDir;

    if (scaleDirList.length() > 0) {
        detectedScaleDir = scaleDirList.at(0).fileName();
    } else {
        qWarning() << "Could not find any zN.M subdirs!";
    }

    QString scaleDir = themeIconSubDir(m_pixelRatio);

    if (!scaleDir.isEmpty() && scaleDir != detectedScaleDir) {
        imageDirNodes.append(ImageDirNode(root + scaleDir + "/icons-monochrome/",
                                      QStringList() << ".png" << ".jpg", IconInfo::MonochromeIcon));
        imageDirNodes.append(ImageDirNode(root + scaleDir + "/icons/",
                                      QStringList() << ".png" << ".jpg", IconInfo::ColorIcon));
    }

    if (!detectedScaleDir.isEmpty()) {
        imageDirNodes.append(ImageDirNode(root + detectedScaleDir + "/icons-monochrome/",
                                      QStringList() << ".png" << ".jpg", IconInfo::MonochromeIcon));
        imageDirNodes.append(ImageDirNode(root + detectedScaleDir + "/icons/",
                                      QStringList() << ".png" << ".jpg", IconInfo::ColorIcon));
    }

    imageDirNodes.append(ImageDirNode(root + "/icons-monochrome/",
                                   QStringList() << ".png" << ".jpg", IconInfo::MonochromeIcon));
    imageDirNodes.append(ImageDirNode(root + "/icons/",
                                   QStringList() << ".png" << ".jpg", IconInfo::ColorIcon));
}

ThemeIconResolver::ThemeIconResolver()
    : d_ptr(new ThemeIconResolverPrivate)
{
    d_ptr->m_pixelRatio = 1.0;
    d_ptr->loadDefaultTheme = true;
}

ThemeIconResolver::ThemeIconResolver(InitFlags initFlags)
    : d_ptr(new ThemeIconResolverPrivate)
{
    d_ptr->m_pixelRatio = 1.0;
    d_ptr->loadDefaultTheme = (initFlags & LoadDefaultTheme);
}

ThemeIconResolver::ThemeIconResolver(qreal pixelRatio, InitFlags initFlags)
    : d_ptr(new ThemeIconResolverPrivate)
{
    d_ptr->m_pixelRatio = pixelRatio;
    d_ptr->loadDefaultTheme = (initFlags & LoadDefaultTheme);
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

    // Check icon roots
    for (const QString &root : d->iconRoots) {
        // Try monochrome icons first
        QString iconPath = root + QLatin1String("/icons-monochrome/") + id;
        if (QFileInfo::exists(iconPath)) {
            resolvedPath = iconPath;
            resolvedType = IconInfo::MonochromeIcon;
            break;
        }

        // Then try color icons
        iconPath = root + QLatin1String("/icons/") + id;
        if (QFileInfo::exists(iconPath)) {
            resolvedPath = iconPath;
            resolvedType = IconInfo::ColorIcon;
            break;
        }

        // Check pixel ratio subdirectories if needed
        if (d->m_pixelRatio > 1.0) {
            QString ratioDir = QString("z%1").arg(d->m_pixelRatio, 0, 'f', d->m_pixelRatio == floor(d->m_pixelRatio) ? 1 : 2);

            iconPath = root + QLatin1Char('/') + ratioDir + QLatin1String("/icons-monochrome/") + id;
            if (QFileInfo::exists(iconPath)) {
                resolvedPath = iconPath;
                resolvedType = IconInfo::MonochromeIcon;
                break;
            }

            iconPath = root + QLatin1Char('/') + ratioDir + QLatin1String("/icons/") + id;
            if (QFileInfo::exists(iconPath)) {
                resolvedPath = iconPath;
                resolvedType = IconInfo::ColorIcon;
                break;
            }
        }
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