#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QQmlEngine>
#include "squareimageprovider.h"
#include "silicabackground/abstractfilter.h"
#include "silicabackground/filteredimage.h"

class SailfishSilicaBackgroundPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void initializeEngine(QQmlEngine *engine, const char *uri) override
    {
        Q_UNUSED(uri)
        // Add the "silica-square" image provider to the engine
        engine->addImageProvider("silica-square", new Sailfish::Silica::Background::SquareImageProvider());
    }

    void registerTypes(const char *uri) override
    {
        if (strcmp(uri, "Sailfish.Silica.Background") == 0) {
            // Register the module version
            qmlRegisterModule(uri, 1, 0);

            qmlRegisterUncreatableType<Sailfish::Silica::Background::AbstractFilter>(uri, 1, 0, "AbstractFilter", "AbstractFilter is a base class");
            qmlRegisterType<Sailfish::Silica::Background::FilteredImage>(uri, 1, 0, "FilteredImage");
        }
    }
};

#include "plugin.moc"
