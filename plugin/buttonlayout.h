// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_BUTTONLAYOUT_H
#define SAILFISH_SILICA_PLUGIN_BUTTONLAYOUT_H

#include <QQuickItem>

class ButtonLayoutAttached;

class ButtonLayout : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal rowSpacing READ rowSpacing WRITE setRowSpacing NOTIFY rowSpacingChanged)
    Q_PROPERTY(qreal columnSpacing READ columnSpacing WRITE setColumnSpacing NOTIFY columnSpacingChanged)
    Q_PROPERTY(qreal preferredWidth READ preferredWidth WRITE setPreferredWidth NOTIFY preferredWidthChanged)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    explicit ButtonLayout(QQuickItem *parent = nullptr);

    qreal rowSpacing() const { return m_rowSpacing; }
    void setRowSpacing(qreal s);
    qreal columnSpacing() const { return m_colSpacing; }
    void setColumnSpacing(qreal s);
    qreal preferredWidth() const { return m_preferredWidth; }
    void setPreferredWidth(qreal w);

    static ButtonLayoutAttached *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void rowSpacingChanged();
    void columnSpacingChanged();
    void preferredWidthChanged();

protected:
    void componentComplete() override;
    void itemChange(ItemChange change, const ItemChangeData &data) override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void updatePolish() override;

private:
    void scheduleLayout();
    void relayout();
    qreal m_rowSpacing = 0;
    qreal m_colSpacing = 0;
    qreal m_preferredWidth = 0;
};

class ButtonLayoutAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool newLine READ newLine WRITE setNewLine NOTIFY newLineChanged)
public:
    explicit ButtonLayoutAttached(QObject *parent = nullptr) : QObject(parent) {}
    bool newLine() const { return m_newLine; }
    void setNewLine(bool v) { if (m_newLine==v) return; m_newLine=v; emit newLineChanged(); }
Q_SIGNALS:
    void newLineChanged();
private:
    bool m_newLine = false;
};

QML_DECLARE_TYPEINFO(ButtonLayout, QML_HAS_ATTACHED_PROPERTIES)

#endif // SAILFISH_SILICA_PLUGIN_BUTTONLAYOUT_H
