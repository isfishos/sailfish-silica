// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_ROUNDEDWINDOWCORNERS_H
#define SAILFISH_SILICA_PLUGIN_ROUNDEDWINDOWCORNERS_H

#include <QQuickItem>
#include <QSGNode>

class RoundedWindowCorners : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(Corners corners READ corners WRITE setCorners NOTIFY cornersChanged)

public:
    enum Corner {
        TopLeft = 0x01,
        TopRight = 0x02,
        BottomRight = 0x04,
        BottomLeft = 0x08
    };
    Q_ENUM(Corner)
    Q_DECLARE_FLAGS(Corners, Corner)

    explicit RoundedWindowCorners(QQuickItem *parent = nullptr);

    qreal radius() const { return m_radius; }
    void setRadius(qreal radius);
    Corners corners() const { return m_corners; }
    void setCorners(Corners corners);

Q_SIGNALS:
    void radiusChanged();
    void cornersChanged();

protected:
    void itemChange(ItemChange change, const ItemChangeData &value) override;
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
    void updateWindowAssociation();

    qreal m_radius = 0.0;
    Corners m_corners = Corners(TopLeft | TopRight | BottomRight | BottomLeft);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(RoundedWindowCorners::Corners)

#endif // SAILFISH_SILICA_PLUGIN_ROUNDEDWINDOWCORNERS_H
