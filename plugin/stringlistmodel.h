// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_STRINGLISTMODEL_H
#define SAILFISH_SILICA_PLUGIN_STRINGLISTMODEL_H

#include <QAbstractItemModel>
#include <QStringList>
#include <QHash>

class StringListModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString propertyName READ propertyName WRITE setPropertyName NOTIFY propertyNameChanged)
    Q_PROPERTY(QStringList strings READ strings WRITE setStrings NOTIFY stringsChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    explicit StringListModel(QObject *parent = nullptr);

    QString propertyName() const { return m_propertyName; }
    void setPropertyName(const QString &name);
    QStringList strings() const { return m_strings; }
    void setStrings(const QStringList &strings);
    int count() const { return rowCount(); }

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

Q_SIGNALS:
    void propertyNameChanged();
    void stringsChanged();
    void countChanged();

private:
    void updateRoleNames();

    QString m_propertyName = QStringLiteral("string");
    QStringList m_strings;
    QHash<int, QByteArray> m_roleNames;
};

#endif // SAILFISH_SILICA_PLUGIN_STRINGLISTMODEL_H
