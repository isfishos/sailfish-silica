// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativepreedittext.h"
#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlContext>

DeclarativePreeditText::DeclarativePreeditText(QObject *parent)
    : QObject(parent)
{
}

void DeclarativePreeditText::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;
        updatePreeditText();
        emit textChanged();
    }
}

void DeclarativePreeditText::componentComplete()
{
    detectParentEditor();
}

void DeclarativePreeditText::detectParentEditor()
{
    // Find the parent editor by walking up the parent chain
    QObject *current = parent();
    while (current) {
        if (QQuickItem *item = qobject_cast<QQuickItem*>(current)) {
            // Check if this item is an editor (has text property or specific object name)
            if (item->property("text").isValid() || 
                item->objectName().contains("TextInput") ||
                item->objectName().contains("TextEdit")) {
                m_parentEditor = item;
                break;
            }
        }
        current = current->parent();
    }

    if (m_parentEditor) {
        // Connect to focus changes
        connect(m_parentEditor, SIGNAL(focusChanged()), this, SLOT(handleFocusChanged()));
        updatePreeditText();
    }
}

void DeclarativePreeditText::handleFocusChanged()
{
    if (!m_parentEditor) {
        return;
    }

    bool hasFocus = m_parentEditor->property("focus").toBool();

    if (!hasFocus) {
        // Clear preedit when focus is lost
        if (!m_recursionGuard) {
            m_recursionGuard = true;
            m_parentEditor->setProperty("preeditText", "");
            m_recursionGuard = false;
        }
    } else {
        // Update preedit when focus is gained
        updatePreeditText();
    }
}

void DeclarativePreeditText::updatePreeditText()
{
    if (!m_parentEditor || m_recursionGuard) {
        return;
    }

    m_recursionGuard = true;

    // Set the preedit text on the parent editor
    m_parentEditor->setProperty("preeditText", m_text);

    // If the editor has a text property, merge the preedit text
    if (m_parentEditor->property("text").isValid()) {
        QString currentText = m_parentEditor->property("text").toString();
        QString displayText = currentText + m_text;
        m_parentEditor->setProperty("text", displayText);
    }

    m_recursionGuard = false;
}
