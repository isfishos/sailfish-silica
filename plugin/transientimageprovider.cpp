// SPDX-License-Identifier: LGPL-2.1-only

#include "transientimageprovider.h"
#include <QQuickTextureFactory>
#include <QPainter>
#include <QColor>
#include <QQuickWindow>

class TransientImageTextureFactory : public QQuickTextureFactory
{
public:
    explicit TransientImageTextureFactory(const QImage &image)
        : m_image(image)
    {
    }

    QSGTexture *createTexture(QQuickWindow *window) const override
    {
        return window->createTextureFromImage(m_image);
    }

    QSize textureSize() const override
    {
        return m_image.size();
    }

    int textureByteCount() const override
    {
        return m_image.sizeInBytes();
    }

    QImage image() const override
    {
        return m_image;
    }

private:
    QImage m_image;
};

TransientImageProvider::TransientImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Texture)
{
}

TransientImageProvider::~TransientImageProvider()
{
}

QPixmap TransientImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage image = generateImage(id, requestedSize);
    if (size) {
        *size = image.size();
    }
    return QPixmap::fromImage(image);
}

QImage TransientImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage image = generateImage(id, requestedSize);
    if (size) {
        *size = image.size();
    }
    return image;
}

QQuickTextureFactory *TransientImageProvider::requestTexture(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage image = generateImage(id, requestedSize);
    if (size) {
        *size = image.size();
    }
    return new TransientImageTextureFactory(image);
}

QImage TransientImageProvider::generateImage(const QString &id, const QSize &requestedSize)
{
    // Parse the ID to determine what kind of image to generate
    // Format: "type:params" (e.g., "color:red", "gradient:blue:green", "solid:100x200")

    QStringList parts = id.split(':');
    if (parts.isEmpty()) {
        return QImage();
    }

    QString type = parts.first();
    QSize size = requestedSize.isValid() ? requestedSize : QSize(100, 100);

    if (type == "color" && parts.size() > 1) {
        QColor color(parts[1]);
        QImage image(size, QImage::Format_ARGB32);
        image.fill(color);
        return image;
    } else if (type == "gradient" && parts.size() > 2) {
        QColor color1(parts[1]);
        QColor color2(parts[2]);

        QImage image(size, QImage::Format_ARGB32);
        QPainter painter(&image);

        QLinearGradient gradient(0, 0, size.width(), size.height());
        gradient.setColorAt(0, color1);
        gradient.setColorAt(1, color2);

        painter.fillRect(image.rect(), gradient);
        return image;
    } else if (type == "solid") {
        QImage image(size, QImage::Format_ARGB32);
        image.fill(Qt::white);
        return image;
    }

    // Default: return a simple colored rectangle
    QImage image(size, QImage::Format_ARGB32);
    image.fill(Qt::lightGray);
    return image;
}
