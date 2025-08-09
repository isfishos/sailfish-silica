// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_TEXTLAYOUTMODEL_H
#define SAILFISH_SILICA_PLUGIN_TEXTLAYOUTMODEL_H

#include <QAbstractListModel>
#include <QFont>
#include <QTextLayout>
#include <QList>

struct TextLineInfo {
    int lineNumber;
    QString text;
    int length;
    qreal width;
    qreal height;
    bool elided;
};

class TextLayoutModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int wrapMode READ wrapMode WRITE setWrapMode NOTIFY wrapModeChanged)
    Q_PROPERTY(int maximumLineCount READ maximumLineCount WRITE setMaximumLineCount NOTIFY maximumLineCountChanged)
    Q_PROPERTY(int lineCount READ lineCount NOTIFY lineCountChanged)

public:
    enum Roles {
        LineNumberRole = Qt::UserRole + 1,
        TextRole,
        LengthRole,
        WidthRole,
        HeightRole,
        ElidedRole
    };

    explicit TextLayoutModel(QObject *parent = nullptr);

    QString text() const { return m_text; }
    void setText(const QString &text);
    QFont font() const { return m_font; }
    void setFont(const QFont &font);
    qreal width() const { return m_width; }
    void setWidth(qreal width);
    int wrapMode() const { return m_wrapMode; }
    void setWrapMode(int mode);
    int maximumLineCount() const { return m_maximumLineCount; }
    void setMaximumLineCount(int count);
    int lineCount() const { return rowCount(); }

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

Q_SIGNALS:
    void textChanged();
    void fontChanged();
    void widthChanged();
    void wrapModeChanged();
    void maximumLineCountChanged();
    void lineCountChanged();

private:
    void updateLayout();

    QString m_text;
    QFont m_font;
    qreal m_width = 0.0;
    int m_wrapMode = 0;
    int m_maximumLineCount = -1;
    QList<TextLineInfo> m_lines;
};

#endif // SAILFISH_SILICA_PLUGIN_TEXTLAYOUTMODEL_H
