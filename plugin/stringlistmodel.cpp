// SPDX-License-Identifier: LGPL-2.1-only

#include "stringlistmodel.h"

StringListModel::StringListModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    updateRoleNames();
}

void StringListModel::setPropertyName(const QString &name)
{
    if (m_propertyName != name) {
        m_propertyName = name;
        updateRoleNames();
        emit propertyNameChanged();
    }
}

void StringListModel::setStrings(const QStringList &strings)
{
    if (m_strings != strings) {
        beginResetModel();
        m_strings = strings;
        endResetModel();
        emit stringsChanged();
        emit countChanged();
    }
}

QModelIndex StringListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() || column != 0 || row < 0 || row >= m_strings.size()) {
        return QModelIndex();
    }
    return createIndex(row, column);
}

QModelIndex StringListModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int StringListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_strings.size();
}

int StringListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant StringListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_strings.size() || index.column() != 0) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::UserRole) {
        return m_strings.at(index.row());
    }

    return QVariant();
}

QHash<int, QByteArray> StringListModel::roleNames() const
{
    return m_roleNames;
}

void StringListModel::updateRoleNames()
{
    m_roleNames.clear();
    m_roleNames[Qt::UserRole] = m_propertyName.toUtf8();
}
