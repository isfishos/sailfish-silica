#ifndef SILICA_IMAGEPROVIDER_P_H
#define SILICA_IMAGEPROVIDER_P_H

#include "silicaimageprovider.h"
#include <QHash>

namespace Silica {

class ImageProviderPrivate {
public:
    ThemeIconResolver iconResolver;
    QHash<QString, QImage> cache;
};

} // namespace Silica

#endif