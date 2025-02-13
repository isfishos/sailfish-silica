#include "rectangle_p.h"

namespace Silica {

Rectangle::Rectangle(QQuickItem *parent)
    : QQuickRectangle(parent)
{
}

Rectangle::~Rectangle()
{
}

void Rectangle::itemChange(ItemChange change, const ItemChangeData &value)
{
    QQuickRectangle::itemChange(change, value);

    if ((change == ItemSceneChange && value.window) ||
        (change == ItemParentHasChanged && value.item)) {
        updateControl(this);
    }
}

} // namespace Silica
