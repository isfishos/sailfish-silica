// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEGLASSITEM_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEGLASSITEM_H

#include <QColor>
#include <QUrl>
#include <QQuickItem>
#include "silicaitem.h"

class DeclarativeGlassItem : public Silica::Item
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QUrl pattern READ pattern WRITE setPattern NOTIFY patternChanged)
    Q_PROPERTY(bool dimmed READ dimmed WRITE setDimmed NOTIFY dimmedChanged)
    Q_PROPERTY(bool cache READ cache WRITE setCache NOTIFY cacheChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(qreal falloffRadius READ falloffRadius WRITE setFalloffRadius NOTIFY falloffRadiusChanged)
    Q_PROPERTY(qreal ratio READ ratio WRITE setRatio NOTIFY ratioChanged)
    Q_PROPERTY(bool dashed READ dashed WRITE setDashed NOTIFY dashedChanged)
    Q_PROPERTY(qreal dashOffset READ dashOffset WRITE setDashOffset NOTIFY dashOffsetChanged)
    Q_PROPERTY(qreal dashLength READ dashLength WRITE setDashLength NOTIFY dashLengthChanged)
    Q_PROPERTY(qreal dashMargin READ dashMargin WRITE setDashMargin NOTIFY dashMarginChanged)
    Q_PROPERTY(qreal brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)

public:
    explicit DeclarativeGlassItem(QQuickItem *parent = nullptr);

    QColor color() const { return m_color; }
    void setColor(const QColor &c);

    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor &c);

    QUrl pattern() const { return m_pattern; }
    void setPattern(const QUrl &p);

    bool dimmed() const { return m_dimmed; }
    void setDimmed(bool d);

    bool cache() const { return m_cache; }
    void setCache(bool c);

    qreal radius() const { return m_radius; }
    void setRadius(qreal r);

    qreal falloffRadius() const { return m_falloffRadius; }
    void setFalloffRadius(qreal r);

    qreal ratio() const { return m_ratio; }
    void setRatio(qreal r);

    bool dashed() const { return m_dashed; }
    void setDashed(bool d);

    qreal dashOffset() const { return m_dashOffset; }
    void setDashOffset(qreal v);

    qreal dashLength() const { return m_dashLength; }
    void setDashLength(qreal v);

    qreal dashMargin() const { return m_dashMargin; }
    void setDashMargin(qreal v);

    qreal brightness() const { return m_brightness; }
    void setBrightness(qreal v);

Q_SIGNALS:
    void colorChanged();
    void backgroundColorChanged();
    void patternChanged();
    void dimmedChanged();
    void cacheChanged();
    void radiusChanged();
    void falloffRadiusChanged();
    void ratioChanged();
    void dashedChanged();
    void dashOffsetChanged();
    void dashLengthChanged();
    void dashMarginChanged();
    void brightnessChanged();

private:
    QColor m_color;
    QColor m_backgroundColor;
    QUrl m_pattern;
    bool m_dimmed = false;
    bool m_cache = false;
    qreal m_radius = 0.0;
    qreal m_falloffRadius = 0.0;
    qreal m_ratio = 0.0;
    bool m_dashed = false;
    qreal m_dashOffset = 0.0;
    qreal m_dashLength = 0.0;
    qreal m_dashMargin = 0.0;
    qreal m_brightness = 1.0;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEGLASSITEM_H
