// SPDX-License-Identifier: LGPL-2.1-only

#include "animatedloader.h"
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QJSValueIterator>

AnimatedLoader::AnimatedLoader(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, false);
}

QJSValue AnimatedLoader::source() const { return m_source; }
void AnimatedLoader::setSource(const QJSValue &src)
{
    if (m_source.strictlyEquals(src)) return;
    m_source = src; emit sourceChanged();
}

bool AnimatedLoader::isActive() const { return m_active; }
void AnimatedLoader::setActive(bool a) { if (m_active==a) return; m_active=a; emit activeChanged(); }

bool AnimatedLoader::asynchronous() const { return m_asynchronous; }
void AnimatedLoader::setAsynchronous(bool a) { m_asynchronous=a; m_asyncExplicit=true; emit asynchronousChanged(); }
void AnimatedLoader::resetAsynchronous() { m_asyncExplicit=false; m_asynchronous=false; emit asynchronousChanged(); }

AnimatedLoader::Status AnimatedLoader::status() const { return m_status; }
bool AnimatedLoader::isAnimating() const { return m_animating; }
bool AnimatedLoader::isTransitioning() const { return m_transitioning; }

bool AnimatedLoader::transitionEnabled() const { return m_transitionEnabled; }
void AnimatedLoader::setTransitionEnabled(bool e) { m_transitionEnabled=e; m_transitionEnabledExplicit=true; emit transitionEnabledChanged(); }
void AnimatedLoader::resetTransitionEnabled() { m_transitionEnabledExplicit=false; m_transitionEnabled=true; emit transitionEnabledChanged(); }

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
    if (!isActive()) { return; }
    setSource(src);
    setStatus(Loading);
    QQmlEngine *eng = qmlEngine(this);
    if (!eng) { setStatus(Error); emit error(QStringLiteral("No engine")); return; }

    QQmlComponent *component = nullptr;
    QScopedPointer<QQmlComponent> owned;
    if (src.isString()) {
        owned.reset(new QQmlComponent(eng, QUrl(src.toString())));
        component = owned.data();
    } else if (src.isQObject()) {
        component = qobject_cast<QQmlComponent*>(src.toQObject());
    }
    if (!component) { setStatus(Error); emit error(QStringLiteral("Invalid source")); return; }
    m_pendingTransition = transition;
    m_pendingProperties = properties;
    m_activeComponent = component;

    const auto mode = m_asynchronous ? QQmlIncubator::Asynchronous : QQmlIncubator::AsynchronousIfNested;
    delete m_incubator;
    m_incubator = new Incubator(this, mode);
    component->create(*m_incubator, eng->rootContext());
}

void AnimatedLoader::completeAnimation()
{
    if (!m_transitioning)
        return;
    m_transitioning = false; emit transitioningChanged();
    m_animating = false; emit animatingChanged();
    if (m_replacedItem) {
        m_replacedItem->setParentItem(nullptr);
        m_replacedItem->deleteLater();
        m_replacedItem = nullptr;
        emit replacedItemChanged();
    }
    m_pendingTransition.clear();
    emit transitionComplete();
}

void AnimatedLoader::applyInitialProperties(QQuickItem *it, const QJSValue &properties)
{
    if (!properties.isObject())
        return;
    QJSValueIterator itProps(properties);
    while (itProps.hasNext()) {
        itProps.next();
        it->setProperty(itProps.name().toUtf8().constData(), itProps.value().toVariant());
    }
}

void AnimatedLoader::handleIncubatorStatus(QQmlIncubator::Status status)
{
    if (status == QQmlIncubator::Loading)
        return;

    if (status == QQmlIncubator::Error) {
        setStatus(Error);
        QStringList errs;
        const auto list = m_incubator->errors();
        for (const auto &e : list) errs << e.toString();
        emit error(errs.join('\n'));
        return;
    }

    QObject *obj = m_incubator->object();
    auto newItem = qobject_cast<QQuickItem*>(obj);
    if (!newItem) {
        delete obj;
        setStatus(Error);
        emit error(QStringLiteral("Create failed"));
        return;
    }

    newItem->setParentItem(this);
    if (newItem->parentItem() == this) {
        newItem->setX(0);
        newItem->setY(0);
    }
    applyInitialProperties(newItem, m_pendingProperties);

    emit initializeItem(newItem);
    m_replacedItem = m_item;
    emit replacedItemChanged();
    setItem(newItem);
    setStatus(Ready);
    emit loaded();
    if (transitionEnabled() && !m_pendingTransition.isEmpty()) {
        m_transitioning = true;
        emit transitioningChanged();
        m_animating = true;
        emit animatingChanged();
        emit animate(m_pendingTransition);
    }
}

void AnimatedLoader::Incubator::statusChanged(QQmlIncubator::Status status)
{
    if (!m_owner)
        return;
    m_owner->handleIncubatorStatus(status);
}

AnimatedLoaderAttached *AnimatedLoader::qmlAttachedProperties(QObject *object)
{
    return new AnimatedLoaderAttached(object);
}
