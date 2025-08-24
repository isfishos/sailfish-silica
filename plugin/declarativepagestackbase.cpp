// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativepagestackbase.h"
#include <QQuickWindow>
#include <QGuiApplication>
#include <QStyleHints>
#include <QCoreApplication>
#include <QMetaObject>
#include <QKeyEvent>

// Grab threshold multiplier
#ifndef PAGESTACK_THRESHOLD_MULTIPLIER
#define PAGESTACK_THRESHOLD_MULTIPLIER 1.5
#endif

DeclarativePageStackBase::DeclarativePageStackBase(QQuickItem *parent)
    : Silica::Control(parent)
{
    setFiltersChildMouseEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void DeclarativePageStackBase::setOngoingTransitionCount(int ongoingTransitionCount)
{
    if (m_ongoingTransitionCount != ongoingTransitionCount) {
        bool wasBusy = isBusy();
        m_ongoingTransitionCount = ongoingTransitionCount;
        emit ongoingTransitionCountChanged();
        if (wasBusy != isBusy()) {
            emit busyChanged();
        }
    }
}

void DeclarativePageStackBase::setDepth(int depth)
{
    if (m_depth != depth) {
        m_depth = depth;
        emit depthChanged();
    }
}

void DeclarativePageStackBase::setBackNavigation(bool enabled)
{
    if (m_backNavigation != enabled) {
        m_backNavigation = enabled;
        emit backNavigationChanged();
    }
}

void DeclarativePageStackBase::setForwardNavigation(bool enabled)
{
    if (m_forwardNavigation != enabled) {
        m_forwardNavigation = enabled;
        emit forwardNavigationChanged();
    }
}

void DeclarativePageStackBase::setNavigationStyle(DeclarativePageNavigation::Style style)
{
    if (m_navigationStyle != style) {
        m_navigationStyle = style;
        emit navigationStyleChanged();
        emit horizontalNavigationStyleChanged();
    }
}

void DeclarativePageStackBase::setCurrentContainer(QQuickItem *container)
{
    if (m_currentContainer != container) {
        m_currentContainer = container;
        emit currentContainerChanged();
    }
}

void DeclarativePageStackBase::setCurrentPage(QQuickItem *page)
{
    if (m_currentPage != page) {
        m_currentPage = page;
        emit currentPageChanged();
    }
}

void DeclarativePageStackBase::setNoGrabbing(bool noGrabbing)
{
    if (m_noGrabbing != noGrabbing) {
        m_noGrabbing = noGrabbing;
        emit noGrabbingChanged();
    }
}

bool DeclarativePageStackBase::handlePress(const QPointF &pos)
{
    m_pressPos = pos;

    if (!isMouseGrabbed() && (backNavigation() || forwardNavigation() || m_noGrabbing)) {
        m_pressed = true;
        emit pressed();
        emit pressedChanged();
    }
    return m_pressed;
}

bool DeclarativePageStackBase::handleMove(const QPointF &pos)
{
    // Only consider moves when a press was registered or when we're in non-grabbing mode
    if (m_pressed || m_noGrabbing) {
        // If some other item has the mouse and we're not in non-grabbing mode, cancel
        if (isMouseGrabbed() && !m_noGrabbing) {
            reset();
            return false;
        }

        // Detect the initial drag/capture threshold
        if (!m_capture) {
            const int threshold = PAGESTACK_THRESHOLD_MULTIPLIER * QGuiApplication::styleHints()->startDragDistance();

            qreal deltaX = pos.x() - m_pressPos.x();
            qreal deltaY = pos.y() - m_pressPos.y();

            // If vertical motion clearly dominates horizontal and we expect horizontal gestures,
            // suppress page stack activation for this gesture.
            if (qAbs(deltaY) > qAbs(deltaX) && (horizontalNavigationStyle() && qAbs(deltaY) > threshold)) {
                m_verticalBlock = true;
            }

            if (m_verticalBlock)
                return false;

            // If the move is in a direction that's currently disabled, treat it as a new press
            if ((!forwardNavigation() && deltaX < 0.0) ||
                (!backNavigation() && horizontalNavigationStyle() && deltaX > 0.0)) {
                m_pressPos = pos;
            } else if ((backNavigation() && horizontalNavigationStyle() && deltaX > threshold) ||
                       (forwardNavigation() && deltaX < -threshold)) {
                // Started a valid horizontal capture
                m_pressPos.setX(pos.x());
                m_capture = true;
            } else if ((backNavigation() && !horizontalNavigationStyle() && qAbs(deltaY) > threshold)) {
                // Started a valid vertical capture for back navigation
                m_pressPos.setY(pos.y());
                m_capture = true;
            }

            if (!m_noGrabbing)
                return m_grabbed;
        }

        // We're captured: update grab state and per-direction differences
        if (m_capture) {
            if (!m_grabbed && !m_noGrabbing)
                _grabMouse();

            qreal leftDiff = 0.0;
            qreal rightDiff = 0.0;
            qreal upDiff = 0.0;
            qreal downDiff = 0.0;

            // Back navigation contributes to left/right or up/down depending on style
            if (backNavigation()) {
                if (horizontalNavigationStyle()) {
                    if (!m_navigatingLeft)
                        m_pressPos.setX(pos.x());
                    leftDiff = qMax(qreal(0.0), pos.x() - m_pressPos.x());
                } else {
                    if (!m_navigatingUp && !m_navigatingDown)
                        m_pressPos.setY(pos.y());
                    upDiff = qMax(qreal(0.0), pos.y() - m_pressPos.y());
                    if (!m_navigatingDown)
                        m_pressPos.setY(pos.y());
                    downDiff = qMax(qreal(0.0), 0.0 - (pos.y() - m_pressPos.y()));
                }
            }

            if (forwardNavigation()) {
                if (!m_navigatingRight)
                    m_pressPos.setX(pos.x());
                rightDiff = qMax(qreal(0.0), 0.0 - (pos.x() - m_pressPos.x()));
            }

            // Update which directions are currently tracked
            m_navigatingLeft = backNavigation() && horizontalNavigationStyle();
            m_navigatingRight = forwardNavigation();
            m_navigatingUp = backNavigation() && !horizontalNavigationStyle();
            m_navigatingDown = backNavigation() && !horizontalNavigationStyle();

            // In non-grabbing mode, ensure we emit a press if it was missed
            if (m_noGrabbing && !m_pressed) {
                m_pressed = true;
                emit pressed();
                emit pressedChanged();
            }

            // Make sure zero transitions are emitted before positive changes to avoid flicker
            if ((m_rightFlickDifference > 0.0) && (rightDiff == 0.0))
                setRightFlickDifference(rightDiff);
            else if ((m_leftFlickDifference > 0.0) && (leftDiff == 0.0))
                setLeftFlickDifference(leftDiff);

            if ((m_upFlickDifference > 0.0) && (upDiff == 0.0))
                setUpFlickDifference(upDiff);
            else if ((m_downFlickDifference > 0.0) && (downDiff == 0.0))
                setDownFlickDifference(downDiff);

            // Finally publish the measured differences
            setLeftFlickDifference(leftDiff);
            setDownFlickDifference(downDiff);
            setRightFlickDifference(rightDiff);
            setUpFlickDifference(upDiff);
        }
    }

    return m_grabbed;
}

void DeclarativePageStackBase::handleRelease()
{
    if (m_pressed) {
        m_pressed = false;
        emit pressedChanged();
        emit released();

        if (m_grabbed) {
            ungrabMouse();
            setKeepMouseGrab(false);
        }
    }
    reset();
}

QString DeclarativePageStackBase::resolveImportPage(const QString &page)
{
    if (page.isEmpty()) {
        return QString();
    }

    // Use the standard paths to resolve the import
    return m_stdPaths.resolveImport(page);
}

void DeclarativePageStackBase::_grabMouse()
{
    grabMouse();
    setKeepMouseGrab(true);
    m_grabbed = true;
}

void DeclarativePageStackBase::mousePressEvent(QMouseEvent *event)
{
    handleMouse(event);
}

void DeclarativePageStackBase::mouseMoveEvent(QMouseEvent *event)
{
    handleMouse(event);
}

void DeclarativePageStackBase::mouseReleaseEvent(QMouseEvent *event)
{
    handleMouse(event);
}

void DeclarativePageStackBase::mouseUngrabEvent()
{
    reset();
}

bool DeclarativePageStackBase::childMouseEventFilter(QQuickItem *, QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseMove:
    case QEvent::MouseButtonRelease: {
        return handleMouse(static_cast<QMouseEvent *>(event));
    }
    case QEvent::UngrabMouse:
        // withdraw if something grabs the mouse first
        if (window() && window()->mouseGrabberItem() && window()->mouseGrabberItem() != this) {
            reset();
        }
        break;
    default:
        break;
    }
    return false;
}

bool DeclarativePageStackBase::handleMouse(QMouseEvent *mouseEvent)
{
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        handleRelease();
    } else if (m_currentContainer && m_currentPage && isVisible()) {
        if (QQuickItem *parent_ = parentItem()) {
            switch (mouseEvent->type()) {
            case QEvent::MouseButtonPress:
            case QEvent::MouseMove: {
                // Get the point in the pageStack coordinates
                QPointF pos = parent_->mapFromScene(mouseEvent->windowPos());

                // Offset by the current position of the container (and thus the page)
                pos += m_currentContainer->position();

                // Map into the page's coordinates
                pos = QQuickItem::mapToItem(m_currentPage, pos);

                if (mouseEvent->type() == QEvent::MouseButtonPress) {
                    handlePress(pos);
                } else {
                    return handleMove(pos);
                }
                break;
            }
            default:
                break;
            }
        }
    }
    return false;
}

void DeclarativePageStackBase::keyReleaseEvent(QKeyEvent *event)
{
    bool accept = false;
    static bool isAppWindow = qApp->platformName() == QLatin1String("wayland");
    if (!isBusy() && isAppWindow && static_cast<QKeyEvent *>(event)->key() == Qt::Key_Back) {
        if (m_depth > 1) {
            QMetaObject::invokeMethod(this, "pop", Q_ARG(QVariant, QVariant()), Q_ARG(QVariant, QVariant()));
            accept = true;
        }
    }

    if (!accept) {
        event->ignore();
    }
}

void DeclarativePageStackBase::setLeftFlickDifference(qreal difference)
{
    if (m_leftFlickDifference != difference) {
        m_leftFlickDifference = difference;
        emit backFlickDifferenceChanged();
    }
}

void DeclarativePageStackBase::setRightFlickDifference(qreal difference)
{
    if (m_rightFlickDifference != difference) {
        m_rightFlickDifference = difference;
        emit forwardFlickDifferenceChanged();
    }
}

void DeclarativePageStackBase::setUpFlickDifference(qreal difference)
{
    if (m_upFlickDifference != difference) {
        m_upFlickDifference = difference;
        emit upFlickDifferenceChanged();
    }
}

void DeclarativePageStackBase::setDownFlickDifference(qreal difference)
{
    if (m_downFlickDifference != difference) {
        m_downFlickDifference = difference;
        emit downFlickDifferenceChanged();
    }
}

bool DeclarativePageStackBase::isMouseGrabbed()
{
    if (QQuickWindow *window_ = window()) {
        // avoid flicking on top of slider and selection handles
        QQuickItem *grabber = qobject_cast<QQuickItem*>(window_->mouseGrabberItem());
        return grabber && grabber->keepMouseGrab() && grabber != this;
    }
    return false;
}

void DeclarativePageStackBase::reset()
{
    m_capture = false;
    m_grabbed = false;
    m_verticalBlock = false;
    if (m_pressed) {
        m_pressed = false;
        emit pressedChanged();
        emit canceled();
    }
    setKeepMouseGrab(false);
    m_navigatingLeft = false;
    m_navigatingRight = false;
    m_navigatingUp = false;
    m_navigatingDown = false;
    setLeftFlickDifference(0.0);
    setRightFlickDifference(0.0);
    setUpFlickDifference(0.0);
    setDownFlickDifference(0.0);
}
