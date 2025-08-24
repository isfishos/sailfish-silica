#include "silicaitem.h"
#include "silicaitem_p.h"
#include "silicacontrol.h"
#include "silicacontrol_p.h"
#include "silicapalette_p.h"
#include "mousearea.h"
#include "mousearea_p.h"
#include "silicabackground/fill.h"

namespace Silica {

AbstractItemPrivate::AbstractItemPrivate()
    : m_palette()
    , m_control(nullptr)
    , m_highlighted(false)
    , m_explicitHighlighted(false)
{
}

AbstractItemPrivate::~AbstractItemPrivate()
{
    // Ensure we don't leave a dangling pointer in the parent control's children list.
    // If this private object is being destroyed while still referenced by a ControlPrivate
    // instance, remove the pointer from that list so the control won't later access freed
    // memory during its own destruction.
    if (m_control) {
        m_control->children.removeAll(this);
        m_control = nullptr;
    }
}

void AbstractItemPrivate::setHighlighted(bool value, bool isExplicit)
{
    if (m_highlighted != value) {
        m_highlighted = value;
        highlightedChanged();
    }
}

void AbstractItemPrivate::resetHighlighted()
{
    if (m_control) {
        setHighlighted(m_control->isHighlighted(), false);
    } else {
        setHighlighted(false, false);
    }
}

ControlPrivate* AbstractItemPrivate::findNearestControlAncestor(QQuickItem* item) const
{
    QQuickItem* parent = item->parentItem();
    while (parent) {
        if (auto control = qobject_cast<Control*>(parent)) {
            return ControlPrivate::get(control);
        }
        if (auto mouseArea = qobject_cast<MouseArea*>(parent)) {
            return MouseAreaPrivate::get(mouseArea);
        }
        parent = parent->parentItem();
    }
    return nullptr;
}

void AbstractItemPrivate::updateControl(QQuickItem *item)
{
    ControlPrivate* control = findNearestControlAncestor(item);

    if (m_control != control) {
        m_control = control;

        if (m_control) {
            m_control->children.append(this);

            if (!m_explicitHighlighted) {
                setHighlighted(m_control->isHighlighted());
            }

            PalettePrivate::get(&m_palette)->updateParent(PalettePrivate::get(&m_control->m_palette));
        } else if (item->parentItem()) {
            PalettePrivate::get(&m_palette)->updateParent(nullptr);
        }
    }
}

ItemPrivate::ItemPrivate(Item *q)
    : q_ptr(q)
{
}

ItemPrivate::~ItemPrivate()
{
}

void ItemPrivate::highlightedChanged()
{
    Q_Q(Item);
    emit q->highlightedChanged();
}

Item::Item(QQuickItem *parent)
    : QQuickItem(parent)
    , d_ptr(new ItemPrivate(this))
{
    if (parent) {
        d_ptr->updateControl(parent);
    }
}

Item::Item(ItemPrivate &dd, QQuickItem *parent)
    : QQuickItem(parent)
    , d_ptr(&dd)
{
    d_ptr->q_ptr = this;

    if (parent) {
        d_ptr->updateControl(parent);
    }
}

Item::~Item()
{
}

Palette *Item::palette() const
{
    return &d_ptr->m_palette;
}

bool Item::isHighlighted() const
{
    return d_ptr->m_highlighted;
}

void Item::setHighlighted(bool highlighted)
{
    d_ptr->setHighlighted(highlighted, true);
}

void Item::resetHighlighted()
{
    setHighlighted(false);
}

void Item::itemChange(ItemChange change, const ItemChangeData &value)
{
    QQuickItem::itemChange(change, value);

    if ((change == ItemSceneChange && value.window) ||
        (change == ItemParentHasChanged && value.item)) {
        d_ptr->updateControl(this);
    }
}

} // namespace Silica
