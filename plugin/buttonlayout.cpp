// SPDX-License-Identifier: LGPL-2.1-only

#include "buttonlayout.h"

ButtonLayout::ButtonLayout(QQuickItem *parent)
    : QQuickItem(parent)
{
}

void ButtonLayout::setRowSpacing(qreal s) { if (m_rowSpacing==s) return; m_rowSpacing=s; emit rowSpacingChanged(); scheduleLayout(); }
void ButtonLayout::setColumnSpacing(qreal s) { if (m_colSpacing==s) return; m_colSpacing=s; emit columnSpacingChanged(); scheduleLayout(); }
void ButtonLayout::setPreferredWidth(qreal w) { if (m_preferredWidth==w) return; m_preferredWidth=w; emit preferredWidthChanged(); scheduleLayout(); }

ButtonLayoutAttached *ButtonLayout::qmlAttachedProperties(QObject *object)
{ return new ButtonLayoutAttached(object); }

void ButtonLayout::componentComplete()
{ QQuickItem::componentComplete(); scheduleLayout(); }

void ButtonLayout::itemChange(ItemChange change, const ItemChangeData &data)
{ QQuickItem::itemChange(change, data); if (change==ItemChildAddedChange || change==ItemChildRemovedChange || change==ItemSceneChange) scheduleLayout(); }

void ButtonLayout::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{ QQuickItem::geometryChanged(newGeometry, oldGeometry); if (newGeometry.size()!=oldGeometry.size()) scheduleLayout(); }

void ButtonLayout::updatePolish()
{ relayout(); }

void ButtonLayout::scheduleLayout()
{ polish(); }

void ButtonLayout::relayout()
{
    const qreal maxWidth = m_preferredWidth > 0 ? m_preferredWidth : width();
    qreal x = 0, y = 0, rowHeight = 0;
    for (auto *child : childItems()) {
        if (!child->isVisible()) continue;
        const qreal cw = child->implicitWidth();
        const qreal ch = child->implicitHeight();
        if (x > 0 && x + cw > maxWidth) { x = 0; y += rowHeight + m_rowSpacing; rowHeight = 0; }
        child->setX(x);
        child->setY(y);
        x += cw + m_colSpacing;
        rowHeight = qMax(rowHeight, ch);
    }
    setImplicitHeight(y + rowHeight);
}
