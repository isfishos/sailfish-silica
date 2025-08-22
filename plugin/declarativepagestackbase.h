// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTACKBASE_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTACKBASE_H

#include <QQuickItem>
#include <QPointF>
#include <QSet>
#include <silicacontrol.h>

class DeclarativePageStackBase : public Silica::Control
{
    Q_OBJECT
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(int depth READ depth WRITE setDepth NOTIFY depthChanged)
    Q_PROPERTY(bool backNavigation READ backNavigation WRITE setBackNavigation NOTIFY backNavigationChanged)
    Q_PROPERTY(bool forwardNavigation READ forwardNavigation WRITE setForwardNavigation NOTIFY forwardNavigationChanged)
    Q_PROPERTY(bool horizontalNavigationStyle READ horizontalNavigationStyle NOTIFY horizontalNavigationStyleChanged)
    Q_PROPERTY(int navigationStyle READ navigationStyle WRITE setNavigationStyle NOTIFY navigationStyleChanged)
    Q_PROPERTY(qreal _leftFlickDifference READ leftFlickDifference NOTIFY leftFlickDifferenceChanged)
    Q_PROPERTY(qreal _rightFlickDifference READ rightFlickDifference NOTIFY rightFlickDifferenceChanged)
    Q_PROPERTY(qreal _upFlickDifference READ upFlickDifference NOTIFY upFlickDifferenceChanged)
    Q_PROPERTY(qreal _downFlickDifference READ downFlickDifference NOTIFY downFlickDifferenceChanged)
    Q_PROPERTY(int _ongoingTransitionCount READ ongoingTransitionCount WRITE setOngoingTransitionCount NOTIFY ongoingTransitionCountChanged)
    Q_PROPERTY(QQuickItem* _currentContainer READ currentContainer WRITE setCurrentContainer NOTIFY currentContainerChanged)
    Q_PROPERTY(QQuickItem* currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(bool _noGrabbing READ noGrabbing WRITE setNoGrabbing NOTIFY noGrabbingChanged)

public:
    explicit DeclarativePageStackBase(QQuickItem *parent = nullptr);

    bool isPressed() const { return m_pressed; }
    bool busy() const { return m_busy; }
    int depth() const { return m_depth; }
    void setDepth(int depth);
    bool backNavigation() const { return m_backNavigation; }
    void setBackNavigation(bool enabled);
    bool forwardNavigation() const { return m_forwardNavigation; }
    void setForwardNavigation(bool enabled);
    bool horizontalNavigationStyle() const { return m_navigationStyle == 0; } // 0 = Horizontal
    int navigationStyle() const { return m_navigationStyle; }
    void setNavigationStyle(int style);
    qreal leftFlickDifference() const { return m_leftFlickDifference; }
    qreal rightFlickDifference() const { return m_rightFlickDifference; }
    qreal upFlickDifference() const { return m_upFlickDifference; }
    qreal downFlickDifference() const { return m_downFlickDifference; }
    int ongoingTransitionCount() const { return m_ongoingTransitionCount; }
    void setOngoingTransitionCount(int c) {
        if (m_ongoingTransitionCount != c) {
            m_ongoingTransitionCount = c;
            emit ongoingTransitionCountChanged();
            const bool newBusy = (m_ongoingTransitionCount > 0);
            if (m_busy != newBusy) {
                m_busy = newBusy;
                emit busyChanged();
            }
        }
    }
    QQuickItem* currentContainer() const { return m_currentContainer; }
    void setCurrentContainer(QQuickItem *container);
    QQuickItem* currentPage() const { return m_currentPage; }
    void setCurrentPage(QQuickItem *page);
    bool noGrabbing() const { return m_noGrabbing; }
    void setNoGrabbing(bool noGrabbing);

    Q_INVOKABLE void handlePress(const QPointF &pos);
    Q_INVOKABLE void handleMove(const QPointF &pos);
    Q_INVOKABLE void handleRelease();
    Q_INVOKABLE QString resolveImportPage(const QString &page);
    Q_INVOKABLE void _grabMouse();

Q_SIGNALS:
    void pressedChanged();
    void pressed(); // Signal emitted when the item is first pressed
    void busyChanged();
    void depthChanged();
    void backNavigationChanged();
    void forwardNavigationChanged();
    void horizontalNavigationStyleChanged();
    void navigationStyleChanged();
    void leftFlickDifferenceChanged();
    void rightFlickDifferenceChanged();
    void upFlickDifferenceChanged();
    void downFlickDifferenceChanged();
    void ongoingTransitionCountChanged();
    void currentContainerChanged();
    void currentPageChanged();
    void noGrabbingChanged();
    void released();
    void canceled();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void updateFlickDifferences(const QPointF &delta);
    void checkNavigationThreshold();
    void incrementTransitionCount();
    void decrementTransitionCount();

    bool m_pressed = false;
    bool m_busy = false;
    int m_depth = 0;
    bool m_backNavigation = true;
    bool m_forwardNavigation = true;
    int m_navigationStyle = 0; // 0 = Horizontal, 1 = Vertical
    qreal m_leftFlickDifference = 0.0;
    qreal m_rightFlickDifference = 0.0;
    qreal m_upFlickDifference = 0.0;
    qreal m_downFlickDifference = 0.0;
    int m_ongoingTransitionCount = 0;
    QQuickItem *m_currentContainer = nullptr;
    QQuickItem *m_currentPage = nullptr;
    bool m_noGrabbing = false;

    QPointF m_pressPos;
    QPointF m_lastPos;
    QSet<QString> m_visitedPages;
    static constexpr qreal NAVIGATION_THRESHOLD = 50.0;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTACKBASE_H
