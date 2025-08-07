// SPDX-License-Identifier: LGPL-2.1-only

#include "repeatfilter.h"
#include "sequencefilter_p.h"

namespace Sailfish { namespace Silica { namespace Background {

class RepeatFilterPrivate : public SequenceFilterPrivate {
public:
    int repetitions = 1;
};

RepeatFilter::RepeatFilter(QObject *parent)
    : SequenceFilter(*new RepeatFilterPrivate, parent)
{
}

RepeatFilter::~RepeatFilter() = default;

int RepeatFilter::repetitions() const
{
    Q_D(const RepeatFilter);
    return d->repetitions;
}

void RepeatFilter::setRepetitions(int repetitions)
{
    Q_D(RepeatFilter);
    if (d->repetitions == repetitions)
        return;
    d->repetitions = repetitions;
    Q_EMIT repetitionsChanged();
    Q_EMIT filterChanged();
}

}}}


