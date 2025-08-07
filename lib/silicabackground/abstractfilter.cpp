// SPDX-License-Identifier: LGPL-2.1-only

#include "abstractfilter.h"
#include "abstractfilter_p.h"

#include <QImage>
#include <QQuickWindow>

namespace Sailfish { namespace Silica { namespace Background {

AbstractFilter::AbstractFilter(AbstractFilterPrivate &dd, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

AbstractFilter::~AbstractFilter() = default;

bool AbstractFilter::isEnabled() const
{
    Q_D(const AbstractFilter);
    return d->enabled;
}

void AbstractFilter::setEnabled(bool enabled)
{
    Q_D(AbstractFilter);
    if (d->enabled == enabled)
        return;
    d->enabled = enabled;
    emit enabledChanged();
    emit filterChanged();
}

QSize AbstractFilter::targetSize(const QSize &sourceSize) const
{
    return sourceSize;
}

bool AbstractFilter::isValid() const
{
    return isEnabled();
}

bool AbstractFilter::apply(const TextureInfo &/*source*/, const TextureInfo &/*target*/, QObject * /*properties*/, int /*propertyOffset*/, QQuickWindow * /*window*/)
{
    // Not implemented for texture path in minimal version
    return false;
}

bool AbstractFilter::apply(const TextureInfo &source, const TextureInfo &target, const QVariantMap &properties)
{
    Q_UNUSED(source)
    Q_UNUSED(target)
    Q_UNUSED(properties)
    return false;
}

QImage AbstractFilter::apply(const QImage &source, QObject * /*properties*/, int /*propertyOffset*/, QQuickWindow * /*window*/)
{
    // Default: return unchanged
    return source;
}

QImage AbstractFilter::apply(const QImage &source, const QVariantMap &/*properties*/)
{
    return source;
}

Task *AbstractFilter::updateTask(Task *task, QObject * /*properties*/, int /*propertyOffset*/, QQuickWindow * /*window*/, const QMetaMethod &/*update*/)
{
    return task; // no-op
}

Task *AbstractFilter::updateTask(Task *task, const QVariantMap &/*properties*/)
{
    return task; // no-op
}

}}}


