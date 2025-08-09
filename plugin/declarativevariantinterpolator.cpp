// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativevariantinterpolator.h"
#include <QVariantAnimation>
#include <QMetaType>

DeclarativeVariantInterpolator::DeclarativeVariantInterpolator(QObject *parent)
    : QObject(parent)
{
}

void DeclarativeVariantInterpolator::setFrom(const QVariant &from)
{
    if (m_from != from) {
        m_from = from;
        updateValue();
        emit fromChanged();
    }
}

void DeclarativeVariantInterpolator::setTo(const QVariant &to)
{
    if (m_to != to) {
        m_to = to;
        updateValue();
        emit toChanged();
    }
}

void DeclarativeVariantInterpolator::setProgress(qreal progress)
{
    // Clamp progress to [0,1]
    progress = qBound(0.0, progress, 1.0);

    if (m_progress != progress) {
        m_progress = progress;
        updateValue();
        emit progressChanged();
    }
}

void DeclarativeVariantInterpolator::updateValue()
{
    QVariant newValue = interpolate(m_from, m_to, m_progress);

    if (m_value != newValue) {
        m_value = newValue;
        emit valueChanged();
    }
}

QVariant DeclarativeVariantInterpolator::interpolate(const QVariant &from, const QVariant &to, qreal progress)
{
    if (!from.isValid() || !to.isValid()) {
        return to;
    }

    // Try to use QVariantAnimation's interpolator first
    QVariantAnimation animation;
    animation.setStartValue(from);
    animation.setEndValue(to);
    animation.setCurrentTime(progress * animation.totalDuration());

    QVariant interpolated = animation.currentValue();
    if (interpolated.isValid()) {
        return interpolated;
    }

    // Fallback interpolation for specific types
    if (from.type() == to.type()) {
        switch (from.type()) {
            case QVariant::Int:
            case QVariant::Double: {
                qreal fromVal = from.toReal();
                qreal toVal = to.toReal();
                return fromVal + (toVal - fromVal) * progress;
            }

            case QVariant::Color: {
                QColor fromColor = from.value<QColor>();
                QColor toColor = to.value<QColor>();

                return QColor(
                    fromColor.red() + (toColor.red() - fromColor.red()) * progress,
                    fromColor.green() + (toColor.green() - fromColor.green()) * progress,
                    fromColor.blue() + (toColor.blue() - fromColor.blue()) * progress,
                    fromColor.alpha() + (toColor.alpha() - fromColor.alpha()) * progress
                );
            }

            case QVariant::PointF: {
                QPointF fromPoint = from.toPointF();
                QPointF toPoint = to.toPointF();

                return QPointF(
                    fromPoint.x() + (toPoint.x() - fromPoint.x()) * progress,
                    fromPoint.y() + (toPoint.y() - fromPoint.y()) * progress
                );
            }

            case QVariant::SizeF: {
                QSizeF fromSize = from.toSizeF();
                QSizeF toSize = to.toSizeF();

                return QSizeF(
                    fromSize.width() + (toSize.width() - fromSize.width()) * progress,
                    fromSize.height() + (toSize.height() - fromSize.height()) * progress
                );
            }

            case QVariant::RectF: {
                QRectF fromRect = from.toRectF();
                QRectF toRect = to.toRectF();

                return QRectF(
                    fromRect.x() + (toRect.x() - fromRect.x()) * progress,
                    fromRect.y() + (toRect.y() - fromRect.y()) * progress,
                    fromRect.width() + (toRect.width() - fromRect.width()) * progress,
                    fromRect.height() + (toRect.height() - fromRect.height()) * progress
                );
            }

            default:
                break;
        }
    }

    // Default fallback: choose 'to' when progress >= 0.5
    return progress >= 0.5 ? to : from;
}
