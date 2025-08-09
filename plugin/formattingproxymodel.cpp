// SPDX-License-Identifier: LGPL-2.1-only

#include "formattingproxymodel.h"
#include "declarativeformatter.h"
#include <QAbstractItemModel>
#include <QHash>

FormattingProxyModel::FormattingProxyModel(QObject *parent)
    : QIdentityProxyModel(parent)
{
    m_formatter = new DeclarativeFormatter(this);
}

void FormattingProxyModel::setFormattedProperties(const QVariantList &properties)
{
    if (m_formattedProperties != properties) {
        m_formattedProperties = properties;
        buildLookup();
        emit formattedPropertiesChanged();
    }
}

QHash<int, QByteArray> FormattingProxyModel::roleNames() const
{
    if (sourceModel()) {
        return sourceModel()->roleNames();
    }
    return QHash<int, QByteArray>();
}

QVariant FormattingProxyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !sourceModel()) {
        return QVariant();
    }

    // Check if this role should be formatted
    if (m_roleLookup.contains(role)) {
        QVariant sourceData = sourceModel()->data(mapToSource(index), role);

        // Format the data using DeclarativeFormatter
        // For now, just return the source data as a formatted string
        // TODO: Implement proper formatting based on the role type
        return sourceData.toString();
    }

    // Return source data unchanged
    return sourceModel()->data(mapToSource(index), role);
}

void FormattingProxyModel::buildLookup()
{
    m_roleLookup.clear();

    if (!sourceModel()) {
        return;
    }

    QHash<int, QByteArray> roles = sourceModel()->roleNames();

    for (const QVariant &property : m_formattedProperties) {
        if (property.type() == QVariant::String) {
            // Look up role by name
            QString roleName = property.toString();
            for (auto it = roles.begin(); it != roles.end(); ++it) {
                if (it.value() == roleName.toUtf8()) {
                    m_roleLookup[it.key()] = 0; // Format type 0 for now
                    break;
                }
            }
        } else if (property.type() == QVariant::Int) {
            // Direct role ID
            int roleId = property.toInt();
            m_roleLookup[roleId] = 0; // Format type 0 for now
        }
    }
}
