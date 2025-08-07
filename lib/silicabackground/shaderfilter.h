/*
 * Copyright (c) 2020 Open Mobile Platform LLC.
 *
 * License: Proprietary
*/

#ifndef SAILFISH_SILICA_BACKGROUND_SHADERFILTER_H
#define SAILFISH_SILICA_BACKGROUND_SHADERFILTER_H

#include "abstractfilter.h"

namespace Sailfish { namespace Silica { namespace Background {

class ShaderFilterPrivate;
class SAILFISH_SILICA_EXPORT ShaderFilter : public AbstractFilter
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ShaderFilter)
    Q_PROPERTY(QString vertexShader READ vertexShader WRITE setVertexShader NOTIFY vertexShaderChanged RESET resetVertexShader)
    Q_PROPERTY(QString fragmentShader READ fragmentShader WRITE setFragmentShader NOTIFY fragmentShaderChanged RESET resetFragmentShader)
public:
    explicit ShaderFilter(QObject *parent = nullptr);
    ~ShaderFilter() override;

    QString vertexShader() const;
    void setVertexShader(const QString &shader);
    void resetVertexShader();

    QString fragmentShader() const;
    void setFragmentShader(const QString &shader);
    void resetFragmentShader();

Q_SIGNALS:
    void vertexShaderChanged();
    void fragmentShaderChanged();

private Q_SLOTS:
    void attributesChanged();
};

}}}

#endif
