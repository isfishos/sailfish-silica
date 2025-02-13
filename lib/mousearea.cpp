#include "silicaitem_p.h"
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
    return d->isHighlighted();
}

void MouseArea::setHighlighted(bool highlighted)
{
    Q_D(MouseArea);
    d->setHighlighted(highlighted);
}

void MouseArea::resetHighlighted()
{
    setHighlighted(false);
}

void MouseArea::itemChange(ItemChange change, const ItemChangeData &value)
{
    QQuickMouseArea::itemChange(change, value);

    if (change == ItemSceneChange && value.window) {
        Q_D(MouseArea);
        d->updateControl(this);
    }
}

} // namespace Silica
