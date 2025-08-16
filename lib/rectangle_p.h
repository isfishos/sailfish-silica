#ifndef SILICA_RECTANGLE_H
#define SILICA_RECTANGLE_H

#include <private/qquickrectangle_p.h>
#include <silicapalette.h>
#include <silicaitem_p.h>

namespace Silica {

class SAILFISH_SILICA_EXPORT Rectangle : public QQuickRectangle, public AbstractItemPrivate
{
    Q_OBJECT
    Q_PROPERTY(Silica::Palette *palette READ palette CONSTANT)
    Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted RESET resetHighlighted NOTIFY highlightedChanged)

public:
    explicit Rectangle(QQuickItem *parent = nullptr);
    ~Rectangle();

    Silica::Palette *palette() const { return const_cast<Palette*>(&m_palette); }

    using AbstractItemPrivate::isHighlighted;
    using AbstractItemPrivate::setHighlighted;
    void resetHighlighted() { setHighlighted(false); }

Q_SIGNALS:
    void highlightedChanged();

protected:
    void itemChange(ItemChange change, const ItemChangeData &value) override;
};

} // namespace Silica

#endif
