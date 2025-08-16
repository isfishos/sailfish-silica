#include "highlightimagebase.h"
#include "silicatheme.h"
#include "silicapalette.h"
#include "silicapalette.h"

namespace Silica {

HighlightImageBase::HighlightImageBase(QQuickItem *parent)
    : QQuickImage(parent)
    , m_highlighted(false)
    , m_color(Qt::black)
    , m_highlightColor(Qt::white)
    , m_colorWeight(1.0)
    , m_monochromeWeight(0.0)
    , m_palette(new Palette(this))
{
}

Silica::Palette *HighlightImageBase::palette() const
{
    return m_palette;
}

bool HighlightImageBase::highlighted() const
{
    return m_highlighted;
}

QColor HighlightImageBase::color() const
{
    return m_color;
}

QColor HighlightImageBase::highlightColor() const
{
    return m_highlightColor;
}

double HighlightImageBase::colorWeight() const
{
    return m_colorWeight;
}

double HighlightImageBase::monochromeWeight() const
{
    return m_monochromeWeight;
}

void HighlightImageBase::setHighlighted(bool highlighted)
{
    if (m_highlighted != highlighted) {
        m_highlighted = highlighted;
        emit highlightedChanged();
    }
}

void HighlightImageBase::setColor(const QColor &color)
{
    if (m_color != color) {
        m_color = color;
        emit colorChanged();
    }
}

void HighlightImageBase::setHighlightColor(const QColor &highlightColor)
{
    if (m_highlightColor != highlightColor) {
        m_highlightColor = highlightColor;
        emit highlightColorChanged();
    }
}

void HighlightImageBase::setColorWeight(double weight)
{
    if (m_colorWeight != weight) {
        m_colorWeight = weight;
        emit colorWeightChanged();
    }
}

void HighlightImageBase::setMonochromeWeight(double weight)
{
    if (m_monochromeWeight != weight) {
        m_monochromeWeight = weight;
        emit monochromeWeightChanged();
    }
}

} // namespace Silica
