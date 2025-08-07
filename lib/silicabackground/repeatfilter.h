/*
 * Copyright (c) 2020 Open Mobile Platform LLC.
 *
 * License: Proprietary
*/

#ifndef SAILFISH_SILICA_BACKGROUND_REPEATFILTER_H
#define SAILFISH_SILICA_BACKGROUND_REPEATFILTER_H

#include "sequencefilter.h"

namespace Sailfish { namespace Silica { namespace Background {

class RepeatFilterPrivate;
class SAILFISH_SILICA_EXPORT RepeatFilter : public SequenceFilter
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(RepeatFilter)
    Q_PROPERTY(int repetitions READ repetitions WRITE setRepetitions NOTIFY repetitionsChanged)
public:
    explicit RepeatFilter(QObject *parent = nullptr);
    ~RepeatFilter() override;

    int repetitions() const;
    void setRepetitions(int repetitions);

Q_SIGNALS:
    void repetitionsChanged();
};

}}}

#endif
