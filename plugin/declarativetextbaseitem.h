// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVETEXTBASEITEM_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVETEXTBASEITEM_H

#include <QQuickItem>
#include <QString>

class DeclarativeTextBaseItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int horizontalAlignment READ horizontalAlignment WRITE setHorizontalAlignment NOTIFY horizontalAlignmentChanged)
    Q_PROPERTY(bool explicitHorizontalAlignment READ explicitHorizontalAlignment NOTIFY explicitHorizontalAlignmentChanged)
    Q_PROPERTY(QQuickItem* _editor READ editor WRITE setEditor NOTIFY editorChanged)
    Q_PROPERTY(QString _keyboardPalette READ keyboardPalette WRITE setKeyboardPalette NOTIFY keyboardPaletteChanged)

public:
    enum HAlignment {
        AlignLeft = 1,
        AlignRight = 2,
        AlignHCenter = 4
    };
    Q_ENUM(HAlignment)

    explicit DeclarativeTextBaseItem(QQuickItem *parent = nullptr);

    int horizontalAlignment() const { return m_horizontalAlignment; }
    void setHorizontalAlignment(int alignment);
    bool explicitHorizontalAlignment() const { return m_explicitHorizontalAlignment; }
    QQuickItem* editor() const { return m_editor; }
    void setEditor(QQuickItem *editor);
    QString keyboardPalette() const { return m_keyboardPalette; }
    void setKeyboardPalette(const QString &palette);

    Q_INVOKABLE void setImplicitHorizontalAlignment(int alignment);
    Q_INVOKABLE void resetHorizontalAlignment();

Q_SIGNALS:
    void horizontalAlignmentChanged();
    void explicitHorizontalAlignmentChanged();
    void editorChanged();
    void keyboardPaletteChanged();

protected:
    void updatePolish() override;

private:
    void updateEditorProperties();

    int m_horizontalAlignment = AlignLeft;
    bool m_explicitHorizontalAlignment = false;
    QQuickItem *m_editor = nullptr;
    QString m_keyboardPalette;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVETEXTBASEITEM_H
