// SPDX-License-Identifier: LGPL-2.1-only

#include "autofill.h"
#include <QQuickItem>

AutoFill::AutoFill(QObject *parent)
    : QObject(parent)
{
}

QString AutoFill::key() const
{
    return m_key;
}

void AutoFill::setKey(const QString &k)
{
    if (m_key == k)
        return;
    m_key = k;
    Q_EMIT keyChanged();
}

QString AutoFill::partialText() const
{
    return m_partial;
}

void AutoFill::setPartialText(const QString &t)
{
    if (m_partial == t)
        return;
    m_partial = t;
    m_partialExplicit = true;
    Q_EMIT partialTextChanged();
    scheduleRecompute();
}

int AutoFill::maximumSuggestions() const
{
    return m_max;
}

void AutoFill::setMaximumSuggestions(int v)
{
    if (m_max == v)
        return;
    m_max = v;
    Q_EMIT maximumSuggestionsChanged();
    scheduleRecompute();
}

bool AutoFill::canRemove() const
{
    return m_canRemove;
}

void AutoFill::setCanRemove(bool v)
{
    if (m_canRemove == v)
        return;
    m_canRemove = v;
    Q_EMIT canRemoveChanged();
}

QStringList AutoFill::suggestions() const
{
    return m_suggestions;
}

void AutoFill::setSuggestions(const QStringList &s)
{
    if (m_suggestions == s)
        return;
    m_suggestions = s;
    Q_EMIT suggestionsChanged();
}

QQuickItem *AutoFill::inputItem() const
{
    return m_input;
}

void AutoFill::setInputItem(QQuickItem *i)
{
    if (m_input == i)
        return;
    m_input = i;
    Q_EMIT inputItemChanged();
    scheduleRecompute();
}

void AutoFill::insert(const QString &suggestion)
{
    if (m_suggestions.contains(suggestion))
        return;
    m_suggestions.append(suggestion);
    Q_EMIT suggestionsChanged();
}

void AutoFill::remove(const QString &suggestion)
{
    if (!m_suggestions.removeOne(suggestion))
        return;
    Q_EMIT suggestionsChanged();
    Q_EMIT suggestionRemoved(suggestion);
}

void AutoFill::save()
{
    // TODO: connect to persistence backend
    persist();
}

AutoFill *AutoFill::qmlAttachedProperties(QObject *object)
{
    return new AutoFill(object);
}

void AutoFill::scheduleRecompute()
{
    if (!m_debounce.isActive()) {
        m_debounce.start(200, this);
    }
}

void AutoFill::timerEvent(QTimerEvent *ev)
{
    if (ev->timerId() == m_debounce.timerId()) {
        m_debounce.stop();
        recomputeSuggestions();
        return;
    }
    QObject::timerEvent(ev);
}

void AutoFill::recomputeSuggestions()
{
    // For now, just clamp to maximum and ensure uniqueness
    QStringList unique = m_suggestions;
    unique.removeDuplicates();
    if (unique.size() > m_max)
        unique = unique.mid(0, m_max);
    if (unique != m_suggestions) {
        m_suggestions = unique;
        Q_EMIT suggestionsChanged();
    }
}

void AutoFill::load()
{
    // TODO: load from settings store keyed by m_key
}

void AutoFill::persist()
{
    // TODO: write to settings store keyed by m_key
}
