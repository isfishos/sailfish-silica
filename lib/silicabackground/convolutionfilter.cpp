// SPDX-License-Identifier: LGPL-2.1-only

#include "convolutionfilter.h"
#include "convolutionfilter_p.h"

namespace Sailfish { namespace Silica { namespace Background {

ConvolutionFilter::ConvolutionFilter(QObject *parent)
    : AbstractFilter(*new ConvolutionFilterPrivate, parent)
{
}

ConvolutionFilter::ConvolutionFilter(ConvolutionFilterPrivate &dd, QObject *parent)
    : AbstractFilter(dd, parent)
{
}

ConvolutionFilter::~ConvolutionFilter() = default;

Kernel ConvolutionFilter::kernel() const
{
    Q_D(const ConvolutionFilter);
    return d->kernel;
}

void ConvolutionFilter::setKernel(const Kernel &kernel)
{
    Q_D(ConvolutionFilter);
    if (d->kernel == kernel)
        return;
    d->kernel = kernel;
    Q_EMIT kernelChanged();
    Q_EMIT filterChanged();
}

void ConvolutionFilter::resetKernel()
{
    setKernel(Kernel());
}

}}}


