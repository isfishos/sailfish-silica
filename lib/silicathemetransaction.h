// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_THEMETRANSACTION_H
#define SAILFISH_SILICA_THEMETRANSACTION_H

#include <QObject>

namespace Silica
{

class ThemeTransaction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool deferAmbience READ deferAmbience WRITE setDeferAmbience NOTIFY deferAmbienceChanged)

public:
    explicit ThemeTransaction(QObject *parent = nullptr);

    bool deferAmbience() const;
    void setDeferAmbience(bool defer);

signals:
    void ambienceAboutToChange();
    void deferAmbienceChanged();

private:
    bool m_deferAmbience = false;
};

} // namespace Silica

#endif // SAILFISH_SILICA_THEMETRANSACTION_H
