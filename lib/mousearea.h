#ifndef SILICA_MOUSEAREA_H
#define SILICA_MOUSEAREA_H

#include <silicapalette.h>
#include <private/qquickmousearea_p.h>

namespace Silica {

class MouseAreaPrivate;

class SAILFISH_SILICA_EXPORT MouseArea : public QQuickMouseArea
{
    Q_OBJECT
    Q_PROPERTY(Silica::Palette *palette READ palette CONSTANT)
    Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted RESET resetHighlighted NOTIFY highlightedChanged)

public:
    explicit MouseArea(QQuickItem *parent = nullptr);
    ~MouseArea();

    Palette *palette() const;
    bool isHighlighted() const;
    void setHighlighted(bool highlighted);
    void resetHighlighted();

Q_SIGNALS:
    void highlightedChanged();

protected:
    void itemChange(ItemChange change, const ItemChangeData &value) override;

private:
    QScopedPointer<MouseAreaPrivate> d_ptr;
    Q_DECLARE_PRIVATE(MouseArea)
};

} // namespace Silica

#endif
