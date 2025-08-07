/*
 * Copyright (c) 2020 Open Mobile Platform LLC.
 *
 * License: Proprietary
*/

#ifndef SAILFISH_SILICA_BACKGROUND_SEQUENCEFILTER_H
#define SAILFISH_SILICA_BACKGROUND_SEQUENCEFILTER_H

#include "abstractfilter.h"

#include <QQmlListProperty>

namespace Sailfish { namespace Silica { namespace Background {

class SequenceFilterPrivate;
class SAILFISH_SILICA_EXPORT SequenceFilter : public AbstractFilter
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SequenceFilter)
    Q_PROPERTY(QQmlListProperty<Sailfish::Silica::Background::AbstractFilter> filters READ filterList NOTIFY filtersChanged)
    Q_CLASSINFO("DefaultProperty", "filters")
public:
    explicit SequenceFilter(QObject *parent = nullptr);
    ~SequenceFilter() override;

    QVector<AbstractFilter *> filters() const;
    void setFilters(const QVector<AbstractFilter *> &filters);

    QQmlListProperty<AbstractFilter> filterList();

Q_SIGNALS:
    void filtersChanged();

protected:
    explicit SequenceFilter(SequenceFilterPrivate &dd, QObject *parent);
};

}}}

#endif
