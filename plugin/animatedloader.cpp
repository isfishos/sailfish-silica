// SPDX-License-Identifier: LGPL-2.1-only

#include "animatedloader.h"
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlExpression>
#include <QQmlInfo>
#include <QJSValueIterator>
#include <QTimer>
#include <QQuickWindow>
#include <private/qquickitem_p.h>

namespace {
class PropertyAssigner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject *object READ object CONSTANT)
    Q_PROPERTY(QJSValue properties READ properties CONSTANT)
public:
    static void assignProperties(QQmlEngine *engine, QObject *object, const QJSValue &properties)
    {
        static QHash<QQmlEngine *, PropertyAssigner *> instances;
        PropertyAssigner *&assigner = instances[engine];
        if (!assigner) {
            assigner = new PropertyAssigner(engine);
            QObject::connect(engine, &QObject::destroyed, assigner, [engine, assigner]() {
                instances.remove(engine);
                delete assigner;
            });
        }
        assigner->assignProperties(object, properties);
    }

private:
    explicit PropertyAssigner(QQmlEngine *engine, QObject *parent = nullptr)
        : QObject(parent)
        , m_expression(engine->rootContext(), this, QLatin1String(
              "var target = object\n"
              "var source = properties\n"
              "var errors = []\n"
              "for (var property in source) {\n"
              "    try {\n"
              "        target[property] = source[property]\n"
              "    } catch (error) {\n"
              "        errors.push(error.message)\n"
              "    }\n"
              "}\n"
              "if (errors.length > 0) {"
              "    throw errors.join('\n')\n"
              "}\n"))
    {
    }

    QObject *object() const { return m_object; }
    QJSValue properties() const { return m_properties; }

    void assignProperties(QObject *object, const QJSValue &properties)
    {
        m_object = object;
        m_properties = properties;
        m_expression.evaluate();
        if (m_expression.hasError()) {
            qmlInfo(object) << m_expression.error();
            m_expression.clearError();
        }
        m_object = nullptr;
        m_properties = QJSValue();
    }

    QQmlExpression m_expression;
    QObject *m_object = nullptr;
    QJSValue m_properties;
};
}

AnimatedLoader::AnimatedLoader(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, false);
}

QJSValue AnimatedLoader::source() const { return m_source; }
void AnimatedLoader::setSource(const QJSValue &src)
{
    if (m_source.strictlyEquals(src)) return;
    m_pendingTransition.clear();
    m_pendingProperties = QJSValue();
    m_source = src;
    maybeLoad();
    emit sourceChanged();
}

bool AnimatedLoader::isActive() const { return m_active; }
void AnimatedLoader::setActive(bool a) { if (m_active==a) return; m_active=a; emit activeChanged(); }

bool AnimatedLoader::asynchronous() const { return m_asynchronous; }
void AnimatedLoader::setAsynchronous(bool a) { if (m_asynchronous==a) return; if (a && isComponentComplete()) updatePolish(); m_asynchronous=a; m_asyncExplicit=true; emit asynchronousChanged(); }
void AnimatedLoader::resetAsynchronous() { m_asyncExplicit=false; setAsynchronous(isComponentComplete()); }

AnimatedLoader::Status AnimatedLoader::status() const { return m_status; }
bool AnimatedLoader::isAnimating() const { return m_animating; }

void AnimatedLoader::setAnimating(bool animating)
{
    if (m_animating != animating) {
        m_animating = animating;
        emit animatingChanged();
    }
}
bool AnimatedLoader::isTransitioning() const { return m_transitioning; }

bool AnimatedLoader::transitionEnabled() const { return m_transitionEnabled; }
void AnimatedLoader::setTransitionEnabled(bool e) { if (m_transitionEnabled==e) return; m_transitionEnabled=e; m_transitionEnabledExplicit=true; emit transitionEnabledChanged(); if (m_animating && !m_transitionEnabled) emit completeAnimation(); }
void AnimatedLoader::resetTransitionEnabled() { m_transitionEnabledExplicit=false; setTransitionEnabled(isVisible()); }

QQuickItem *AnimatedLoader::item() const { return m_item; }
QQuickItem *AnimatedLoader::replacedItem() const { return m_replacedItem; }

void AnimatedLoader::setStatus(Status s) { if (m_status==s) return; m_status=s; emit statusChanged(); }
void AnimatedLoader::setItem(QQuickItem *it)
{
    if (m_item==it) return;
    m_item = it; emit itemChanged();
}

void AnimatedLoader::load(const QJSValue &src)
{ load(src, QString()); }

void AnimatedLoader::load(const QJSValue &src, const QString &transition)
{ load(src, transition, QJSValue()); }

void AnimatedLoader::load(const QJSValue &src, const QString &transition, const QJSValue &properties)
{
    m_pendingTransition = transition;
    m_pendingProperties = properties;
    // Mark as transitioning if we are active; actual emission happens on replace
    if (!m_source.strictlyEquals(src)) {
        m_transitioning = m_active;
        setSource(src);
    } else if (!m_source.isUndefined()) {
        m_transitioning = m_active;
    }
}

void AnimatedLoader::applyInitialProperties(QQuickItem *it, const QJSValue &properties)
{
    if (!properties.isObject()) return;
    if (QQmlEngine *eng = qmlEngine(this)) {
        PropertyAssigner::assignProperties(eng, it, properties);
    }
}

void AnimatedLoader::itemChange(ItemChange change, const ItemChangeData &data)
{
    if (change == ItemVisibleHasChanged && data.boolValue) {
        updatePolish();
    }
    QQuickItem::itemChange(change, data);
    if (change == ItemVisibleHasChanged && !m_transitionEnabledExplicit) {
        setTransitionEnabled(data.boolValue);
    }
}

void AnimatedLoader::updatePolish()
{
    if (!m_loadQueued || m_animatingReplace) return;
    Q_ASSERT(!m_replacedItem);

    // Clear previous incubations/components
    if (m_incubator) {
        m_incubator->clear();
    }

    if (m_component && m_component->status() == QQmlComponent::Loading) {
        QObject::disconnect(m_component.data(), &QQmlComponent::statusChanged,
                            this, &AnimatedLoader::componentStatusChanged);
    }

    if (m_ownedComponent) {
        m_ownedComponent->deleteLater();
        m_ownedComponent.reset(nullptr);
    }
    m_component = nullptr;
    m_loadQueued = false;

    Status prev = m_status;
    m_animateReplace = isComponentComplete();

    // Resolve component from source
    if (m_active && !m_source.isUndefined()) {
        m_component = qobject_cast<QQmlComponent *>(m_source.toQObject());
        if (QQmlContext *context = !m_component ? qmlContext(this) : nullptr) {
            const QString path = m_source.toString();
            const QUrl url = path.isEmpty() ? QUrl() : context->resolvedUrl(QUrl(path));
            if (!url.isEmpty()) {
                m_ownedComponent.reset(new QQmlComponent(context->engine(), url,
                    m_asynchronous ? QQmlComponent::Asynchronous : QQmlComponent::PreferSynchronous));
                m_component = m_ownedComponent.data();
            }
        }
    }

    if (m_component) {
        loadComponent();
    } else {
        m_status = Null;
        unload();
    }

    if (m_asynchronous || m_status != Loading) {
        m_animateReplace = true;
    }
    if (m_status != prev && m_status != Ready) emit statusChanged();
}

void AnimatedLoader::maybeLoad(bool force)
{
    if ((!m_loadQueued || force) && isComponentComplete()) {
        polish();
        QQuickWindow *w = window();
        if (!w || !w->isVisible()) QTimer::singleShot(0, this, &AnimatedLoader::updatePolish);
    }
    m_loadQueued = true;
}

void AnimatedLoader::loadComponent()
{
    Q_ASSERT(m_component);
    switch (m_component->status()) {
    case QQmlComponent::Null:
    case QQmlComponent::Error:
        qmlInfo(this) << m_component->errors();
        m_status = Error;
        emit error(m_component->errorString());
        updatePolish();
        if (m_status == Error) unload();
        break;
    case QQmlComponent::Loading:
        m_status = Loading;
        QObject::connect(m_component.data(), &QQmlComponent::statusChanged,
                         this, &AnimatedLoader::componentStatusChanged);
        break;
    case QQmlComponent::Ready: {
        m_status = Loading;
        QQmlContext *forContext = qmlContext(this);
        QQmlContext *context = m_component->creationContext();
        if (!context) context = forContext ? forContext : qmlEngine(this)->rootContext();
        delete m_incubator;
        m_incubator = new Incubator(this, QQmlIncubator::AsynchronousIfNested);
        m_component->create(*m_incubator, context);
        break;
    }
    }
}

void AnimatedLoader::unload()
{
    m_pendingTransition.clear();
    m_pendingProperties = QJSValue();
    if (m_item) replaceItem(nullptr);
}

void AnimatedLoader::componentStatusChanged(QQmlComponent::Status status)
{
    QObject::disconnect(m_component.data(), &QQmlComponent::statusChanged,
                        this, &AnimatedLoader::componentStatusChanged);
    if (status == QQmlComponent::Ready || status == QQmlComponent::Error) loadComponent();
}

void AnimatedLoader::replaceItem(QQuickItem *item)
{
    if (m_animateReplace && transitionEnabled()) {
        m_replacedItem = m_item;
        m_item = item;
        m_animatingReplace = true;
        emit itemChanged();
        emit replacedItemChanged();
        emit animate(m_pendingTransition);
        if (!m_animating) {
            m_animatingReplace = false;
            bool transitioning = m_transitioning;
            m_transitioning = m_loadQueued;
            if (m_replacedItem) {
                QQuickItemPrivate::get(m_replacedItem)->setCulled(true);
                m_replacedItem->deleteLater();
                m_replacedItem = nullptr;
                emit replacedItemChanged();
            }
            if (transitioning && !m_transitioning) { emit transitioningChanged(); emit transitionComplete(); }
        }
    } else {
        bool transitioning = m_transitioning;
        m_transitioning = m_loadQueued;
        if (m_item) {
            QQuickItemPrivate::get(m_item)->setCulled(true);
            m_item->deleteLater();
        }
        m_item = item;
        emit itemChanged();
        if (transitioning && !m_transitioning) { emit transitioningChanged(); emit transitionComplete(); }
    }
}

void AnimatedLoader::setInitialState(QObject *object)
{
    QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
    if (QQuickItem *it = qobject_cast<QQuickItem*>(object)) {
        // Hide until completed
        it->setVisible(false);
        it->setParent(this);
        it->setParentItem(this);
        if (auto *attached = qobject_cast<AnimatedLoaderAttached*>(qmlAttachedPropertiesObject<AnimatedLoader>(it, true))) {
            attached->setAsynchronous(m_asynchronous);
        }
        applyInitialProperties(it, m_pendingProperties);
        emit initializeItem(it);
        if (!m_asyncExplicit && !m_asynchronous) { m_asynchronous = true; emit asynchronousChanged(); }
    } else {
        qmlInfo(this) << "Object is not an Item";
    }
}

void AnimatedLoader::attachedStatusChanged(AnimatedLoaderAttached *attached, Status status)
{
    Q_UNUSED(status)
    if (!attached) return;
    if (m_loadingItem && attached != m_loadingItem) {
        attached->setLoader(nullptr);
        if (attached->item()) attached->item()->deleteLater();
        return;
    }
}

void AnimatedLoader::itemReady(QQuickItem *item)
{
    m_status = Ready;
    item->setVisible(true);
    replaceItem(item);
    m_animateReplace = true;
    emit statusChanged();
    emit loaded();
}

void AnimatedLoader::itemError(const QString &errorString)
{
    m_status = Error;
    emit error(errorString);
    updatePolish();
    if (m_status == Error) {
        if (m_item) replaceItem(nullptr);
        m_animateReplace = true;
        emit statusChanged();
    }
}
void AnimatedLoader::handleIncubatorStatus(QQmlIncubator::Status status)
{
    if (status == QQmlIncubator::Loading) return;
    if (status == QQmlIncubator::Error) {
        setStatus(Error);
        QStringList errs;
        const auto list = m_incubator->errors();
        for (const auto &e : list) errs << e.toString();
        emit error(errs.join('\n'));
        // Continue polish to potentially unload/cleanup
        updatePolish();
        return;
    }

    QObject *obj = m_incubator->object();
    auto newItem = qobject_cast<QQuickItem*>(obj);
    if (!newItem) {
        delete obj;
        setStatus(Error);
        emit error(QStringLiteral("Create failed"));
        updatePolish();
        return;
    }

    // Decull and replace
    // setInitialState is called earlier via Incubator::setInitialState
    QQuickItemPrivate::get(newItem)->setCulled(false);
    replaceItem(newItem);
    m_animateReplace = true;
    setStatus(Ready);
    emit loaded();
}

void AnimatedLoader::componentComplete()
{
    emit aboutToComplete();
    updatePolish();
    QQuickItem::componentComplete();
}

void AnimatedLoader::Incubator::statusChanged(QQmlIncubator::Status status)
{
    if (!m_owner) return;
    m_owner->handleIncubatorStatus(status);
}

AnimatedLoaderAttached *AnimatedLoader::qmlAttachedProperties(QObject *object)
{
    return new AnimatedLoaderAttached(object);
}

void AnimatedLoaderAttached::setStatus(AnimatedLoader::Status status)
{
    if (m_status != status) {
        m_status = status;
        if (m_loader) m_loader->attachedStatusChanged(this, m_status);
        emit statusChanged();
    }
}

void AnimatedLoaderAttached::setErrorString(const QString &error)
{
    if (m_errorString == error) return;
    m_errorString = error;
    emit errorStringChanged();
}

void AnimatedLoaderAttached::setAsynchronous(bool asynchronous)
{
    if (m_asynchronous == asynchronous) return;
    m_asynchronous = asynchronous;
    emit asynchronousChanged();
}

#include "animatedloader.moc"
