// SPDX-License-Identifier: LGPL-2.1-only

#include "filteredimage.h"
#include "abstractfilter.h"

namespace Sailfish {
namespace Silica {
namespace Background {

FilteredImage::FilteredImage(QQuickItem *parent)
    : QQuickImageBase(parent)
{
}

FilteredImage::~FilteredImage() = default;

void FilteredImage::setFiltering(bool filtering)
{
    if (m_filtering == filtering)
        return;
    m_filtering = filtering;
    Q_EMIT filteringChanged();
    update();
}

QQmlListProperty<AbstractFilter> FilteredImage::filters()
{
    return QQmlListProperty<AbstractFilter>(
        this, this,
        &FilteredImage::filters_append,
        &FilteredImage::filters_count,
        &FilteredImage::filters_at,
        &FilteredImage::filters_clear);
}

void FilteredImage::pixmapChange()
{
    QQuickImageBase::pixmapChange();
    // Apply filters if needed
    if (m_filtering && !m_filters.isEmpty()) {
        // TODO: Apply filtering to the image
        update();
    }
}

void FilteredImage::filters_append(QQmlListProperty<AbstractFilter> *prop, AbstractFilter *filter)
{
    FilteredImage *item = qobject_cast<FilteredImage*>(prop->object);
    if (item) {
        item->m_filters.append(filter);
        Q_EMIT item->filtersChanged();
    }
}

int FilteredImage::filters_count(QQmlListProperty<AbstractFilter> *prop)
{
    FilteredImage *item = qobject_cast<FilteredImage*>(prop->object);
    return item ? item->m_filters.count() : 0;
}

AbstractFilter *FilteredImage::filters_at(QQmlListProperty<AbstractFilter> *prop, int index)
{
    FilteredImage *item = qobject_cast<FilteredImage*>(prop->object);
    return item && index >= 0 && index < item->m_filters.count() ? item->m_filters.at(index) : nullptr;
}

void FilteredImage::filters_clear(QQmlListProperty<AbstractFilter> *prop)
{
    FilteredImage *item = qobject_cast<FilteredImage*>(prop->object);
    if (item) {
        item->m_filters.clear();
        Q_EMIT item->filtersChanged();
    }
}

} // namespace Background
} // namespace Silica
} // namespace Sailfish
