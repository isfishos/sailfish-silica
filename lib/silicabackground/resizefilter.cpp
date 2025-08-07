// SPDX-License-Identifier: LGPL-2.1-only

#include "resizefilter.h"
#include "convolutionfilter_p.h"

namespace Sailfish { namespace Silica { namespace Background {

class ResizeFilterPrivate : public ConvolutionFilterPrivate {
public:
    QSize size;
    Fill::Mode mode = Fill::Stretch;
    int maximumScale = 2;
};

ResizeFilter::ResizeFilter(QObject *parent)
    : ConvolutionFilter(*new ResizeFilterPrivate, parent)
{
}

ResizeFilter::~ResizeFilter() = default;

QSize ResizeFilter::size() const
{
    Q_D(const ResizeFilter);
    return d->size;
}

void ResizeFilter::setSize(const QSize &size)
{
    Q_D(ResizeFilter);
    if (d->size == size)
        return;
    d->size = size;
    Q_EMIT sizeChanged();
    Q_EMIT filterChanged();
}

Fill::Mode ResizeFilter::fillMode() const
{
    Q_D(const ResizeFilter);
    return d->mode;
}

void ResizeFilter::setFillMode(Fill::Mode mode)
{
    Q_D(ResizeFilter);
    if (d->mode == mode)
        return;
    d->mode = mode;
    Q_EMIT fillModeChanged();
    Q_EMIT filterChanged();
}

int ResizeFilter::maximumScaleFactor() const
{
    Q_D(const ResizeFilter);
    return d->maximumScale;
}

void ResizeFilter::setMaximumScaleFactor(int maximum)
{
    Q_D(ResizeFilter);
    if (d->maximumScale == maximum)
        return;
    d->maximumScale = maximum;
    Q_EMIT maximumScaleFactorChanged();
    Q_EMIT filterChanged();
}

void ResizeFilter::resetSize()
{
    setSize(QSize());
}

}}}


