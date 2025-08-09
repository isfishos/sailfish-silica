#include "silicaprivateplugin_p.h"

#include <QQmlEngine>
#include <QQmlFileSelector>
#include <QQuickItem>
#include <private/qquickmousearea_p.h>
#include <private/qquickrectangle_p.h>
#include <private/qquicktext_p.h>

#include "mousearea_p.h"
#include "rectangle_p.h"
#include "silicabackground/fill.h"
#include "silicacontrol.h"
#include "silicaitem.h"
#include "silicatheme_p.h"
#include "text_p.h"

namespace Silica {
namespace Private {

void initializeEngine(QQmlEngine *engine, const char *uri)
{
    // QString platform = ThemePrivate::instance()->platform();
    QString platform = "";
    if (platform.isEmpty()) {
        return;
    }

    QQmlFileSelector *selector = QQmlFileSelector::get(engine);
    if (!selector) {
        selector = new QQmlFileSelector(engine);
    }

    selector->setExtraSelectors(QStringList() << platform);
}

void registerTypes(const char *uri)
{
    qmlRegisterRevision<QQuickItem, 6>(uri, 1, 0);
    qmlRegisterType<Silica::Item>(uri, 1, 0, "SilicaItem");
    qmlRegisterType<Silica::Control>(uri, 1, 0, "SilicaControl");
}

void registerPrivateTypes(const char *uri)
{
    qmlRegisterRevision<QQuickItem, 6>(uri, 1, 0);
    qmlRegisterRevision<QQuickMouseArea, 6>(uri, 1, 0);
    qmlRegisterRevision<QQuickRectangle, 6>(uri, 1, 0);
    qmlRegisterRevision<QQuickText, 6>(uri, 1, 0);

    qmlRegisterType<Silica::MouseArea>(uri, 1, 0, "SilicaMouseArea");
    qmlRegisterType<Silica::Rectangle>(uri, 1, 0, "SilicaRectangle");
    qmlRegisterType<Silica::Text>(uri, 1, 0, "SilicaText");

    qmlRegisterUncreatableType<Sailfish::Silica::Background::Fill>(uri, 1, 0, "FillPrivate", QString());
}

void registerBackgroundTypes(const char *uri)
{
}

} // namespace Private
} // namespace Silica


