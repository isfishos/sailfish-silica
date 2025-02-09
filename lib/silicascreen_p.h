#ifndef SILICA_SCREEN_P_H
#define SILICA_SCREEN_P_H

#include <silicascreen.h>
#include <QRect>
#include <MGConfItem>
#include <QScreen>
#include <QGuiApplication>
#include <QGlobalStatic>

namespace Silica {

class RoundedCornerPrivate {
public:
    int x;
    int y;
    int radius;
};

class ScreenPrivate : public QObject {
    Q_OBJECT
public:
    explicit ScreenPrivate(QObject *parent = nullptr);
    ~ScreenPrivate();

    int m_width;
    int m_height;
    qreal m_widthRatio;
    Screen::SizeCategory m_sizeCategory;
    bool m_hasCutouts;
    QRect m_topCutout;
    RoundedCorner *m_topLeftCorner;
    RoundedCorner *m_topRightCorner;
    RoundedCorner *m_bottomLeftCorner;
    RoundedCorner *m_bottomRightCorner;

    MGConfItem *m_cutoutConfiguration;
    MGConfItem *m_roundedCornerConfig;
    MGConfItem *m_sizeCategoryConfig;

    QList<QRect> m_cutouts;

private:
    enum CornerPosition {
        CornerTopLeft = 0,
        CornerTopRight,
        CornerBottomLeft,
        CornerBottomRight,
        CornerCount
    };

    void mapRelativePosition(int relativeX, int relativeY, CornerPosition pos, int *outX, int *outY);

private slots:
    void readCutouts();
    void readRoundedCorners();
    void readSizeCategory();

signals:
    void cutoutsChanged();
    void roundedCornersChanged();
};

} // namespace Silica

#endif