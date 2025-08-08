// SPDX-License-Identifier: LGPL-2.1-only

#include "silicaimageprovider.h"
#include "silicaimageprovider_p.h"
#include "silicatheme.h"
#include "silicathemeiconresolver.h"

#include <QImageReader>
#include <QQuickTextureFactory>
#include <QUrlQuery>

using namespace Silica;

namespace {
static bool parseMonochromeId(const QString &id)
{
    // Heuristic: monochrome icons in Silica often use prefix "icon-m-"
    return id.startsWith(QLatin1String("icon-m-"))
        || id.contains(QLatin1String("/icons-monochrome/"));
}

static QImage colorizeMonochrome(QImage img, const QColor &color)
{
    if (img.format() != QImage::Format_ARGB32_Premultiplied) {
        img = img.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }
    const uchar r = color.red();
    const uchar g = color.green();
    const uchar b = color.blue();
    for (int y = 0; y < img.height(); ++y) {
        QRgb *scan = reinterpret_cast<QRgb *>(img.scanLine(y));
        for (int x = 0; x < img.width(); ++x) {
            const int a = qAlpha(scan[x]);
            scan[x] = qRgba(r, g, b, a);
        }
    }
    return img;
}
}

ImageProvider::ImageProvider(InitFlags initFlags)
    : QQuickImageProvider(QQuickImageProvider::Texture)
    , d_ptr(new Silica::ImageProviderPrivate)
{
    Q_UNUSED(initFlags)
}

ImageProvider::~ImageProvider() = default;

void ImageProvider::addIconRoot(const QString &path)
{
    d_ptr->iconResolver.addIconRoot(path);
}

QQuickTextureFactory *ImageProvider::requestTexture(const QString &id, QSize *size, const QSize &requestedSize)
{
    // Parse parameters like "id?color=#RRGGBB"
    QString iconId = id;
    QColor overrideColor;
    int queryPos = id.indexOf('?');
    if (queryPos > 0) {
        const QString base = id.left(queryPos);
        const QUrl url(QString::fromLatin1("image://theme/") + id);
        QUrlQuery q(url);
        const QString colorStr = q.queryItemValue(QStringLiteral("color"));
        if (!colorStr.isEmpty()) {
            overrideColor = QColor(colorStr);
        }
        iconId = base;
    }

    const QString cacheKey = iconId + QLatin1Char('|')
                           + QString::number(requestedSize.width())
                           + QLatin1Char('x')
                           + QString::number(requestedSize.height())
                           + QLatin1Char('|')
                           + (overrideColor.isValid() ? overrideColor.name() : QString());

    if (d_ptr->cache.contains(cacheKey)) {
        const QImage &img = d_ptr->cache[cacheKey];
        if (size) *size = img.size();
        return QQuickTextureFactory::textureFactoryForImage(img);
    }

    const IconInfo info = d_ptr->iconResolver.resolveIcon(iconId, Theme::instance()->colorScheme());
    if (info.filePath().isEmpty()) {
        // Return a 1x1 transparent
        QImage img(1, 1, QImage::Format_ARGB32_Premultiplied);
        img.fill(Qt::transparent);
        if (size) *size = img.size();
        return QQuickTextureFactory::textureFactoryForImage(img);
    }

    QImageReader reader(info.filePath());
    QImage img = reader.read();
    if (img.isNull()) {
        QImage fallback(1, 1, QImage::Format_ARGB32_Premultiplied);
        fallback.fill(Qt::transparent);
        if (size) *size = fallback.size();
        return QQuickTextureFactory::textureFactoryForImage(fallback);
    }

    const bool monochrome = (info.iconType() == IconInfo::MonochromeIcon) || parseMonochromeId(iconId);
    if (monochrome) {
        QColor color = overrideColor.isValid() ? overrideColor : Theme::instance()->primaryColor();
        img = colorizeMonochrome(img, color);
    }

    if (requestedSize.isValid()) {
        img = img.scaled(requestedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    d_ptr->cache.insert(cacheKey, img);
    if (size) *size = img.size();
    return QQuickTextureFactory::textureFactoryForImage(img);
}


