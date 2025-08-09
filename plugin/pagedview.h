// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_PAGEDVIEW_H
#define SAILFISH_SILICA_PLUGIN_PAGEDVIEW_H

#include <silicacontrol.h>
#include <QQmlComponent>
#include <QQmlListProperty>
#include <QQuickItem>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPointF>
#include <qqml.h>

class QQmlDelegateModel;
class PagedViewAttached;

class PagedView : public Silica::Control
{
    Q_OBJECT
    Q_PROPERTY(QVariant model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QQmlComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(Qt::Alignment horizontalAlignment READ horizontalAlignment WRITE setHorizontalAlignment NOTIFY horizontalAlignmentChanged)
    Q_PROPERTY(Qt::Alignment verticalAlignment READ verticalAlignment WRITE setVerticalAlignment NOTIFY verticalAlignmentChanged)
    Q_PROPERTY(WrapMode wrapMode READ wrapMode WRITE setWrapMode NOTIFY wrapModeChanged)
    Q_PROPERTY(qreal horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing NOTIFY horizontalSpacingChanged)
    Q_PROPERTY(qreal verticalSpacing READ verticalSpacing WRITE setVerticalSpacing NOTIFY verticalSpacingChanged)
    Q_PROPERTY(QQuickItem *contentItem READ contentItem NOTIFY contentItemChanged)
    Q_PROPERTY(QQuickItem *currentItem READ currentItem NOTIFY currentItemChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool interactive READ interactive WRITE setInteractive NOTIFY interactiveChanged)
    Q_PROPERTY(int dragThreshold READ dragThreshold WRITE setDragThreshold NOTIFY dragThresholdChanged)
    Q_PROPERTY(int moveDragThreshold READ moveDragThreshold WRITE setMoveDragThreshold NOTIFY moveDragThresholdChanged)
    Q_PROPERTY(bool dragging READ dragging NOTIFY draggingChanged)
    Q_PROPERTY(bool moving READ moving NOTIFY movingChanged)
    Q_PROPERTY(int moveDuration READ moveDuration WRITE setMoveDuration NOTIFY moveDurationChanged)
    Q_PROPERTY(int cacheSize READ cacheSize WRITE setCacheSize NOTIFY cacheSizeChanged)
    Q_PROPERTY(QQmlListProperty<QObject> exposedItems READ exposedItems NOTIFY exposedItemsChanged)
    Q_PROPERTY(qreal contentOffset READ contentOffset WRITE setContentOffset NOTIFY contentOffsetChanged)

public:
    enum Direction {
        LTR, // Left to Right
        RTL, // Right to Left
        TB,  // Top to Bottom
        BT   // Bottom to Top
    };
    Q_ENUM(Direction)

    enum WrapMode {
        NoWrap = 0x00,
        WrapHorizontal = 0x01,
        WrapVertical = 0x02
    };
    Q_ENUM(WrapMode)
    Q_DECLARE_FLAGS(WrapModes, WrapMode)

    enum Transition {
        Animated = 0,
        Immediate = 1
    };
    Q_ENUM(Transition)

    explicit PagedView(QQuickItem *parent = nullptr);
    ~PagedView();

    QVariant model() const { return m_model; }
    void setModel(const QVariant &model);
    QQmlComponent *delegate() const { return m_delegate; }
    void setDelegate(QQmlComponent *delegate);
    Direction direction() const { return m_direction; }
    void setDirection(Direction direction);
    Qt::Alignment horizontalAlignment() const { return m_horizontalAlignment; }
    void setHorizontalAlignment(Qt::Alignment alignment);
    Qt::Alignment verticalAlignment() const { return m_verticalAlignment; }
    void setVerticalAlignment(Qt::Alignment alignment);
    WrapMode wrapMode() const { return m_wrapMode; }
    void setWrapMode(WrapMode mode);
    qreal horizontalSpacing() const { return m_horizontalSpacing; }
    void setHorizontalSpacing(qreal spacing);
    qreal verticalSpacing() const { return m_verticalSpacing; }
    void setVerticalSpacing(qreal spacing);
    QQuickItem *contentItem() const { return m_contentItem; }
    QQuickItem *currentItem() const { return m_currentItem; }
    int currentIndex() const { return m_currentIndex; }
    void setCurrentIndex(int index);
    int count() const { return m_count; }
    bool interactive() const { return m_interactive; }
    void setInteractive(bool interactive);
    int dragThreshold() const { return m_dragThreshold; }
    void setDragThreshold(int threshold);
    int moveDragThreshold() const { return m_moveDragThreshold; }
    void setMoveDragThreshold(int threshold);
    bool dragging() const { return m_dragging; }
    bool moving() const { return m_moving; }
    int moveDuration() const { return m_moveDuration; }
    void setMoveDuration(int duration);
    int cacheSize() const { return m_cacheSize; }
    void setCacheSize(int size);
    QQmlListProperty<QObject> exposedItems();
    qreal contentOffset() const { return m_contentOffset; }
    void setContentOffset(qreal offset);

    Q_INVOKABLE void moveTo(int index, int transition = Animated);
    Q_INVOKABLE QQuickItem *itemAt(int index);

    static PagedViewAttached *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void modelChanged();
    void delegateChanged();
    void directionChanged();
    void horizontalAlignmentChanged();
    void verticalAlignmentChanged();
    void wrapModeChanged();
    void horizontalSpacingChanged();
    void verticalSpacingChanged();
    void contentItemChanged();
    void currentItemChanged();
    void currentIndexChanged();
    void countChanged();
    void interactiveChanged();
    void dragThresholdChanged();
    void moveDragThresholdChanged();
    void draggingChanged();
    void movingChanged();
    void moveDurationChanged();
    void cacheSizeChanged();
    void exposedItemsChanged();
    void contentOffsetChanged();

protected:
    void componentComplete() override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void updatePolish() override;

private slots:
    void onModelCountChanged();
    void onModelDataChanged(int index, int count);
    void onModelRowsInserted(int index, int count);
    void onModelRowsRemoved(int index, int count);
    void onModelRowsMoved(int from, int to, int count);
    void onAnimationFinished();

private:
    void updateLayout();
    void updateCurrentItem();
    void updateExposedItems();
    void createContentItem();
    void destroyContentItem();
    QQuickItem *createItem(int index);
    void destroyItem(QQuickItem *item);
    void positionItem(QQuickItem *item, int index);
    void updateItemPositions();
    void updateContentSize();
    void handleDragStart(const QPointF &pos);
    void handleDragMove(const QPointF &pos);
    void handleDragEnd(const QPointF &pos);
    void startFlickAnimation(qreal velocity);
    void stopFlickAnimation();
    qreal calculateItemPosition(int index) const;
    int calculateIndexFromPosition(qreal position) const;
    bool isItemVisible(QQuickItem *item) const;
    void updateAttachedProperties(QQuickItem *item, int index);

    QVariant m_model;
    QQmlComponent *m_delegate = nullptr;
    Direction m_direction = LTR;
    Qt::Alignment m_horizontalAlignment = Qt::AlignHCenter;
    Qt::Alignment m_verticalAlignment = Qt::AlignVCenter;
    WrapMode m_wrapMode = NoWrap;
    qreal m_horizontalSpacing = 0.0;
    qreal m_verticalSpacing = 0.0;
    QQuickItem *m_contentItem = nullptr;
    QQuickItem *m_currentItem = nullptr;
    int m_currentIndex = -1;
    int m_count = 0;
    bool m_interactive = true;
    int m_dragThreshold = 10;
    int m_moveDragThreshold = 50;
    bool m_dragging = false;
    bool m_moving = false;
    int m_moveDuration = 300;
    int m_cacheSize = 3;
    QList<QObject*> m_exposedItems;

    // Private implementation details
    QQmlDelegateModel *m_delegateModel = nullptr;
    QList<QQuickItem*> m_items;
    QPropertyAnimation *m_animation = nullptr;
    QPointF m_pressPos;
    QPointF m_lastPos;
    qreal m_contentOffset = 0.0;
    qreal m_targetOffset = 0.0;
    bool m_flicking = false;
    QTimer m_polishTimer;
    bool m_layoutDirty = true;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PagedView::WrapModes)

class PagedViewAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PagedView::Direction direction READ direction NOTIFY directionChanged)
    Q_PROPERTY(PagedView *view READ view NOTIFY viewChanged)
    Q_PROPERTY(qreal contentWidth READ contentWidth NOTIFY contentWidthChanged)
    Q_PROPERTY(qreal contentHeight READ contentHeight NOTIFY contentHeightChanged)
    Q_PROPERTY(bool isCurrentItem READ isCurrentItem NOTIFY isCurrentItemChanged)
    Q_PROPERTY(bool exposed READ exposed NOTIFY exposedChanged)

public:
    explicit PagedViewAttached(QObject *parent = nullptr);

    PagedView::Direction direction() const { return m_direction; }
    void setDirection(PagedView::Direction direction);
    PagedView *view() const { return m_view; }
    void setView(PagedView *view);
    qreal contentWidth() const { return m_contentWidth; }
    void setContentWidth(qreal width);
    qreal contentHeight() const { return m_contentHeight; }
    void setContentHeight(qreal height);
    bool isCurrentItem() const { return m_isCurrentItem; }
    void setIsCurrentItem(bool current);
    bool exposed() const { return m_exposed; }
    void setExposed(bool exposed);

Q_SIGNALS:
    void directionChanged();
    void viewChanged();
    void contentWidthChanged();
    void contentHeightChanged();
    void isCurrentItemChanged();
    void exposedChanged();

private:
    PagedView::Direction m_direction = PagedView::LTR;
    PagedView *m_view = nullptr;
    qreal m_contentWidth = 0.0;
    qreal m_contentHeight = 0.0;
    bool m_isCurrentItem = false;
    bool m_exposed = false;
};

QML_DECLARE_TYPEINFO(PagedView, QML_HAS_ATTACHED_PROPERTIES)

#endif // SAILFISH_SILICA_PLUGIN_PAGEDVIEW_H
