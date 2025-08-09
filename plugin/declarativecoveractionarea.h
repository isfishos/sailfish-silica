// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERACTIONAREA_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERACTIONAREA_H

#include <QQuickItem>

class DeclarativeCoverActionArea : public QQuickItem
{
    Q_OBJECT

public:
    explicit DeclarativeCoverActionArea(QQuickItem *parent = nullptr);

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void componentComplete() override;

private:
    void updateLayout();
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERACTIONAREA_H
