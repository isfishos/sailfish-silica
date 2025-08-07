// SPDX-License-Identifier: LGPL-2.1-only

#include "kernel.h"

#include <QtMath>

namespace Sailfish { namespace Silica { namespace Background {

class KernelPrivate : public QSharedData {
public:
    Kernel::SampleSize size = Kernel::SampleSize5;
    QVector<float> data;
};

Kernel::Kernel()
    : d(new KernelPrivate)
{
}

Kernel::Kernel(const Kernel &kernel) = default;
Kernel::Kernel(Kernel &&kernel) = default;
Kernel::~Kernel() = default;

Kernel &Kernel::operator=(const Kernel &kernel) = default;
Kernel &Kernel::operator=(Kernel &&kernel) = default;

bool Kernel::operator==(const Kernel &kernel) const
{
    return d->size == kernel.d->size && d->data == kernel.d->data;
}

bool Kernel::operator!=(const Kernel &kernel) const
{
    return !(*this == kernel);
}

bool Kernel::isNull() const
{
    return d->data.isEmpty();
}

Kernel::SampleSize Kernel::sampleSize() const
{
    return d->size;
}

float *Kernel::weights() const
{
    return const_cast<float*>(d->data.constData());
}

int Kernel::weightCount() const
{
    return d->data.size();
}

int Kernel::weightCountForSampleSize(SampleSize size)
{
    // 1D kernel length for separable convolution; mapping by enum index
    static const int map[] = { 5, 9, 13, 17, 21, 25, 29, 33 };
    const int idx = static_cast<int>(size);
    return map[qBound(0, idx, int(sizeof(map)/sizeof(map[0])) - 1)];
}

Kernel Kernel::gaussian(SampleSize sampleSize, qreal deviation)
{
    const int count = weightCountForSampleSize(sampleSize);
    QVector<float> w(count);
    const float sigma = deviation > 0 ? float(deviation) : float(count) / 6.0f; // heuristic
    const float twoSigma2 = 2.0f * sigma * sigma;
    const int mid = count / 2;
    float sum = 0.0f;
    for (int i = 0; i < count; ++i) {
        const int x = i - mid;
        const float val = qExp(-(x * x) / twoSigma2);
        w[i] = val;
        sum += val;
    }
    // normalize
    for (int i = 0; i < count; ++i) w[i] /= sum;
    return fromWeights(w);
}

Kernel Kernel::fromWeights(const float *weights, int count)
{
    Kernel k;
    k.d->data = QVector<float>(weights, weights + count);
    // choose closest enum by count
    const int sizes[] = {5,9,13,17,21,25,29,33};
    int idx = 0;
    int bestDelta = INT_MAX;
    for (int i = 0; i < int(sizeof(sizes)/sizeof(sizes[0])); ++i) {
        int delta = qAbs(sizes[i] - count);
        if (delta < bestDelta) { bestDelta = delta; idx = i; }
    }
    k.d->size = static_cast<SampleSize>(idx);
    return k;
}

Kernel Kernel::fromWeights(const QVector<float> &weights)
{
    return fromWeights(weights.constData(), weights.size());
}

Kernel::Kernel(KernelPrivate *priv)
    : d(priv)
{
}

}}}


