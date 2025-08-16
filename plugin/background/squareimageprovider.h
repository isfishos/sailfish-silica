#ifndef SQUAREIMAGEPROVIDER_H
#define SQUAREIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QQuickTextureFactory>

namespace Sailfish {
namespace Silica {
namespace Background {

class SquareImageProvider : public QQuickImageProvider
{
public:
    SquareImageProvider();
    ~SquareImageProvider();

    QQuickTextureFactory *requestTexture(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    bool m_useHybrisBuffers;
    void *m_eglDisplay;

    // Helper functions
    QImage processImage(const QString &id, const QSize &requestedSize, QSize *size);
    QImage createSquareImage(const QImage &source, int targetSize);
};

} // namespace Background
} // namespace Silica
} // namespace Sailfish

#endif // SQUAREIMAGEPROVIDER_H
