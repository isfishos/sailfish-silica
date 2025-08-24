// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTACKBASE_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTACKBASE_H

#include <QQuickItem>
#include <QPointF>
#include <QSet>
#include <QPointer>
#include <silicacontrol.h>
#include "declarativepagenavigation.h"
#include "declarativestandardpaths.h"

class DeclarativePageStackBase : public Silica::Control
{
    Q_OBJECT
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged FINAL)
    Q_PROPERTY(int depth READ depth WRITE setDepth NOTIFY depthChanged FINAL)
    Q_PROPERTY(bool backNavigation READ backNavigation WRITE setBackNavigation NOTIFY backNavigationChanged)
    Q_PROPERTY(bool forwardNavigation READ forwardNavigation WRITE setForwardNavigation NOTIFY forwardNavigationChanged)
    Q_PROPERTY(bool horizontalNavigationStyle READ horizontalNavigationStyle NOTIFY horizontalNavigationStyleChanged)
    Q_PROPERTY(DeclarativePageNavigation::Style navigationStyle READ navigationStyle WRITE setNavigationStyle NOTIFY navigationStyleChanged)
    Q_PROPERTY(qreal _leftFlickDifference READ leftFlickDifference NOTIFY backFlickDifferenceChanged)
    Q_PROPERTY(qreal _rightFlickDifference READ rightFlickDifference NOTIFY forwardFlickDifferenceChanged)
    Q_PROPERTY(qreal _upFlickDifference READ upFlickDifference NOTIFY upFlickDifferenceChanged)
    Q_PROPERTY(qreal _downFlickDifference READ downFlickDifference NOTIFY downFlickDifferenceChanged)
    Q_PROPERTY(int _ongoingTransitionCount READ ongoingTransitionCount WRITE setOngoingTransitionCount NOTIFY ongoingTransitionCountChanged)
    Q_PROPERTY(QQuickItem* _currentContainer READ currentContainer WRITE setCurrentContainer NOTIFY currentContainerChanged)
    Q_PROPERTY(QQuickItem* currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(bool _noGrabbing READ noGrabbing WRITE setNoGrabbing NOTIFY noGrabbingChanged)

public:
    explicit DeclarativePageStackBase(QQuickItem *parent = nullptr);

    bool isPressed() const { return m_pressed; }
    bool isBusy() const { return m_ongoingTransitionCount > 0; }
    int depth() const { return m_depth; }
    void setDepth(int depth);
    bool backNavigation() const { return m_backNavigation; }
    void setBackNavigation(bool enabled);
    bool forwardNavigation() const { return m_forwardNavigation; }
    void setForwardNavigation(bool enabled);
    bool horizontalNavigationStyle() const { return m_navigationStyle == DeclarativePageNavigation::Horizontal; }
    DeclarativePageNavigation::Style navigationStyle() const { return m_navigationStyle; }
    void setNavigationStyle(DeclarativePageNavigation::Style style);
    qreal leftFlickDifference() const { return m_leftFlickDifference; }
    qreal rightFlickDifference() const { return m_rightFlickDifference; }
    qreal upFlickDifference() const { return m_upFlickDifference; }
    qreal downFlickDifference() const { return m_downFlickDifference; }
    int ongoingTransitionCount() const { return m_ongoingTransitionCount; }
    void setOngoingTransitionCount(int ongoingTransitionCount);
    QQuickItem* currentContainer() const { return m_currentContainer; }
    void setCurrentContainer(QQuickItem *container);
    QQuickItem* currentPage() const { return m_currentPage; }
    void setCurrentPage(QQuickItem *page);
    bool noGrabbing() const { return m_noGrabbing; }
    void setNoGrabbing(bool noGrabbing);

    Q_INVOKABLE bool handlePress(const QPointF &pos);
    Q_INVOKABLE bool handleMove(const QPointF &pos);
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
    void backFlickDifferenceChanged();
    void forwardFlickDifferenceChanged();
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
    void mouseUngrabEvent() override;
    bool childMouseEventFilter(QQuickItem *item, QEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    bool handleMouse(QMouseEvent *mouseEvent);
    bool isMouseGrabbed();
    void reset();
    void setLeftFlickDifference(qreal difference);
    void setRightFlickDifference(qreal difference);
    void setUpFlickDifference(qreal difference);
    void setDownFlickDifference(qreal difference);

    bool m_pressed = false;
    int m_depth = 0;
    bool m_capture = false;
    bool m_grabbed = false;
    bool m_verticalBlock = false;
    bool m_backNavigation = true;
    DeclarativePageNavigation::Style m_navigationStyle = DeclarativePageNavigation::Horizontal;
    bool m_forwardNavigation = false;
    bool m_noGrabbing = false;
    bool m_navigatingLeft = false;
    bool m_navigatingRight = false;
    bool m_navigatingUp = false;
    bool m_navigatingDown = false;
    qreal m_leftFlickDifference = 0.0;
    qreal m_rightFlickDifference = 0.0;
    qreal m_upFlickDifference = 0.0;
    qreal m_downFlickDifference = 0.0;
    int m_ongoingTransitionCount = 0;
    QPointF m_pressPos;
    QPointer<QQuickItem> m_currentContainer;
    QPointer<QQuickItem> m_currentPage;
    DeclarativeStandardPaths m_stdPaths;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTACKBASE_H
