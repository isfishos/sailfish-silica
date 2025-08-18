// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_ANIMATEDLOADER_H
#define SAILFISH_SILICA_PLUGIN_ANIMATEDLOADER_H

#include <QQuickItem>
#include <QJSValue>
#include <QPointer>
#include <qqml.h>
#include <QQmlIncubator>
#include <QQmlComponent>

class AnimatedLoaderAttached;

class AnimatedLoader : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QJSValue source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool asynchronous READ asynchronous WRITE setAsynchronous RESET resetAsynchronous NOTIFY asynchronousChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool animating READ isAnimating WRITE setAnimating NOTIFY animatingChanged)
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
    void setAnimating(bool animating);
    bool isTransitioning() const;
    bool transitionEnabled() const; void setTransitionEnabled(bool e); void resetTransitionEnabled();
    QQuickItem *item() const;
    QQuickItem *replacedItem() const;

    Q_INVOKABLE void load(const QJSValue &source);
    Q_INVOKABLE void load(const QJSValue &source, const QString &transition);
    Q_INVOKABLE void load(const QJSValue &source, const QString &transition, const QJSValue &properties);

    static AnimatedLoaderAttached *qmlAttachedProperties(QObject *object);

protected:
    void classBegin() override { QQuickItem::classBegin(); }
    void componentComplete() override;
    void itemChange(ItemChange change, const ItemChangeData &data) override;
    void updatePolish() override;

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
    void completeAnimation();

private:
    // Loading flow helpers
    void maybeLoad(bool force = false);
    void loadComponent();
    void unload();
    void componentStatusChanged(QQmlComponent::Status status);
    void replaceItem(QQuickItem *item);

    // Incubation helpers
    void setInitialState(QObject *object);
    void handleIncubatorStatus(QQmlIncubator::Status status);
public:
    void attachedStatusChanged(class AnimatedLoaderAttached *attached, Status status);
    void itemReady(QQuickItem *item);
    void itemError(const QString &errorString);

    // Utility
    void setStatus(Status s);
    void setItem(QQuickItem *it);
    void applyInitialProperties(QQuickItem *it, const QJSValue &properties);

    class Incubator : public QQmlIncubator {
    public:
        explicit Incubator(AnimatedLoader *owner, IncubationMode mode)
            : QQmlIncubator(mode)
            , m_owner(owner)
        {}
    protected:
        void setInitialState(QObject *object) override { if (m_owner) m_owner->setInitialState(object); }
        void statusChanged(Status status) override;
    private:
        AnimatedLoader *m_owner = nullptr;
    };

    Incubator *m_incubator = nullptr;

    // State
    QJSValue m_source;
    bool m_active = true;
    bool m_asyncExplicit = false;
    bool m_asynchronous = false;
    bool m_transitionEnabledExplicit = false;
    bool m_transitionEnabled = true;
    Status m_status = Null;
    bool m_animating = false;
    bool m_transitioning = false;
    bool m_animatingReplace = false;
    bool m_loadQueued = false;
    bool m_animateReplace = true;

    QPointer<QQuickItem> m_item;
    QPointer<QQuickItem> m_replacedItem;
    QString m_pendingTransition;
    QJSValue m_pendingProperties;

    QPointer<QQmlComponent> m_component;
    QScopedPointer<QQmlComponent> m_ownedComponent;
    QPointer<class AnimatedLoaderAttached> m_loadingItem;
};

class AnimatedLoaderAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AnimatedLoader::Status status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged)
    Q_PROPERTY(bool asynchronous READ asynchronous NOTIFY asynchronousChanged)
public:
    explicit AnimatedLoaderAttached(QObject *parent = nullptr) : QObject(parent), m_item(qobject_cast<QQuickItem*>(parent)) {}
    AnimatedLoader::Status status() const { return m_status; }
    void setStatus(AnimatedLoader::Status status);
    QString errorString() const { return m_errorString; }
    void setErrorString(const QString &error);
    bool asynchronous() const { return m_asynchronous; }
    void setAsynchronous(bool asynchronous);
    void setLoader(AnimatedLoader *loader) { m_loader = loader; }
    QQuickItem *item() const { return m_item; }
Q_SIGNALS:
    void statusChanged();
    void errorStringChanged();
    void asynchronousChanged();

private:
    QPointer<AnimatedLoader> m_loader;
    QQuickItem * const m_item;
    QString m_errorString;
    AnimatedLoader::Status m_status = AnimatedLoader::Ready;
    bool m_asynchronous = false;
};

QML_DECLARE_TYPEINFO(AnimatedLoader, QML_HAS_ATTACHED_PROPERTIES)

#endif // SAILFISH_SILICA_PLUGIN_ANIMATEDLOADER_H
