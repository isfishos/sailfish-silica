// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_FORMATTINGPROXYMODEL_H
#define SAILFISH_SILICA_PLUGIN_FORMATTINGPROXYMODEL_H

#include <QIdentityProxyModel>
#include <QVariantList>
#include <QHash>

class DeclarativeFormatter;

class FormattingProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* sourceModel READ sourceModel WRITE setSourceModel)
    Q_PROPERTY(QVariantList formattedProperties READ formattedProperties WRITE setFormattedProperties NOTIFY formattedPropertiesChanged)

public:
    explicit FormattingProxyModel(QObject *parent = nullptr);

    QVariantList formattedProperties() const { return m_formattedProperties; }
    void setFormattedProperties(const QVariantList &properties);

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

Q_SIGNALS:
    void formattedPropertiesChanged();

private:
    void buildLookup();

    QVariantList m_formattedProperties;
    QHash<int, int> m_roleLookup; // Maps role to formatting function
    DeclarativeFormatter *m_formatter = nullptr;
};

#endif // SAILFISH_SILICA_PLUGIN_FORMATTINGPROXYMODEL_H
