// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_FILTEREDIMAGE_H
#define SAILFISH_SILICA_FILTEREDIMAGE_H

#include <private/qquickimagebase_p.h>
#include <QQmlListProperty>

namespace Sailfish {
namespace Silica {
namespace Background {
class AbstractFilter;

class FilteredImage : public QQuickImageBase
{
    Q_OBJECT
    Q_PROPERTY(bool filtering READ filtering WRITE setFiltering NOTIFY filteringChanged)
    Q_PROPERTY(QQmlListProperty<Sailfish::Silica::Background::AbstractFilter> filters READ filters NOTIFY filtersChanged)

public:
    explicit FilteredImage(QQuickItem *parent = nullptr);
    ~FilteredImage();

    bool filtering() const { return m_filtering; }
    void setFiltering(bool filtering);

    QQmlListProperty<AbstractFilter> filters();

Q_SIGNALS:
    void filteringChanged();
    void filtersChanged();

protected:
    void pixmapChange() override;

private:
    static void filters_append(QQmlListProperty<AbstractFilter> *prop, AbstractFilter *filter);
    static int filters_count(QQmlListProperty<AbstractFilter> *prop);
    static AbstractFilter *filters_at(QQmlListProperty<AbstractFilter> *prop, int index);
    static void filters_clear(QQmlListProperty<AbstractFilter> *prop);

    bool m_filtering = false;
    QList<AbstractFilter*> m_filters;
};

} // namespace Background
} // namespace Silica
} // namespace Sailfish

#endif // SAILFISH_SILICA_FILTEREDIMAGE_H
