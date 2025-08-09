// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_SLIDEABLE_H
#define SAILFISH_SILICA_PLUGIN_SLIDEABLE_H

#include <QQuickItem>
#include <QBasicTimer>
#include <QList>
#include <qqml.h>

class Slideable;

class Slide : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Slideable *view READ view NOTIFY viewChanged)
    Q_PROPERTY(QQuickItem *backward READ backward NOTIFY backwardChanged)
    Q_PROPERTY(QQuickItem *forward READ forward NOTIFY forwardChanged)
    Q_PROPERTY(qreal offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(bool isCurrent READ isCurrent NOTIFY isCurrentChanged)
    Q_PROPERTY(bool isExposed READ isExposed NOTIFY isExposedChanged)
    Q_PROPERTY(bool isFirst READ isFirst NOTIFY isFirstChanged)
    Q_PROPERTY(bool isLast READ isLast NOTIFY isLastChanged)
    Q_PROPERTY(bool inCache READ inCache NOTIFY inCacheChanged)
    Q_PROPERTY(bool keepAlive READ keepAlive WRITE setKeepAlive NOTIFY keepAliveChanged)
    Q_PROPERTY(qreal _offset READ _offset NOTIFY _offsetChanged)
    Q_PROPERTY(Slideable *_view READ _view NOTIFY _viewChanged)

public:
    enum Flow {
        LeftToRight = 0,
        RightToLeft = 1,
        TopToBottom = 2,
        BottomToTop = 3
    };
    Q_ENUM(Flow)

    enum Direction {
        NoDirection = 0,
        Forward = 1,
        Backward = 2
    };
    Q_ENUM(Direction)

    explicit Slide(QObject *parent = nullptr);

    Slideable *view() const { return m_view; }
    void setView(Slideable *view);
    QQuickItem *backward() const { return m_backward; }
    void setBackward(QQuickItem *backward);
    QQuickItem *forward() const { return m_forward; }
    void setForward(QQuickItem *forward);
    qreal offset() const { return m_offset; }
    void setOffset(qreal offset);
    bool isCurrent() const { return m_isCurrent; }
    void setIsCurrent(bool current);
    bool isExposed() const { return m_isExposed; }
    void setIsExposed(bool exposed);
    bool isFirst() const { return m_isFirst; }
    void setIsFirst(bool first);
    bool isLast() const { return m_isLast; }
    void setIsLast(bool last);
    bool inCache() const { return m_inCache; }
    void setInCache(bool inCache);
    bool keepAlive() const { return m_keepAlive; }
    void setKeepAlive(bool keepAlive);
    qreal _offset() const { return m_offset; }
    Slideable *_view() const { return m_view; }

    static Slide *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void viewChanged();
    void backwardChanged();
    void forwardChanged();
    void offsetChanged();
    void isCurrentChanged();
    void isExposedChanged();
    void isFirstChanged();
    void isLastChanged();
    void inCacheChanged();
    void keepAliveChanged();
    void _offsetChanged();
    void _viewChanged();

private:
    Slideable *m_view = nullptr;
    QQuickItem *m_backward = nullptr;
    QQuickItem *m_forward = nullptr;
    qreal m_offset = 0.0;
    bool m_isCurrent = false;
    bool m_isExposed = false;
    bool m_isFirst = false;
    bool m_isLast = false;
    bool m_inCache = false;
    bool m_keepAlive = false;
};

class Slideable : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem *currentItem READ currentItem WRITE setCurrentItem NOTIFY currentItemChanged)
    Q_PROPERTY(QQuickItem *alternateItem READ alternateItem NOTIFY alternateItemChanged)
    Q_PROPERTY(Slide::Flow flow READ flow WRITE setFlow NOTIFY flowChanged)
    Q_PROPERTY(int cacheSize READ cacheSize WRITE setCacheSize NOTIFY cacheSizeChanged)
    Q_PROPERTY(int cacheExpiry READ cacheExpiry WRITE setCacheExpiry NOTIFY cacheExpiryChanged)

public:
    explicit Slideable(QQuickItem *parent = nullptr);

    QQuickItem *currentItem() const { return m_currentItem; }
    void setCurrentItem(QQuickItem *item);
    QQuickItem *alternateItem() const { return m_alternateItem; }
    Slide::Flow flow() const { return m_flow; }
    void setFlow(Slide::Flow flow);
    int cacheSize() const { return m_cacheSize; }
    void setCacheSize(int size);
    int cacheExpiry() const { return m_cacheExpiry; }
    void setCacheExpiry(int expiry);

    Q_INVOKABLE void cache(QQuickItem *item);

Q_SIGNALS:
    void currentItemChanged();
    void alternateItemChanged();
    void exposedChanged();
    void flowChanged();
    void cacheSizeChanged();
    void cacheExpiryChanged();
    void createAdjacentItem(QQuickItem *item, Slide::Direction direction);

protected:
    void componentComplete() override;
    void itemChange(ItemChange change, const ItemChangeData &data) override;

private slots:
    void onCacheExpiry();

private:
    void updateSlideProperties();
    void updateCache();
    void evictExpiredItems();

    QQuickItem *m_currentItem = nullptr;
    QQuickItem *m_alternateItem = nullptr;
    Slide::Flow m_flow = Slide::LeftToRight;
    int m_cacheSize = 2;
    int m_cacheExpiry = 5000; // 5 seconds
    QList<QQuickItem*> m_cachedItems;
    QBasicTimer m_cacheTimer;
};

QML_DECLARE_TYPEINFO(Slide, QML_HAS_ATTACHED_PROPERTIES)

#endif // SAILFISH_SILICA_PLUGIN_SLIDEABLE_H
