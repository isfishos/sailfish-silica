// SPDX-License-Identifier: LGPL-2.1-only

#include "pagedview.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QtMath>
#include <private/qqmldelegatemodel_p.h>

PagedView::PagedView(QQuickItem *parent)
    : Silica::Control(parent)
    , m_delegateModel(nullptr)
    , m_animation(nullptr)
    , m_flicking(false)
    , m_layoutDirty(true)
{
    connect(&m_polishTimer, &QTimer::timeout, this, &PagedView::updatePolish);
    m_polishTimer.setSingleShot(true);
}

PagedView::~PagedView()
{
    destroyContentItem();
    if (m_delegateModel) {
        delete m_delegateModel;
        m_delegateModel = nullptr;
    }
    if (m_animation) {
        delete m_animation;
    }
}

void PagedView::setModel(const QVariant &model)
{
    if (m_model != model) {
        m_model = model;
        emit modelChanged();

        if (m_delegateModel) {
            delete m_delegateModel;
            m_delegateModel = nullptr;
        }

        // Create QQmlDelegateModel if we have a valid model
        if (m_model.isValid()) {
            QQmlContext *context = qmlContext(this);
            if (context) {
                m_delegateModel = new QQmlDelegateModel(context, this);
                m_delegateModel->setModel(m_model);

                // Connect to model signals
                connect(m_delegateModel, &QQmlDelegateModel::countChanged, this, &PagedView::onModelCountChanged);

                m_count = m_delegateModel->count();
            } else {
                // Fallback for when context is not available
                if (m_model.type() == QVariant::List) {
                    m_count = m_model.toList().size();
                } else if (m_model.canConvert<int>()) {
                    m_count = m_model.toInt();
                } else {
                    m_count = 0;
                }
            }
            emit countChanged();
        } else {
            m_count = 0;
            emit countChanged();
        }

        updateLayout();
    }
}

void PagedView::setDelegate(QQmlComponent *delegate)
{
    if (m_delegate != delegate) {
        m_delegate = delegate;
        emit delegateChanged();
        updateLayout();
    }
}

void PagedView::setDirection(Direction direction)
{
    if (m_direction != direction) {
        m_direction = direction;
        emit directionChanged();
        updateLayout();
    }
}

void PagedView::setHorizontalAlignment(Qt::Alignment alignment)
{
    if (m_horizontalAlignment != alignment) {
        m_horizontalAlignment = alignment;
        emit horizontalAlignmentChanged();
        updateLayout();
    }
}

void PagedView::setVerticalAlignment(Qt::Alignment alignment)
{
    if (m_verticalAlignment != alignment) {
        m_verticalAlignment = alignment;
        emit verticalAlignmentChanged();
        updateLayout();
    }
}

void PagedView::setWrapMode(WrapMode mode)
{
    if (m_wrapMode != mode) {
        m_wrapMode = mode;
        emit wrapModeChanged();
        updateLayout();
    }
}

void PagedView::setHorizontalSpacing(qreal spacing)
{
    if (!qFuzzyCompare(m_horizontalSpacing, spacing)) {
        m_horizontalSpacing = spacing;
        emit horizontalSpacingChanged();
        updateLayout();
    }
}

void PagedView::setVerticalSpacing(qreal spacing)
{
    if (!qFuzzyCompare(m_verticalSpacing, spacing)) {
        m_verticalSpacing = spacing;
        emit verticalSpacingChanged();
        updateLayout();
    }
}

void PagedView::setCurrentIndex(int index)
{
    if (m_currentIndex != index && index >= -1 && index < m_count) {
        int oldIndex = m_currentIndex;
        m_currentIndex = index;
        emit currentIndexChanged();

        if (m_currentIndex >= 0) {
            moveTo(m_currentIndex, Immediate);
        }

        updateCurrentItem();
    }
}

void PagedView::setInteractive(bool interactive)
{
    if (m_interactive != interactive) {
        m_interactive = interactive;
        emit interactiveChanged();
    }
}

void PagedView::setDragThreshold(int threshold)
{
    if (m_dragThreshold != threshold) {
        m_dragThreshold = threshold;
        emit dragThresholdChanged();
    }
}

void PagedView::setMoveDragThreshold(int threshold)
{
    if (m_moveDragThreshold != threshold) {
        m_moveDragThreshold = threshold;
        emit moveDragThresholdChanged();
    }
}

void PagedView::setMoveDuration(int duration)
{
    if (m_moveDuration != duration) {
        m_moveDuration = duration;
        emit moveDurationChanged();
    }
}

void PagedView::setCacheSize(int size)
{
    if (m_cacheSize != size) {
        m_cacheSize = size;
        emit cacheSizeChanged();
        updateLayout();
    }
}

void PagedView::setContentOffset(qreal offset)
{
    if (!qFuzzyCompare(m_contentOffset, offset)) {
        m_contentOffset = offset;
        updateItemPositions();
        emit contentOffsetChanged();
    }
}

QQmlListProperty<QObject> PagedView::exposedItems()
{
    return QQmlListProperty<QObject>(this, &m_exposedItems);
}

void PagedView::moveTo(int index, int transition)
{
    if (index >= 0 && index < m_count && index != m_currentIndex) {
        qreal targetPosition = calculateItemPosition(index);

        if (transition == Immediate) {
            setContentOffset(targetPosition);
            setCurrentIndex(index);
        } else {
            if (!m_animation) {
                m_animation = new QPropertyAnimation(this, "contentOffset", this);
                connect(m_animation, &QPropertyAnimation::finished, this, &PagedView::onAnimationFinished);
            }

            m_animation->setDuration(m_moveDuration);
            m_animation->setStartValue(m_contentOffset);
            m_animation->setEndValue(targetPosition);
            m_animation->start();

            m_moving = true;
            emit movingChanged();
        }
    }
}

QQuickItem *PagedView::itemAt(int index)
{
    if (index >= 0 && index < m_items.size()) {
        return m_items[index];
    }
    return nullptr;
}

PagedViewAttached *PagedView::qmlAttachedProperties(QObject *object)
{
    return new PagedViewAttached(object);
}

void PagedView::componentComplete()
{
    Silica::Control::componentComplete();
    createContentItem();
    updateLayout();
}

void PagedView::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Silica::Control::geometryChanged(newGeometry, oldGeometry);
    m_layoutDirty = true;
    updateLayout();
}

void PagedView::mousePressEvent(QMouseEvent *event)
{
    if (m_interactive) {
        m_pressPos = event->pos();
        m_lastPos = m_pressPos;
        m_dragging = false;
        stopFlickAnimation();
    }
    Silica::Control::mousePressEvent(event);
}

void PagedView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_interactive && !m_pressPos.isNull()) {
        QPointF delta = event->pos() - m_pressPos;
        qreal distance = qSqrt(delta.x() * delta.x() + delta.y() * delta.y());

        if (!m_dragging && distance > m_dragThreshold) {
            m_dragging = true;
            emit draggingChanged();
        }

        if (m_dragging) {
            handleDragMove(event->pos());
        }

        m_lastPos = event->pos();
    }
    Silica::Control::mouseMoveEvent(event);
}

void PagedView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_interactive && m_dragging) {
        handleDragEnd(event->pos());
        m_dragging = false;
        emit draggingChanged();
    }

    m_pressPos = QPointF();
    m_lastPos = QPointF();
    Silica::Control::mouseReleaseEvent(event);
}

void PagedView::updatePolish()
{
    if (m_layoutDirty) {
        updateLayout();
        m_layoutDirty = false;
    }
}

void PagedView::onModelCountChanged()
{
    // This will be called when we have proper model support
    updateLayout();
}

void PagedView::onModelDataChanged(int index, int count)
{
    Q_UNUSED(index)
    Q_UNUSED(count)
    updateLayout();
}

void PagedView::onModelRowsInserted(int index, int count)
{
    Q_UNUSED(index)
    Q_UNUSED(count)
    updateLayout();
}

void PagedView::onModelRowsRemoved(int index, int count)
{
    Q_UNUSED(index)
    Q_UNUSED(count)
    updateLayout();
}

void PagedView::onModelRowsMoved(int from, int to, int count)
{
    Q_UNUSED(from)
    Q_UNUSED(to)
    Q_UNUSED(count)
    updateLayout();
}

void PagedView::onAnimationFinished()
{
    m_moving = false;
    emit movingChanged();

    // Update current index based on final position
    int newIndex = calculateIndexFromPosition(m_contentOffset);
    if (newIndex != m_currentIndex) {
        setCurrentIndex(newIndex);
    }
}

void PagedView::updateLayout()
{
    if (!m_contentItem) {
        return;
    }

    // Clear existing items
    for (QQuickItem *item : m_items) {
        if (item) {
            if (m_delegateModel) {
                // Release the item back to the delegate model
                m_delegateModel->release(item);
            } else {
                item->setParentItem(nullptr);
                item->deleteLater();
            }
        }
    }
    m_items.clear();

    // Create items for visible range
    int startIndex = qMax(0, m_currentIndex - m_cacheSize);
    int endIndex = qMin(m_count - 1, m_currentIndex + m_cacheSize);

    for (int i = startIndex; i <= endIndex; ++i) {
        QQuickItem *item = createItem(i);
        if (item) {
            m_items.append(item);
            positionItem(item, i);
            updateAttachedProperties(item, i);
        }
    }

    updateContentSize();
    updateCurrentItem();
    updateExposedItems();
}

void PagedView::updateCurrentItem()
{
    QQuickItem *newCurrentItem = itemAt(m_currentIndex);
    if (m_currentItem != newCurrentItem) {
        m_currentItem = newCurrentItem;
        emit currentItemChanged();
    }
}

void PagedView::updateExposedItems()
{
    m_exposedItems.clear();
    for (QQuickItem *item : m_items) {
        if (item && isItemVisible(item)) {
            m_exposedItems.append(item);
        }
    }
    emit exposedItemsChanged();
}

void PagedView::createContentItem()
{
    if (!m_contentItem) {
        m_contentItem = new QQuickItem(this);
        m_contentItem->setObjectName("contentItem");
    }
}

void PagedView::destroyContentItem()
{
    if (m_contentItem) {
        delete m_contentItem;
        m_contentItem = nullptr;
    }
}

QQuickItem *PagedView::createItem(int index)
{
    if (!m_delegate) {
        return nullptr;
    }

    QQmlContext *context = qmlContext(this);
    if (!context) {
        return nullptr;
    }

    QQuickItem *item = nullptr;

    if (m_delegateModel) {
        // Use QQmlDelegateModel to create the item
        item = qobject_cast<QQuickItem*>(m_delegateModel->object(index, QQmlIncubator::AsynchronousIfNested));
        if (item) {
            item->setParentItem(m_contentItem);
        }
    } else {
        // Fallback: create item directly from delegate
        item = qobject_cast<QQuickItem*>(m_delegate->create(context));

        if (item) {
            item->setParentItem(m_contentItem);
            // Set the model data if available
            if (m_model.isValid() && m_model.type() == QVariant::List) {
                QVariantList list = m_model.toList();
                if (index < list.size()) {
                    // Set the model data as a property if the item has an index property
                    item->setProperty("index", index);
                    item->setProperty("modelData", list[index]);
                }
            }
        }
    }

    return item;
}

void PagedView::destroyItem(QQuickItem *item)
{
    if (item) {
        item->setParentItem(nullptr);
        item->deleteLater();
    }
}

void PagedView::positionItem(QQuickItem *item, int index)
{
    if (!item) return;

    qreal position = calculateItemPosition(index) - m_contentOffset;

    switch (m_direction) {
        case LTR:
        case RTL:
            item->setX(position);
            item->setY(0);
            break;
        case TB:
        case BT:
            item->setX(0);
            item->setY(position);
            break;
    }
}

void PagedView::updateItemPositions()
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i]) {
            positionItem(m_items[i], i);
        }
    }
}

void PagedView::updateContentSize()
{
    if (!m_contentItem) return;

    qreal contentSize = 0;
    if (m_count > 0) {
        contentSize = calculateItemPosition(m_count - 1) + width();
    }

    switch (m_direction) {
        case LTR:
        case RTL:
            m_contentItem->setWidth(contentSize);
            m_contentItem->setHeight(height());
            break;
        case TB:
        case BT:
            m_contentItem->setWidth(width());
            m_contentItem->setHeight(contentSize);
            break;
    }
}

void PagedView::handleDragStart(const QPointF &pos)
{
    m_pressPos = pos;
    m_lastPos = pos;
    stopFlickAnimation();
}

void PagedView::handleDragMove(const QPointF &pos)
{
    QPointF delta = pos - m_lastPos;
    qreal offset = 0;

    switch (m_direction) {
        case LTR:
        case RTL:
            offset = delta.x();
            break;
        case TB:
        case BT:
            offset = delta.y();
            break;
    }

    setContentOffset(m_contentOffset - offset);
    m_lastPos = pos;
}

void PagedView::handleDragEnd(const QPointF &pos)
{
    QPointF delta = pos - m_pressPos;
    qreal velocity = 0;

    switch (m_direction) {
        case LTR:
        case RTL:
            velocity = delta.x() / 100.0; // Convert to velocity
            break;
        case TB:
        case BT:
            velocity = delta.y() / 100.0;
            break;
    }

    if (qAbs(velocity) > 0.5) {
        startFlickAnimation(velocity);
    } else {
        // Snap to nearest item
        int nearestIndex = calculateIndexFromPosition(m_contentOffset);
        moveTo(nearestIndex, Animated);
    }
}

void PagedView::startFlickAnimation(qreal velocity)
{
    if (!m_animation) {
        m_animation = new QPropertyAnimation(this, "contentOffset", this);
        connect(m_animation, &QPropertyAnimation::finished, this, &PagedView::onAnimationFinished);
    }

    qreal targetOffset = m_contentOffset - velocity * 1000; // Project position
    int targetIndex = calculateIndexFromPosition(targetOffset);
    qreal finalOffset = calculateItemPosition(targetIndex);

    m_animation->setDuration(m_moveDuration);
    m_animation->setStartValue(m_contentOffset);
    m_animation->setEndValue(finalOffset);
    m_animation->start();

    m_flicking = true;
    m_moving = true;
    emit movingChanged();
}

void PagedView::stopFlickAnimation()
{
    if (m_animation && m_animation->state() == QAbstractAnimation::Running) {
        m_animation->stop();
    }
    m_flicking = false;
}

qreal PagedView::calculateItemPosition(int index) const
{
    if (index < 0) return 0;

    qreal position = 0;
    qreal itemSize = 0;

    switch (m_direction) {
        case LTR:
        case RTL:
            itemSize = width() + m_horizontalSpacing;
            break;
        case TB:
        case BT:
            itemSize = height() + m_verticalSpacing;
            break;
    }

    position = index * itemSize;
    return position;
}

int PagedView::calculateIndexFromPosition(qreal position) const
{
    if (position < 0) return 0;

    qreal itemSize = 0;
    switch (m_direction) {
        case LTR:
        case RTL:
            itemSize = width() + m_horizontalSpacing;
            break;
        case TB:
        case BT:
            itemSize = height() + m_verticalSpacing;
            break;
    }

    if (itemSize <= 0) return 0;

    int index = qRound(position / itemSize);
    return qBound(0, index, m_count - 1);
}

bool PagedView::isItemVisible(QQuickItem *item) const
{
    if (!item) return false;

    QRectF itemRect = item->mapRectToItem(this, item->boundingRect());
    QRectF viewRect = boundingRect();

    return itemRect.intersects(viewRect);
}

void PagedView::updateAttachedProperties(QQuickItem *item, int index)
{
    PagedViewAttached *attached = qobject_cast<PagedViewAttached*>(qmlAttachedPropertiesObject<PagedView>(item, false));
    if (attached) {
        attached->setView(this);
        attached->setDirection(m_direction);
        attached->setIsCurrentItem(index == m_currentIndex);
        attached->setExposed(isItemVisible(item));
    }
}

// PagedViewAttached implementation
PagedViewAttached::PagedViewAttached(QObject *parent)
    : QObject(parent)
{
}

void PagedViewAttached::setDirection(PagedView::Direction direction)
{
    if (m_direction != direction) {
        m_direction = direction;
        emit directionChanged();
    }
}

void PagedViewAttached::setView(PagedView *view)
{
    if (m_view != view) {
        m_view = view;
        emit viewChanged();
    }
}

void PagedViewAttached::setContentWidth(qreal width)
{
    if (!qFuzzyCompare(m_contentWidth, width)) {
        m_contentWidth = width;
        emit contentWidthChanged();
    }
}

void PagedViewAttached::setContentHeight(qreal height)
{
    if (!qFuzzyCompare(m_contentHeight, height)) {
        m_contentHeight = height;
        emit contentHeightChanged();
    }
}

void PagedViewAttached::setIsCurrentItem(bool current)
{
    if (m_isCurrentItem != current) {
        m_isCurrentItem = current;
        emit isCurrentItemChanged();
    }
}

void PagedViewAttached::setExposed(bool exposed)
{
    if (m_exposed != exposed) {
        m_exposed = exposed;
        emit exposedChanged();
    }
}
