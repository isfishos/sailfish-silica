/*
 * Copyright (c) 2018 - 2020 Jolla Ltd.
 * Copyright (c) 2020 Open Mobile Platform LLC.
 *
 * License: Proprietary
*/

#ifndef SAILFISH_SILICA_BACKGROUND_RESIZEFILTER_H
#define SAILFISH_SILICA_BACKGROUND_RESIZEFILTER_H

#include "convolutionfilter.h"
#include "fill.h"

namespace Sailfish { namespace Silica { namespace Background {

class ResizeFilterPrivate;
class SAILFISH_SILICA_EXPORT ResizeFilter : public ConvolutionFilter
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ResizeFilter)
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged RESET resetSize)
    Q_PROPERTY(Sailfish::Silica::Background::Fill::Mode fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged)
    Q_PROPERTY(int maximumScaleFactor READ maximumScaleFactor WRITE setMaximumScaleFactor NOTIFY maximumScaleFactorChanged)
public:
    explicit ResizeFilter(QObject *parent = nullptr);
    ~ResizeFilter() override;

    QSize size() const;
    void setSize(const QSize &size);

    Fill::Mode fillMode() const;
    void setFillMode(Fill::Mode mode);

    int maximumScaleFactor() const;
    void setMaximumScaleFactor(int maximum);

Q_SIGNALS:
    void sizeChanged();
    void fillModeChanged();
    void maximumScaleFactorChanged();

protected:
    virtual void resetSize();
};

}}}


#endif
