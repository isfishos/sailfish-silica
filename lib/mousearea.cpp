#include "mousearea.h"
#include "mousearea_p.h"

namespace Silica {

MouseArea::MouseArea(QQuickItem *parent)
    : QQuickMouseArea(parent)
    , d_ptr(new MouseAreaPrivate(this))
{
}

MouseArea::~MouseArea() = default;

bool MouseArea::isHighlighted() const
{
    Q_D(const MouseArea);
    return d->m_highlighted;
}

void MouseArea::setHighlighted(bool highlighted)
{
    Q_D(MouseArea);
    if (d->m_highlighted != highlighted) {
        d->m_highlighted = highlighted;
        emit highlightedChanged();
    }
}

void MouseArea::resetHighlighted()
{
    setHighlighted(false);
}

Silica::Palette *MouseArea::palette() const
{
    Q_D(const MouseArea);
    return const_cast<Palette*>(&d->m_palette);
}

void MouseArea::itemChange(ItemChange change, const ItemChangeData &value)
{
    QQuickMouseArea::itemChange(change, value);
}

MouseAreaPrivate::MouseAreaPrivate(MouseArea *q)
    : ControlPrivate(nullptr)
    , m_highlighted(false)
{
    q_ptr = q;
}

MouseAreaPrivate::~MouseAreaPrivate() = default;

void MouseAreaPrivate::highlightedChanged()
{
    // Signal is emitted from the public class
}

} // namespace Silica
