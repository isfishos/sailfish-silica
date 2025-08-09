// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativecoveractionarea.h"
#include <QQuickItem>

DeclarativeCoverActionArea::DeclarativeCoverActionArea(QQuickItem *parent)
    : QQuickItem(parent)
{
}

void DeclarativeCoverActionArea::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    if (newGeometry != oldGeometry) {
        updateLayout();
    }
}

void DeclarativeCoverActionArea::componentComplete()
{
    QQuickItem::componentComplete();
    updateLayout();
}

void DeclarativeCoverActionArea::updateLayout()
{
    // Position children as needed for cover action layout
    // For now, just ensure the action area fills the parent cover
    if (parentItem()) {
        setSize(parentItem()->size());
    }
}
