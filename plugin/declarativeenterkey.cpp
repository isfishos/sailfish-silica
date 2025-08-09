// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativeenterkey.h"
#include <QQuickItem>
#include <QKeyEvent>
#include <QQmlEngine>
#include <QQmlContext>

DeclarativeEnterKey::DeclarativeEnterKey(QObject *parent)
    : QObject(parent)
{
    m_editor = qobject_cast<QQuickItem*>(parent);
    if (m_editor) {
        m_editor->installEventFilter(this);
        updateEditorExtension();
    }
}

void DeclarativeEnterKey::setIconSource(const QUrl &source)
{
    if (m_iconSource != source) {
        m_iconSource = source;
        updateEditorExtension();
        emit iconSourceChanged();
    }
}

void DeclarativeEnterKey::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;
        updateEditorExtension();
        emit textChanged();
    }
}

void DeclarativeEnterKey::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        updateEditorExtension();
        emit enabledChanged();
    }
}

void DeclarativeEnterKey::setHighlighted(bool highlighted)
{
    if (m_highlighted != highlighted) {
        m_highlighted = highlighted;
        updateEditorExtension();
        emit highlightedChanged();
    }
}

DeclarativeEnterKey *DeclarativeEnterKey::qmlAttachedProperties(QObject *obj)
{
    // Return one instance per target object
    static QHash<QObject*, DeclarativeEnterKey*> instances;

    if (!instances.contains(obj)) {
        instances[obj] = new DeclarativeEnterKey(obj);
    }

    return instances[obj];
}

bool DeclarativeEnterKey::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_editor && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        if (m_enabled && (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)) {
            emit clicked();
            return true; // Consume the event
        }
    }

    return false;
}

void DeclarativeEnterKey::updateEditorExtension()
{
    if (!m_editor) {
        return;
    }

    // Set extension values on the editor's context for QML to bind
    QQmlContext *context = QQmlEngine::contextForObject(m_editor);
    if (context) {
        context->setContextProperty("enterKeyIconSource", m_iconSource);
        context->setContextProperty("enterKeyText", m_text);
        context->setContextProperty("enterKeyEnabled", m_enabled);
        context->setContextProperty("enterKeyHighlighted", m_highlighted);
    }
}

QQuickItem* DeclarativeEnterKey::findEditorDelegate()
{
    // Look for visual delegate (e.g., trailing action button) in the editor
    if (!m_editor) {
        return nullptr;
    }

    // Search for child items that might be the enter key delegate
    const QList<QQuickItem*> children = m_editor->childItems();
    for (QQuickItem *child : children) {
        // Look for items with specific object names or properties that indicate they're the enter key delegate
        if (child->objectName() == "enterKeyDelegate" || 
            child->property("isEnterKeyDelegate").toBool()) {
            return child;
        }
    }

    return nullptr;
}
