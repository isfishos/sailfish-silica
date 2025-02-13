#ifndef MOUSEAREA_P_H
#define MOUSEAREA_P_H

#include <silicacontrol_p.h>
#include "mousearea.h"

namespace Silica {

class MouseAreaPrivate : public ControlPrivate {
    Q_DECLARE_PUBLIC(MouseArea)
public:
    MouseAreaPrivate(MouseArea *q);
    ~MouseAreaPrivate() override;

    static MouseAreaPrivate *get(MouseArea *mouseArea) {
        return static_cast<MouseAreaPrivate *>(mouseArea->d_func());
    }

    void highlightedChanged() override;
};

} // namespace Silica

#endif // MOUSEAREA_P_H
