#include "silicascreen.h"
#include "silicascreen_p.h"

#include <cmath>
#include <QDebug>
#include <QGlobalStatic>

Q_GLOBAL_STATIC(Silica::Screen, screenInstance)
Q_GLOBAL_STATIC(Silica::ScreenPrivate, screenPrivateInstance)

namespace Silica {

ScreenPrivate::ScreenPrivate(QObject *parent)
    : QObject(parent)
    , m_width(540)
    , m_height(960)
    , m_widthRatio(1.0)
    , m_sizeCategory(Screen::Medium)
    , m_hasCutouts(false)
    , m_topLeftCorner(new RoundedCorner(this))
    , m_topRightCorner(new RoundedCorner(this))
    , m_bottomLeftCorner(new RoundedCorner(this))
    , m_bottomRightCorner(new RoundedCorner(this))
    , m_cutoutConfiguration(new MGConfItem("/desktop/sailfish/silica/cutouts", this))
    , m_roundedCornerConfig(new MGConfItem("/desktop/sailfish/silica/rounded_corners", this))
    , m_sizeCategoryConfig(new MGConfItem("/desktop/sailfish/silica/size_category", this))
{
    // Connect configuration change signals
    connect(m_cutoutConfiguration, &MGConfItem::valueChanged,
            this, &ScreenPrivate::readCutouts);
    connect(m_roundedCornerConfig, &MGConfItem::valueChanged,
            this, &ScreenPrivate::readRoundedCorners);
    connect(m_sizeCategoryConfig, &MGConfItem::valueChanged,
            this, &ScreenPrivate::readSizeCategory);

    // Read size category from configuration
    readSizeCategory();

    // Initialize cutouts and rounded corners
    readCutouts();
    readRoundedCorners();

    // Adjust screen size based on primary screen
    if (QScreen *primaryScreen = QGuiApplication::primaryScreen()) {
        QSize size = primaryScreen->size();
        m_width = qMin(size.width(), size.height());
        m_height = qMax(size.width(), size.height());

        // Calculate physical size
        QSizeF physicalSize = primaryScreen->physicalSize();
        if (!physicalSize.isEmpty()) {
            double diagonal = sqrt(pow(physicalSize.width(), 2) +
                                pow(physicalSize.height(), 2)) / 25.4;

            if (diagonal < 1.0 || diagonal > 50.0) {
                qWarning() << "Suspicious physical size" << physicalSize << "Check your device configuration!";
            }

            // Set size category based on diagonal if not configured
            if (!m_sizeCategoryConfig->value().isValid()) {
                if (diagonal < 3.9) m_sizeCategory = Screen::Small;
                else if (diagonal < 6.9) m_sizeCategory = Screen::Medium;
                else if (diagonal < 10.5) m_sizeCategory = Screen::Large;
                else m_sizeCategory = Screen::ExtraLarge;
            }

            // Check for default DPI
            if (qAbs(primaryScreen->logicalDotsPerInch() - 100.0) < 0.1) {
                qWarning() << "Screen DPI appears to be default value (100.0). Check configuration.";
            }
        }
    }
}

void ScreenPrivate::readSizeCategory()
{
    QString category = m_sizeCategoryConfig->value().toString();
    if (!category.isEmpty()) {
        if (category == "small") m_sizeCategory = Screen::Small;
        else if (category == "medium") m_sizeCategory = Screen::Medium;
        else if (category == "large") m_sizeCategory = Screen::Large;
        else if (category == "extralarge") m_sizeCategory = Screen::ExtraLarge;
        else qWarning() << "Invalid size category in configuration:" << category;
    }
}

void ScreenPrivate::readCutouts()
{
    QVariant config = m_cutoutConfiguration->value();
    m_cutouts.clear();
    m_hasCutouts = false;

    if (config.isValid()) {
        QVariantList cutoutsList = config.toList();

        for (const QVariant &cutoutVariant : cutoutsList) {
            QVariantList rect = cutoutVariant.toList();

            if (rect.size() == 4) {
                int values[4] = {0};

                // Convert all values to integers
                for (int i = 0; i < 4; ++i) {
                    values[i] = rect.at(i).toInt();
                }

                QRect cutout(values[0], values[1], values[2], values[3]);
                if (!cutout.isEmpty()) {
                    m_cutouts.append(cutout);
                    if (cutout.y() == 0) {  // If cutout is at the top
                        m_topCutout = cutout;
                        m_hasCutouts = true;
                    }
                    continue;
                }
            }
            // Invalid cutout configuration
            qDebug() << "Invalid display cutout rectangle:" << rect;
        }
    }

    emit cutoutsChanged();
}

void ScreenPrivate::mapRelativePosition(int relativeX, int relativeY, CornerPosition pos, int *outX, int *outY)
{
    *outX = relativeX;
    *outY = relativeY;

    if (pos == CornerTopRight || pos == CornerBottomRight) {
        *outX += m_width;
    }

    if (pos == CornerBottomLeft || pos == CornerBottomRight) {
        *outY += m_height;
    }
}

void ScreenPrivate::readRoundedCorners()
{
    QVariant config = m_roundedCornerConfig->value();
    if (!config.isValid()) {
        return;
    }

    QVariantList cornersList = config.toList();
    RoundedCorner *corners[CornerCount] = {
        m_topLeftCorner,
        m_topRightCorner,
        m_bottomLeftCorner,
        m_bottomRightCorner
    };

    for (int i = 0; i < CornerCount && i < cornersList.size(); ++i) {
        QVariantList cornerConfig = cornersList[i].toList();

        if (cornerConfig.size() == 3) {
            bool ok[3] = {false};
            qreal relativeX = cornerConfig[0].toDouble(&ok[0]);
            qreal relativeY = cornerConfig[1].toDouble(&ok[1]);
            int radius = cornerConfig[2].toInt(&ok[2]);

            if (ok[0] && ok[1] && ok[2]) {
                int x, y;
                mapRelativePosition(relativeX, relativeY,
                                  static_cast<CornerPosition>(i), &x, &y);
                corners[i]->setPosition(x, y);
                corners[i]->setRadius(radius);
                continue;
            }
        }

        qWarning() << "Invalid rounded corner configuration for corner" << i << ":" << cornerConfig;
        corners[i]->setPosition(0, 0);
        corners[i]->setRadius(0);
    }

    // Clear remaining corners if config list was shorter
    for (int i = cornersList.size(); i < CornerCount; ++i) {
        corners[i]->setPosition(0, 0);
        corners[i]->setRadius(0);
    }

    emit roundedCornersChanged();
}

ScreenPrivate::~ScreenPrivate()
{
    delete m_topLeftCorner;
    delete m_topRightCorner;
    delete m_bottomLeftCorner;
    delete m_bottomRightCorner;
}

Screen* Screen::instance()
{
    return screenInstance();
}

Screen::Screen(QObject *parent)
    : QObject(parent)
    , m_private(new ScreenPrivate(this))
{
}

Screen::~Screen()
{
    delete m_private;
}

int Screen::width() const
{
    return m_private->m_width;
}

int Screen::height() const
{
    return m_private->m_height;
}

qreal Screen::widthRatio() const
{
    return m_private->m_widthRatio;
}

Screen::SizeCategory Screen::sizeCategory() const
{
    return m_private->m_sizeCategory;
}

bool Screen::hasCutouts() const
{
    return m_private->m_hasCutouts;
}

QRect Screen::topCutout() const
{
    return m_private->m_topCutout;
}

RoundedCorner* Screen::topLeftCorner()
{
    return m_private->m_topLeftCorner;
}

RoundedCorner* Screen::topRightCorner()
{
    return m_private->m_topRightCorner;
}

RoundedCorner* Screen::bottomLeftCorner()
{
    return m_private->m_bottomLeftCorner;
}

RoundedCorner* Screen::bottomRightCorner()
{
    return m_private->m_bottomRightCorner;
}

}
