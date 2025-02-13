#ifndef SAILFISH_SILICA_CONTROL_P_H
#define SAILFISH_SILICA_CONTROL_P_H

#include <silicacontrol.h>
#include <silicaitem.h>
#include <silicaitem_p.h>

namespace Silica {

class ControlPrivate : public ItemPrivate {
    Q_DECLARE_PUBLIC(Control)
public:
    ControlPrivate(Item *q);
    ~ControlPrivate() override;

    static ControlPrivate *get(Control *control) {
        return static_cast<ControlPrivate *>(control->d_func());
    }

    void highlightedChanged() override;
    void propagateHighlight();

    QList<AbstractItemPrivate*> children;
};

} // namespace Silica
#endif // SAILFISH_SILICA_CONTROL_P_H
