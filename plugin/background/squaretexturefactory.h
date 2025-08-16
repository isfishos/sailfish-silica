#ifndef SQUARETEXTUREFACTORY_H
#define SQUARETEXTUREFACTORY_H

#include <QQuickTextureFactory>
#include <QSGTexture>

namespace Sailfish {
namespace Silica {
namespace Background {

class SquareTextureFactory : public QQuickTextureFactory
{
public:
    SquareTextureFactory();
    ~SquareTextureFactory();

    QSGTexture *createTexture(QQuickWindow *window) const override;
    QSize textureSize() const override;
    int textureByteCount() const override;

private:
    QSize m_size;
    // TODO: Add EGL/Hybris specific members
};

} // namespace Background
} // namespace Silica
} // namespace Sailfish

#endif // SQUARETEXTUREFACTORY_H
