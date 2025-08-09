// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEVISIBILITYCULL_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEVISIBILITYCULL_H

#include <QQuickItem>
#include <QSet>
#include <QTimer>

class DeclarativeVisibilityCull : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit DeclarativeVisibilityCull(QQuickItem *parent = nullptr);

    QQuickItem* target() const { return m_target; }
    void setTarget(QQuickItem *target);
    bool enabled() const { return m_enabled; }
    void setEnabled(bool enabled);

Q_SIGNALS:
    void targetChanged();
    void enabledChanged();

protected:
    void itemChange(ItemChange change, const ItemChangeData &value) override;

private Q_SLOTS:
    void sync();

private:
    void updateVisibility();
    void restoreHiddenItems();

    QQuickItem *m_target = nullptr;
    bool m_enabled = false;
    QSet<QQuickItem*> m_hiddenItems;
    QTimer m_syncTimer;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEVISIBILITYCULL_H
