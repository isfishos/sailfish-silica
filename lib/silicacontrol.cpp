#include "silicacontrol.h"
#include "silicacontrol_p.h"

namespace Silica {

ControlPrivate::ControlPrivate(Item *q)
    : ItemPrivate(q)
{
}

ControlPrivate::~ControlPrivate()
{
    // Clear all child references to this control
    for (AbstractItemPrivate* child : children) {
        child->m_control = nullptr;
    }
}

void ControlPrivate::highlightedChanged()
{
    ItemPrivate::highlightedChanged();
    propagateHighlight();
}

void ControlPrivate::propagateHighlight()
{
    for (AbstractItemPrivate* child : children) {
        if (!child->m_explicitHighlighted) {
            child->setHighlighted(isHighlighted(), false);
        }
    }
}

Control::Control(QQuickItem *parent)
    : Item(*new ControlPrivate(this), parent)
{
}

Control::Control(ControlPrivate &dd, QQuickItem *parent)
    : Item(dd, parent)
{
}

Control::~Control()
{
}

} // namespace Silica
