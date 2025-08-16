#include "squareimageprovider.h"
#include <QImageReader>
#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

namespace Sailfish {
namespace Silica {
namespace Background {

SquareImageProvider::SquareImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Texture)
    , m_useHybrisBuffers(false)
    , m_eglDisplay(nullptr)
{
    // TODO: Add EGL/libhybris support
}

SquareImageProvider::~SquareImageProvider()
{
    // Clean up any EGL resources if needed
}

QQuickTextureFactory *SquareImageProvider::requestTexture(const QString &id, QSize *size, const QSize &requestedSize)
{
    QSize finalSize;
    QImage image = processImage(id, requestedSize, &finalSize);

    if (image.isNull()) {
        return nullptr;
    }

    if (size) {
        *size = finalSize;
    }

    return QQuickTextureFactory::textureFactoryForImage(image);
}

QImage SquareImageProvider::processImage(const QString &id, const QSize &requestedSize, QSize *size)
{
    // Decode the percent-encoded URL
    QString decodedId = QUrl::fromPercentEncoding(id.toUtf8());
    QUrl url(decodedId);

    // Only support local files
    if (!url.isLocalFile()) {
        return QImage();
    }

    QString filePath = url.toLocalFile();
    if (!QFileInfo::exists(filePath)) {
        return QImage();
    }

    QImageReader reader(filePath);
    reader.setAutoTransform(true);

    // Get source size
    QSize sourceSize = reader.size();
    if (!sourceSize.isValid()) {
        return QImage();
    }

    // Calculate minimum dimension
    int minimumSourceDimension = qMin(sourceSize.width(), sourceSize.height());
    int minimumDimension = minimumSourceDimension;

    if (requestedSize.width() > 0) {
        minimumDimension = qMin(minimumDimension, requestedSize.width());
    }
    if (requestedSize.height() > 0) {
        minimumDimension = qMin(minimumDimension, requestedSize.height());
    }

    // Set clip rect for centered square
    int offsetX = (sourceSize.width() - minimumSourceDimension) / 2;
    int offsetY = (sourceSize.height() - minimumSourceDimension) / 2;
    reader.setClipRect(QRect(offsetX, offsetY, minimumSourceDimension, minimumSourceDimension));

    // Set scaled size
    reader.setScaledSize(QSize(minimumDimension, minimumDimension));

    // Read the image
    QImage image = reader.read();
    if (image.isNull()) {
        return QImage();
    }

    // Ensure the image is square
    image = createSquareImage(image, minimumDimension);

    if (size) {
        *size = QSize(minimumDimension, minimumDimension);
    }

    return image;
}

QImage SquareImageProvider::createSquareImage(const QImage &source, int targetSize)
{
    if (source.width() == targetSize && source.height() == targetSize) {
        return source;
    }

    // Scale the image to the target size
    return source.scaled(targetSize, targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

} // namespace Background
} // namespace Silica
} // namespace Sailfish
