#ifndef SILICA_TEXT_H
#define SILICA_TEXT_H

#include <private/qquicktext_p.h>
#include <silicapalette.h>
#include <silicaitem_p.h>

namespace Silica {

class SAILFISH_SILICA_EXPORT Text : public QQuickText, public AbstractItemPrivate
{
    Q_OBJECT
    Q_PROPERTY(Silica::Palette *palette READ palette CONSTANT)
    Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted RESET resetHighlighted NOTIFY highlightedChanged)

public:
    explicit Text(QQuickItem *parent = nullptr);
    ~Text();

    Palette *palette() const { return AbstractItemPrivate::m_palette; }
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
