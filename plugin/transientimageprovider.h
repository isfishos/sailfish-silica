// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_TRANSIENTIMAGEPROVIDER_H
#define SAILFISH_SILICA_PLUGIN_TRANSIENTIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QSize>

class TransientImageProvider : public QQuickImageProvider
{
public:
    explicit TransientImageProvider();
    ~TransientImageProvider() override;

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    QQuickTextureFactory *requestTexture(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    QImage generateImage(const QString &id, const QSize &requestedSize);
};

#endif // SAILFISH_SILICA_PLUGIN_TRANSIENTIMAGEPROVIDER_H
