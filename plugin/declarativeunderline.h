// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEUNDERLINE_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEUNDERLINE_H

#include <QQuickItem>
#include <QSGNode>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

class DeclarativeUnderline : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int horizontalAlignment READ horizontalAlignment WRITE setHorizontalAlignment NOTIFY horizontalAlignmentChanged)
    Q_PROPERTY(QColor primaryColor READ primaryColor WRITE setPrimaryColor NOTIFY primaryColorChanged)
    Q_PROPERTY(QColor secondaryColor READ secondaryColor WRITE setSecondaryColor NOTIFY secondaryColorChanged)

public:
    explicit DeclarativeUnderline(QQuickItem *parent = nullptr);

    int horizontalAlignment() const { return m_horizontalAlignment; }
    void setHorizontalAlignment(int alignment);
    QColor primaryColor() const { return m_primaryColor; }
    void setPrimaryColor(const QColor &color);
    QColor secondaryColor() const { return m_secondaryColor; }
    void setSecondaryColor(const QColor &color);

Q_SIGNALS:
    void horizontalAlignmentChanged();
    void primaryColorChanged();
    void secondaryColorChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
    void updateGeometry(QSGGeometryNode *node);
    QColor colorForPosition(qreal position) const;

    int m_horizontalAlignment = Qt::AlignLeft;
    QColor m_primaryColor = Qt::black;
    QColor m_secondaryColor = Qt::transparent;
    bool m_geometryDirty = true;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEUNDERLINE_H
