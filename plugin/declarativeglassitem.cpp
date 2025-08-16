// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativeglassitem.h"

DeclarativeGlassItem::DeclarativeGlassItem(QQuickItem *parent)
    : Silica::Item(parent)
{
}

void DeclarativeGlassItem::setColor(const QColor &c)
{
    if (m_color == c) return;
    m_color = c; emit colorChanged();
}

void DeclarativeGlassItem::setBackgroundColor(const QColor &c)
{
    if (m_backgroundColor == c) return;
    m_backgroundColor = c; emit backgroundColorChanged();
}

void DeclarativeGlassItem::setPattern(const QUrl &p)
{
    if (m_pattern == p) return;
    m_pattern = p; emit patternChanged();
}

void DeclarativeGlassItem::setDimmed(bool d)
{
    if (m_dimmed == d) return;
    m_dimmed = d; emit dimmedChanged();
}

void DeclarativeGlassItem::setCache(bool c)
{
    if (m_cache == c) return;
    m_cache = c; emit cacheChanged();
}

void DeclarativeGlassItem::setRadius(qreal r)
{
    if (qFuzzyCompare(m_radius, r)) return;
    m_radius = r; emit radiusChanged();
}

void DeclarativeGlassItem::setFalloffRadius(qreal r)
{
    if (qFuzzyCompare(m_falloffRadius, r)) return;
    m_falloffRadius = r; emit falloffRadiusChanged();
}

void DeclarativeGlassItem::setRatio(qreal r)
{
    if (qFuzzyCompare(m_ratio, r)) return;
    m_ratio = r; emit ratioChanged();
}

void DeclarativeGlassItem::setDashed(bool d)
{
    if (m_dashed == d) return;
    m_dashed = d; emit dashedChanged();
}

void DeclarativeGlassItem::setDashOffset(qreal v)
{
    if (qFuzzyCompare(m_dashOffset, v)) return;
    m_dashOffset = v; emit dashOffsetChanged();
}

void DeclarativeGlassItem::setDashLength(qreal v)
{
    if (qFuzzyCompare(m_dashLength, v)) return;
    m_dashLength = v; emit dashLengthChanged();
}

void DeclarativeGlassItem::setDashMargin(qreal v)
{
    if (qFuzzyCompare(m_dashMargin, v)) return;
    m_dashMargin = v; emit dashMarginChanged();
}

void DeclarativeGlassItem::setBrightness(qreal v)
{
    if (qFuzzyCompare(m_brightness, v)) return;
    m_brightness = v; emit brightnessChanged();
}
