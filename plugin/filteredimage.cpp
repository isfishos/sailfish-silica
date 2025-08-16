// SPDX-License-Identifier: LGPL-2.1-only

#include "filteredimage.h"
#include "../lib/silicabackground/abstractfilter.h"

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

QQmlListProperty<Sailfish::Silica::Background::AbstractFilter> FilteredImage::filters()
{
    return QQmlListProperty<Sailfish::Silica::Background::AbstractFilter>(
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

void FilteredImage::filters_append(QQmlListProperty<Sailfish::Silica::Background::AbstractFilter> *prop, Sailfish::Silica::Background::AbstractFilter *filter)
{
    FilteredImage *item = qobject_cast<FilteredImage*>(prop->object);
    if (item) {
        item->m_filters.append(filter);
        Q_EMIT item->filtersChanged();
    }
}

qsizetype FilteredImage::filters_count(QQmlListProperty<Sailfish::Silica::Background::AbstractFilter> *prop)
{
    FilteredImage *item = qobject_cast<FilteredImage*>(prop->object);
    return item ? item->m_filters.count() : 0;
}

Sailfish::Silica::Background::AbstractFilter *FilteredImage::filters_at(QQmlListProperty<Sailfish::Silica::Background::AbstractFilter> *prop, qsizetype index)
{
    FilteredImage *item = qobject_cast<FilteredImage*>(prop->object);
    return item && index >= 0 && index < item->m_filters.count() ? item->m_filters.at(index) : nullptr;
}

void FilteredImage::filters_clear(QQmlListProperty<Sailfish::Silica::Background::AbstractFilter> *prop)
{
    FilteredImage *item = qobject_cast<FilteredImage*>(prop->object);
    if (item) {
        item->m_filters.clear();
        Q_EMIT item->filtersChanged();
    }
}
