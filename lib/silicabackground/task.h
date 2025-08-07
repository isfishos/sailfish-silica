/*
 * Copyright (c) 2020 Open Mobile Platform LLC.
 *
 * License: Proprietary
*/

#ifndef SAILFISH_SILICA_BACKGROUND_TASK_H
#define SAILFISH_SILICA_BACKGROUND_TASK_H

#include "../silicaglobal.h"

#include <qopengl.h>

#include <QMetaMethod>
#include <QVector>

namespace Sailfish { namespace Silica { namespace Background {

class AbstractFilter;
class TextureInfo;

class SAILFISH_SILICA_EXPORT Task
{
public:
    virtual ~Task();

    virtual bool takeDirty() = 0;

    virtual QSize targetSize(const QSize &sourceSize) const = 0;

    virtual void execute(const TextureInfo &source, const TextureInfo &target) = 0;
};

}}}

#endif
