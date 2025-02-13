/****************************************************************************************
**
** Copyright (C) 2019 Jolla Ltd.
** All rights reserved.
**
** License: Proprietary
**
****************************************************************************************/

#ifndef SAILFISH_SILICA_ITEM_H
#define SAILFISH_SILICA_ITEM_H

#include <silicaglobal.h>

#include <QQuickItem>

namespace Silica {

class Palette;

class ItemPrivate;
class SAILFISH_SILICA_EXPORT Item : public QQuickItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Item)
    Q_PROPERTY(Silica::Palette *palette READ palette CONSTANT)
    Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted RESET resetHighlighted NOTIFY highlightedChanged)
public:
    explicit Item(QQuickItem *parent = nullptr);
    explicit Item(ItemPrivate &dd, QQuickItem *parent = nullptr);
    ~Item();

    Silica::Palette *palette() const;

    bool isHighlighted() const;
    void setHighlighted(bool highlighted);
    void resetHighlighted();

Q_SIGNALS:
    void highlightedChanged();

protected:
    void itemChange(ItemChange change, const ItemChangeData &value) override;

    QScopedPointer<ItemPrivate> d_ptr;
};

}

#endif
