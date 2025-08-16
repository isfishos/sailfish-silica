/*
 * Copyright (c) 2018 - 2020 Jolla Ltd.
 * Copyright (c) 2020 Open Mobile Platform LLC.
 *
 * License: Proprietary
*/

#ifndef SAILFISH_SILICA_BACKGROUND_CONVOLUTIONFILTER_H
#define SAILFISH_SILICA_BACKGROUND_CONVOLUTIONFILTER_H

#include "abstractfilter.h"

#include "kernel.h"

namespace Sailfish { namespace Silica { namespace Background {

class ConvolutionFilterPrivate;
class SAILFISH_SILICA_EXPORT ConvolutionFilter : public AbstractFilter
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ConvolutionFilter)
    Q_PROPERTY(Sailfish::Silica::Background::Kernel kernel READ kernel WRITE setKernel NOTIFY kernelChanged RESET resetKernel)
public:
    explicit ConvolutionFilter(QObject *parent = nullptr);
    ~ConvolutionFilter() override;

    Kernel kernel() const;
    void setKernel(const Kernel &kernel);
    void resetKernel();

Q_SIGNALS:
    void kernelChanged();

protected:
    explicit ConvolutionFilter(ConvolutionFilterPrivate &dd, QObject *parent);

private:
};

}}}


#endif
