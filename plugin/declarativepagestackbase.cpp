// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativepagestackbase.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QQmlEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <QDir>
#include <QQuickWindow>

DeclarativePageStackBase::DeclarativePageStackBase(QQuickItem *parent)
    : Silica::Control(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setFocus(true);
    setFiltersChildMouseEvents(true);
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
    m_lastPos = pos;
    m_pressed = true;
    emit pressed();
    emit pressedChanged();
    return true; // Indicate we can handle the press
}

bool DeclarativePageStackBase::handleMove(const QPointF &pos)
{
    if (!m_pressed) {
        return false;
    }

    QPointF delta = pos - m_lastPos;
    updateFlickDifferences(delta);
    m_lastPos = pos;

    checkNavigationThreshold();
    return m_grabbed; // Return true if we've grabbed the mouse
}

void DeclarativePageStackBase::handleRelease()
{
    if (m_pressed) {
        m_pressed = false;
        emit pressedChanged();
        emit released();
    }
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
    // Reset state when grab is lost
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

bool DeclarativePageStackBase::childMouseEventFilter(QQuickItem *, QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseMove:
    case QEvent::MouseButtonRelease:
        return handleMouse(static_cast<QMouseEvent *>(event));
    case QEvent::UngrabMouse:
        if (window() && window()->mouseGrabberItem() && window()->mouseGrabberItem() != this) {
            mouseUngrabEvent();
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
        return false;
    }
    if (m_currentContainer && m_currentPage && isVisible()) {
        if (QQuickItem *parent_ = parentItem()) {
            switch (mouseEvent->type()) {
            case QEvent::MouseButtonPress:
            case QEvent::MouseMove: {
                QPointF pos = parent_->mapFromScene(mouseEvent->windowPos());
                pos += m_currentContainer->position();
                pos = QQuickItem::mapToItem(m_currentPage, pos);
                if (mouseEvent->type() == QEvent::MouseButtonPress) {
                    handlePress(pos);
                } else {
                    handleMove(pos);
                    return m_grabbed;
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
    if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Back) {
        if (m_backNavigation) {
            // TODO: Implement back navigation
            emit canceled();
        }
        event->accept();
    } else {
        QQuickItem::keyReleaseEvent(event);
    }
}

void DeclarativePageStackBase::updateFlickDifferences(const QPointF &delta)
{
    if (m_navigationStyle == DeclarativePageNavigation::Horizontal) {
        if (delta.x() > 0) {
            m_rightFlickDifference += delta.x();
            emit forwardFlickDifferenceChanged();
        } else {
            m_leftFlickDifference += qAbs(delta.x());
            emit backFlickDifferenceChanged();
        }
    } else { // Vertical
        if (delta.y() > 0) {
            m_downFlickDifference += delta.y();
            emit downFlickDifferenceChanged();
        } else {
            m_upFlickDifference += qAbs(delta.y());
            emit upFlickDifferenceChanged();
        }
    }
}

void DeclarativePageStackBase::checkNavigationThreshold()
{
    bool shouldNavigate = false;

    if (m_navigationStyle == DeclarativePageNavigation::Horizontal) {
        if (m_leftFlickDifference > NAVIGATION_THRESHOLD && m_backNavigation) {
            shouldNavigate = true;
        } else if (m_rightFlickDifference > NAVIGATION_THRESHOLD && m_forwardNavigation) {
            shouldNavigate = true;
        }
    } else { // Vertical
        if (m_upFlickDifference > NAVIGATION_THRESHOLD && m_backNavigation) {
            shouldNavigate = true;
        } else if (m_downFlickDifference > NAVIGATION_THRESHOLD && m_forwardNavigation) {
            shouldNavigate = true;
        }
    }

    if (shouldNavigate && !m_busy) {
        incrementTransitionCount();
    }
}

void DeclarativePageStackBase::incrementTransitionCount()
{
    setOngoingTransitionCount(m_ongoingTransitionCount + 1);
}

void DeclarativePageStackBase::decrementTransitionCount()
{
    if (m_ongoingTransitionCount > 0) {
        setOngoingTransitionCount(m_ongoingTransitionCount - 1);
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
    return m_grabbed;
}

void DeclarativePageStackBase::reset()
{
    m_capture = false;
    m_grabbed = false;
    m_verticalBlock = false;
    if (m_pressed) {
        m_pressed = false;
        emit pressedChanged();
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
