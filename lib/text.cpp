#include "text_p.h"

namespace Silica {

Text::Text(QQuickItem *parent)
    : QQuickText(parent)
{
}

Text::~Text()
{
}

void Text::itemChange(ItemChange change, const ItemChangeData &value)
{
    QQuickText::itemChange(change, value);

    if ((change == ItemSceneChange && value.window) ||
        (change == ItemParentHasChanged && value.item)) {
        updateControl(this);
    }
}

} // namespace Silica
