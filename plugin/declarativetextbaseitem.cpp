// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativetextbaseitem.h"
#include <QQuickItem>
#include <QLocale>

DeclarativeTextBaseItem::DeclarativeTextBaseItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    // Set initial alignment based on locale text direction
    QLocale locale;
    if (locale.textDirection() == Qt::RightToLeft) {
        setImplicitHorizontalAlignment(AlignRight);
    } else {
        setImplicitHorizontalAlignment(AlignLeft);
    }
}

void DeclarativeTextBaseItem::setHorizontalAlignment(int alignment)
{
    if (m_horizontalAlignment != alignment) {
        m_horizontalAlignment = alignment;
        m_explicitHorizontalAlignment = true;
        emit horizontalAlignmentChanged();
        emit explicitHorizontalAlignmentChanged();
        updateEditorProperties();
    }
}

void DeclarativeTextBaseItem::setEditor(QQuickItem *editor)
{
    if (m_editor != editor) {
        m_editor = editor;
        emit editorChanged();
        updateEditorProperties();
    }
}

void DeclarativeTextBaseItem::setKeyboardPalette(const QString &palette)
{
    if (m_keyboardPalette != palette) {
        m_keyboardPalette = palette;
        emit keyboardPaletteChanged();
        updateEditorProperties();
    }
}

void DeclarativeTextBaseItem::setImplicitHorizontalAlignment(int alignment)
{
    if (m_horizontalAlignment != alignment) {
        m_horizontalAlignment = alignment;
        m_explicitHorizontalAlignment = false;
        emit horizontalAlignmentChanged();
        emit explicitHorizontalAlignmentChanged();
        updateEditorProperties();
    }
}

void DeclarativeTextBaseItem::resetHorizontalAlignment()
{
    m_explicitHorizontalAlignment = false;
    emit explicitHorizontalAlignmentChanged();

    // Restore implicit alignment based on locale
    QLocale locale;
    if (locale.textDirection() == Qt::RightToLeft) {
        setImplicitHorizontalAlignment(AlignRight);
    } else {
        setImplicitHorizontalAlignment(AlignLeft);
    }
}

void DeclarativeTextBaseItem::updatePolish()
{
    updateEditorProperties();
}

void DeclarativeTextBaseItem::updateEditorProperties()
{
    if (!m_editor) {
        return;
    }

    // Propagate alignment to editor
    m_editor->setProperty("horizontalAlignment", m_horizontalAlignment);

    // Propagate keyboard palette to editor
    if (!m_keyboardPalette.isEmpty()) {
        m_editor->setProperty("keyboardPalette", m_keyboardPalette);
    }
}
