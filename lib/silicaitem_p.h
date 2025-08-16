#ifndef SILICAITEM_P_H
#define SILICAITEM_P_H

#include <silicaitem.h>
#include <silicapalette.h>
#include <QQuickItem>

namespace Silica {

class ControlPrivate;
class MouseArea;

class AbstractItemPrivate {
public:
    AbstractItemPrivate();
    virtual ~AbstractItemPrivate();

    bool isHighlighted() const { return m_highlighted || m_explicitHighlighted; }
    void setHighlighted(bool value, bool isExplicit = true);
    void resetHighlighted();

    virtual void highlightedChanged() = 0;
    void updateControl(QQuickItem *item);

    ControlPrivate* findNearestControlAncestor(QQuickItem* item) const;

    Palette m_palette;
    ControlPrivate *m_control;
    bool m_highlighted = false;
    bool m_explicitHighlighted = false;
};

class ItemPrivate : public AbstractItemPrivate {
public:
    Q_DECLARE_PUBLIC(Item)

    ItemPrivate(Item *q);
    ~ItemPrivate() override;

    void highlightedChanged() override;

    QQuickItem *q_ptr = nullptr;
};

} // namespace Silica

#endif // SILICAITEM_P_H
