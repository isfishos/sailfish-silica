// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEVARIANTINTERPOLATOR_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEVARIANTINTERPOLATOR_H

#include <QObject>
#include <QVariant>
#include <QColor>
#include <QPointF>
#include <QSizeF>
#include <QRectF>

class DeclarativeVariantInterpolator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QVariant to READ to WRITE setTo NOTIFY toChanged)
    Q_PROPERTY(qreal progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)

public:
    explicit DeclarativeVariantInterpolator(QObject *parent = nullptr);

    QVariant from() const { return m_from; }
    void setFrom(const QVariant &from);
    QVariant to() const { return m_to; }
    void setTo(const QVariant &to);
    qreal progress() const { return m_progress; }
    void setProgress(qreal progress);
    QVariant value() const { return m_value; }

Q_SIGNALS:
    void fromChanged();
    void toChanged();
    void progressChanged();
    void valueChanged();

private:
    void updateValue();
    QVariant interpolate(const QVariant &from, const QVariant &to, qreal progress);

    QVariant m_from;
    QVariant m_to;
    qreal m_progress = 0.0;
    QVariant m_value;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEVARIANTINTERPOLATOR_H
