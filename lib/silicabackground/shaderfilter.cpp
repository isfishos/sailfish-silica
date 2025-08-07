// SPDX-License-Identifier: LGPL-2.1-only

#include "shaderfilter.h"
#include "abstractfilter_p.h"

namespace Sailfish { namespace Silica { namespace Background {

class ShaderFilterPrivate : public AbstractFilterPrivate {
public:
    QString vertex;
    QString fragment;
};

ShaderFilter::ShaderFilter(QObject *parent)
    : AbstractFilter(*new ShaderFilterPrivate, parent)
{
}

ShaderFilter::~ShaderFilter() = default;

QString ShaderFilter::vertexShader() const
{
    Q_D(const ShaderFilter);
    return d->vertex;
}

void ShaderFilter::setVertexShader(const QString &shader)
{
    Q_D(ShaderFilter);
    if (d->vertex == shader)
        return;
    d->vertex = shader;
    Q_EMIT vertexShaderChanged();
    Q_EMIT filterChanged();
}

void ShaderFilter::resetVertexShader()
{
    setVertexShader(QString());
}

QString ShaderFilter::fragmentShader() const
{
    Q_D(const ShaderFilter);
    return d->fragment;
}

void ShaderFilter::setFragmentShader(const QString &shader)
{
    Q_D(ShaderFilter);
    if (d->fragment == shader)
        return;
    d->fragment = shader;
    Q_EMIT fragmentShaderChanged();
    Q_EMIT filterChanged();
}

void ShaderFilter::resetFragmentShader()
{
    setFragmentShader(QString());
}

void ShaderFilter::attributesChanged()
{
    Q_EMIT filterChanged();
}

}}}


