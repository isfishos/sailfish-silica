// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_ANIMATEDLOADER_H
#define SAILFISH_SILICA_PLUGIN_ANIMATEDLOADER_H

#include <QQuickItem>
#include <QJSValue>
#include <QPointer>
#include <qqml.h>
#include <QQmlIncubator>

class AnimatedLoaderAttached;

class AnimatedLoader : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QJSValue source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool asynchronous READ asynchronous WRITE setAsynchronous RESET resetAsynchronous NOTIFY asynchronousChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool animating READ isAnimating NOTIFY animatingChanged)
    Q_PROPERTY(bool transitioning READ isTransitioning NOTIFY transitioningChanged)
    Q_PROPERTY(bool transitionEnabled READ transitionEnabled WRITE setTransitionEnabled RESET resetTransitionEnabled NOTIFY transitionEnabledChanged)
    Q_PROPERTY(QQuickItem* item READ item NOTIFY itemChanged)
    Q_PROPERTY(QQuickItem* replacedItem READ replacedItem NOTIFY replacedItemChanged)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    enum Status { Null, Ready, Loading, Error };
    Q_ENUM(Status)

    explicit AnimatedLoader(QQuickItem *parent = nullptr);

    QJSValue source() const; void setSource(const QJSValue &src);
    bool isActive() const; void setActive(bool a);
    bool asynchronous() const; void setAsynchronous(bool a); void resetAsynchronous();
    Status status() const;
    bool isAnimating() const;
    bool isTransitioning() const;
    bool transitionEnabled() const; void setTransitionEnabled(bool e); void resetTransitionEnabled();
    QQuickItem *item() const;
    QQuickItem *replacedItem() const;

    Q_INVOKABLE void load(const QJSValue &source);
    Q_INVOKABLE void load(const QJSValue &source, const QString &transition);
    Q_INVOKABLE void load(const QJSValue &source, const QString &transition, const QJSValue &properties);
    Q_INVOKABLE void completeAnimation();

    static AnimatedLoaderAttached *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void sourceChanged();
    void activeChanged();
    void asynchronousChanged();
    void statusChanged();
    void animatingChanged();
    void transitioningChanged();
    void transitionEnabledChanged();
    void itemChanged();
    void replacedItemChanged();
    void transitionComplete();
    void aboutToComplete();
    void loaded();
    void error(const QString &errorString);
    void initializeItem(QQuickItem *item);
    void animate(const QString &transition);


private:
    QJSValue m_source;
    bool m_active = true;
    bool m_asyncExplicit = false;
    bool m_asynchronous = false;
    bool m_transitionEnabledExplicit = false;
    bool m_transitionEnabled = true;
    Status m_status = Null;
    bool m_animating = false;
    bool m_transitioning = false;
    QPointer<QQuickItem> m_item;
    QPointer<QQuickItem> m_replacedItem;
    QString m_pendingTransition;
    QJSValue m_pendingProperties;
    QQmlComponent *m_activeComponent = nullptr;

    class Incubator : public QQmlIncubator {
    public:
        explicit Incubator(AnimatedLoader *owner, IncubationMode mode)
            : QQmlIncubator(mode)
            , m_owner(owner)
        {}
    protected:
        void statusChanged(Status status) override;
    private:
        AnimatedLoader *m_owner = nullptr;
    };

    Incubator *m_incubator = nullptr;

    void setStatus(Status s);
    void setItem(QQuickItem *it);
    void applyInitialProperties(QQuickItem *it, const QJSValue &properties);
    void handleIncubatorStatus(QQmlIncubator::Status status);
};

class AnimatedLoaderAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AnimatedLoader::Status status READ status NOTIFY dummy)
    Q_PROPERTY(QString errorString READ errorString NOTIFY dummy)
    Q_PROPERTY(bool asynchronous READ asynchronous NOTIFY dummy)
public:
    explicit AnimatedLoaderAttached(QObject *parent = nullptr) : QObject(parent) {}
    AnimatedLoader::Status status() const { return AnimatedLoader::Null; }
    QString errorString() const { return QString(); }
    bool asynchronous() const { return false; }
Q_SIGNALS:
    void dummy();
};

QML_DECLARE_TYPEINFO(AnimatedLoader, QML_HAS_ATTACHED_PROPERTIES)

#endif // SAILFISH_SILICA_PLUGIN_ANIMATEDLOADER_H
