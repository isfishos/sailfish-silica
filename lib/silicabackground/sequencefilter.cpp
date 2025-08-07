// SPDX-License-Identifier: LGPL-2.1-only

#include "sequencefilter.h"
#include "sequencefilter_p.h"

#include <QQmlListProperty>

namespace Sailfish { namespace Silica { namespace Background {

using ::Sailfish::Silica::Background::SequenceFilterPrivate;

SequenceFilter::SequenceFilter(QObject *parent)
    : AbstractFilter(*new SequenceFilterPrivate, parent)
{
    Q_D(SequenceFilter);
    d->q_ptr = this;
}

SequenceFilter::SequenceFilter(SequenceFilterPrivate &dd, QObject *parent)
    : AbstractFilter(dd, parent)
{
}

SequenceFilter::~SequenceFilter() = default;

QVector<AbstractFilter *> SequenceFilter::filters() const
{
    Q_D(const SequenceFilter);
    return d->filters;
}

void SequenceFilter::setFilters(const QVector<AbstractFilter *> &filters)
{
    Q_D(SequenceFilter);
    if (d->filters == filters)
        return;
    d->filters = filters;
    Q_EMIT filtersChanged();
}

QQmlListProperty<AbstractFilter> SequenceFilter::filterList()
{
    Q_D(SequenceFilter);
    return d->filterList();
}

}}}


