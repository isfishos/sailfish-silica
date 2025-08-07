/*
 * Copyright (c) 2018 - 2020 Jolla Ltd.
 * Copyright (c) 2020 Open Mobile Platform LLC.
 *
 * License: Proprietary
*/

#ifndef SAILFISH_SILICA_BACKGROUND_KERNEL_H
#define SAILFISH_SILICA_BACKGROUND_KERNEL_H

#include "../silicaglobal.h"

#include <QObject>
#include <QSharedData>

namespace Sailfish { namespace Silica { namespace Background {

class KernelPrivate;
class SAILFISH_SILICA_EXPORT Kernel
{
    Q_GADGET
public:
    enum SampleSize {
        SampleSize5,
        SampleSize9,
        SampleSize13,
        SampleSize17,
        SampleSize21,
        SampleSize25,
        SampleSize29,
        SampleSize33
    };
    Q_ENUM(SampleSize)

    Kernel();
    Kernel(const Kernel &kernel);
    Kernel(Kernel &&kernel);
    ~Kernel();

    Kernel &operator =(const Kernel &kernel);
    Kernel &operator =(Kernel &&kernel);

    bool operator ==(const Kernel &kernel) const;
    bool operator !=(const Kernel &kernel) const;

    bool isNull() const;

    SampleSize sampleSize() const;

    float *weights() const;
    int weightCount() const;

    static int weightCountForSampleSize(SampleSize size);

    static Kernel gaussian(SampleSize sampleSize, qreal deviation = 0);
    static Kernel fromWeights(const float *weights, int count);
    static Kernel fromWeights(const QVector<float> &weights);

private:
    Kernel(KernelPrivate *d);

    QExplicitlySharedDataPointer<KernelPrivate> d;
};

}}}

Q_DECLARE_METATYPE(Sailfish::Silica::Background::Kernel)

#endif
