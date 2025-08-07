/*
 * Copyright (c) 2020 Open Mobile Platform LLC.
 *
 * License: Proprietary
*/

#ifndef SAILFISH_SILICA_BACKGROUND_ABSTRACTFILTER_H
#define SAILFISH_SILICA_BACKGROUND_ABSTRACTFILTER_H

#include "../silicaglobal.h"

#include <QObject>

#include <QMetaMethod>
#include <QRect>

#include <qopengl.h>

QT_BEGIN_NAMESPACE
class QQuickWindow;
QT_END_NAMESPACE

namespace Sailfish { namespace Silica { namespace Background {

struct TextureInfo
{
    TextureInfo() = default;
    TextureInfo(GLuint id, GLenum format, const QSize &size) : id(id), format(format), size(size) {}
    TextureInfo(const TextureInfo &) = default;
    ~TextureInfo() = default;

    TextureInfo &operator =(const TextureInfo &) = default;

    GLuint id = 0;
    GLenum format = GL_RGBA;
    QSize size;
    void *pixelData = nullptr;
};

class Task;
class AbstractFilterPrivate;
class SAILFISH_SILICA_EXPORT AbstractFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_DECLARE_PRIVATE(AbstractFilter)
public:
    ~AbstractFilter() override;

    bool isEnabled() const;
    void setEnabled(bool enabled);

    QSize targetSize(const QSize &sourceSize) const;
    bool isValid() const;

    bool apply(
            const TextureInfo &source,
            const TextureInfo &target,
            QObject *properties = nullptr,
            int propertyOffset = QObject::staticMetaObject.propertyCount(),
            QQuickWindow *window = nullptr);
    bool apply(
            const TextureInfo &source,  const TextureInfo &target, const QVariantMap &properties);

    QImage apply(
            const QImage &source,
            QObject *properties = nullptr,
            int propertyOffset = QObject::staticMetaObject.propertyCount(),
            QQuickWindow *window = nullptr);
    QImage apply(const QImage &source, const QVariantMap &properties);

    Task *updateTask(
            Task *task,
            QObject *properties = nullptr,
            int propertyOffset = QObject::staticMetaObject.propertyCount(),
            QQuickWindow *window = nullptr,
            const QMetaMethod &update = QMetaMethod());
    Task *updateTask(Task *task, const QVariantMap &properties);

    template <typename... A>
    Task *createTask(A... arguments)
    {
        return updateTask(nullptr, arguments...);
    }

Q_SIGNALS:
    void enabledChanged();
    void filterChanged();

protected:
    explicit AbstractFilter(AbstractFilterPrivate &dd, QObject *parent);

    QScopedPointer<AbstractFilterPrivate> d_ptr;
};

}}}

#endif
