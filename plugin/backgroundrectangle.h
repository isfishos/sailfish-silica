// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_BACKGROUNDRECTANGLE_H
#define SAILFISH_SILICA_PLUGIN_BACKGROUNDRECTANGLE_H

#include <QQuickItem>
#include <QColor>

class BackgroundRectangle : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    explicit BackgroundRectangle(QQuickItem *parent = nullptr);
    QColor color() const { return m_color; }
    void setColor(const QColor &c);

Q_SIGNALS:
    void colorChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
    QColor m_color = Qt::transparent;
};

#endif // SAILFISH_SILICA_PLUGIN_BACKGROUNDRECTANGLE_H
