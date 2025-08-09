// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativepagestackbase.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QQmlEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <QDir>

DeclarativePageStackBase::DeclarativePageStackBase(QQuickItem *parent)
    : QQuickItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setFocus(true);
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

void DeclarativePageStackBase::setNavigationStyle(int style)
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

void DeclarativePageStackBase::handlePress(const QPointF &pos)
{
    m_pressPos = pos;
    m_lastPos = pos;
    m_pressed = true;
    emit pressedChanged();
}

void DeclarativePageStackBase::handleMove(const QPointF &pos)
{
    if (!m_pressed) {
        return;
    }

    QPointF delta = pos - m_lastPos;
    updateFlickDifferences(delta);
    m_lastPos = pos;

    checkNavigationThreshold();
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

    // Check if we've already visited this page to prevent import cycles
    if (m_visitedPages.contains(page)) {
        return QString();
    }

    m_visitedPages.insert(page);

    // TODO: Implement proper import path resolution using DeclarativeStandardPaths
    // For now, return a simple path
    return page;
}

void DeclarativePageStackBase::_grabMouse()
{
    grabMouse();
}

void DeclarativePageStackBase::mousePressEvent(QMouseEvent *event)
{
    if (!m_noGrabbing) {
        handlePress(event->pos());
        event->accept();
    } else {
        QQuickItem::mousePressEvent(event);
    }
}

void DeclarativePageStackBase::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_noGrabbing && m_pressed) {
        handleMove(event->pos());
        event->accept();
    } else {
        QQuickItem::mouseMoveEvent(event);
    }
}

void DeclarativePageStackBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_noGrabbing) {
        handleRelease();
        event->accept();
    } else {
        QQuickItem::mouseReleaseEvent(event);
    }
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
    if (m_navigationStyle == 0) { // Horizontal
        if (delta.x() > 0) {
            m_rightFlickDifference += delta.x();
            emit rightFlickDifferenceChanged();
        } else {
            m_leftFlickDifference += qAbs(delta.x());
            emit leftFlickDifferenceChanged();
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

    if (m_navigationStyle == 0) { // Horizontal
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
        m_busy = true;
        emit busyChanged();
        incrementTransitionCount();
    }
}

void DeclarativePageStackBase::incrementTransitionCount()
{
    m_ongoingTransitionCount++;
    emit ongoingTransitionCountChanged();
}

void DeclarativePageStackBase::decrementTransitionCount()
{
    if (m_ongoingTransitionCount > 0) {
        m_ongoingTransitionCount--;
        emit ongoingTransitionCountChanged();

        if (m_ongoingTransitionCount == 0) {
            m_busy = false;
            emit busyChanged();
        }
    }
}
