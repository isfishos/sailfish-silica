// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_BACKGROUND_SEQUENCEFILTER_P_H
#define SAILFISH_SILICA_BACKGROUND_SEQUENCEFILTER_P_H

#include "abstractfilter_p.h"
#include "sequencefilter.h"

#include <QtGlobal>
#include <QQmlListProperty>

namespace Sailfish { namespace Silica { namespace Background {

class SequenceFilterPrivate : public AbstractFilterPrivate {
public:
    Q_DECLARE_PUBLIC(SequenceFilter)
    SequenceFilter *q_ptr = nullptr;
    QVector<AbstractFilter*> filters;

    QQmlListProperty<AbstractFilter> filterList()
    {
        auto append = [](QQmlListProperty<AbstractFilter>* lp, AbstractFilter* f){
            auto *self = reinterpret_cast<SequenceFilterPrivate*>(lp->data);
            self->filters.append(f);
            Q_EMIT reinterpret_cast<SequenceFilter*>(lp->object)->filtersChanged();
        };
        auto count = [](QQmlListProperty<AbstractFilter>* lp){
            return reinterpret_cast<SequenceFilterPrivate*>(lp->data)->filters.count();
        };
        auto at = [](QQmlListProperty<AbstractFilter>* lp, int i){
            return reinterpret_cast<SequenceFilterPrivate*>(lp->data)->filters.value(i);
        };
        auto clear = [](QQmlListProperty<AbstractFilter>* lp){
            reinterpret_cast<SequenceFilterPrivate*>(lp->data)->filters.clear();
            Q_EMIT reinterpret_cast<SequenceFilter*>(lp->object)->filtersChanged();
        };
        return QQmlListProperty<AbstractFilter>(q_ptr, this, append, count, at, clear);
    }
};

}}}

#endif // SAILFISH_SILICA_BACKGROUND_SEQUENCEFILTER_P_H


