// SPDX-License-Identifier: LGPL-2.1-only

#include <QQmlExtensionPlugin>
#include <QQmlEngine>

#include "silicaitem.h"
#include "silicacontrol.h"
#include "silicatheme.h"
#include "silicascreen.h"
#include "silicaimageprovider.h"
#include "animatedloader.h"
#include "backgroundrectangle.h"
#include "buttonlayout.h"
#include "autoscroll.h"
#include "autoscrollcontroller.h"
#include "declarativeclipboard.h"
#include "declarativedatetime.h"
#include "declarativedialogresult.h"
#include "declarativedialogstatus.h"
#include "declarativebounceeffect.h"
#include "applicationbackground.h"
#include "declarativeconfig.h"
#include "autofill.h"
#include "declarativecover.h"
#include "declarativecoveraction.h"
#include "declarativecoveractionarea.h"
#include "declarativecoveractionlist.h"
#include "declarativecoverwindow.h"
#include "declarativedialogresult.h"
#include "declarativedialogstatus.h"
#include "declarativedimmedregion.h"
#include "declarativedock.h"
#include "declarativedragfilter.h"
#include "declarativeenterkey.h"
#include "declarativefocusbehavior.h"
#include "declarativeformatter.h"
#include "declarativeframerate.h"
#include "declarativeopacityramp.h"
#include "declarativeorientation.h"
#include "declarativepagenavigation.h"
#include "declarativepagestackaction.h"
#include "declarativepagestackbase.h"
#include "declarativepagestatus.h"
#include "declarativepreedittext.h"
#include "declarativequickscrollbuttonbase.h"
#include "declarativestandardpaths.h"
#include "declarativetabviewaction.h"
#include "declarativetextbaseitem.h"
#include "declarativetexteditor.h"
#include "declarativetouchblocker.h"
#include "declarativetouchinteraction.h"
#include "declarativeunderline.h"
#include "declarativeutil.h"
#include "declarativevariantinterpolator.h"
#include "declarativevisibilitycull.h"
#include "declarativewindow.h"
#include "dragdetectoritem.h"
#include "drawingarea.h"
#include "formattingproxymodel.h"
#include "linegraph.h"
#include "lineitem.h"
#include "minversemousearea.h"
#include "notice.h"
#include "notices.h"
#include "overlaygradient.h"
#include "pagedview.h"
#include "profilelistener.h"
#include "proxyvalidator.h"
#include "pulleymenulogic.h"
#include "quickscrolldirection.h"
#include "remorsecache.h"
#include "roundedwindowcorners.h"
#include "slideable.h"
#include "stringlistmodel.h"
#include "swipegesturearea.h"
#include "textlayoutmodel.h"
#include "timezoneupdater.h"
#include "transientimageprovider.h"
#include "wallpaperwindow.h"
#include "declarativebusyindicatorsize.h"
#include "declarativecutoutmode.h"
#include "timepickermode.h"

class SailfishSilicaPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char *uri) override
    {
        // Public API
        qmlRegisterType<Silica::Item>(uri, 1, 0, "SilicaItem");
        qmlRegisterType<Silica::Control>(uri, 1, 0, "SilicaControl");
        qmlRegisterType<ButtonLayout>(uri, 1, 0, "ButtonLayout");
        qmlRegisterType<DeclarativeCover>(uri, 1, 0, "Cover");
        qmlRegisterType<DeclarativeCoverAction>(uri, 1, 0, "CoverAction");
        qmlRegisterType<DeclarativeCoverActionList>(uri, 1, 0, "CoverActionList");
        qmlRegisterType<DeclarativeDimmedRegion>(uri, 1, 0, "DimmedRegion");
        qmlRegisterType<DeclarativeFormatter>(uri, 1, 0, "Formatter");
        qmlRegisterType<DeclarativePageStackBase>(uri, 1, 0, "PageStackBase");
        qmlRegisterType<DeclarativeTouchBlocker>(uri, 1, 0, "TouchBlocker");
        qmlRegisterType<FormattingProxyModel>(uri, 1, 0, "FormattingProxyModel");
        qmlRegisterType<MInverseMouseArea>(uri, 1, 0, "InverseMouseArea");
        qmlRegisterType<Notice>(uri, 1, 0, "Notice");
        qmlRegisterType<PagedView>(uri, 1, 0, "PagedView");

        // Private API
        if (strcmp(uri, "Sailfish.Silica.private") == 0) {
            qmlRegisterType<AnimatedLoader>(uri, 1, 0, "AnimatedLoader");
            qmlRegisterType<BackgroundRectangle>(uri, 1, 0, "BackgroundRectangle");
            qmlRegisterType<Slideable>(uri, 1, 0, "SlideableBase");
            qmlRegisterType<StringListModel>(uri, 1, 0, "StringListModel");
            qmlRegisterType<SwipeGestureArea>(uri, 1, 0, "SwipeGestureArea");
            qmlRegisterType<TextLayoutModel>(uri, 1, 0, "TextLayoutModel");
            qmlRegisterType<WallpaperWindow>(uri, 1, 0, "WallpaperWindow");

            // Uncreatable types
            qmlRegisterUncreatableType<Slide>(uri, 1, 0, "Slide", "Slide is an attached type");

            // Enum types
            qmlRegisterUncreatableType<DeclarativeBusyIndicatorSize>(uri, 1, 0, "BusyIndicatorSize", "BusyIndicatorSize is an enum type");
            qmlRegisterUncreatableType<DeclarativeCutoutMode>(uri, 1, 0, "CutoutMode", "CutoutMode is an enum type");
            qmlRegisterUncreatableType<DeclarativeFocusBehavior>(uri, 1, 0, "FocusBehavior", "FocusBehavior is an enum type");
            qmlRegisterUncreatableType<QuickScrollDirection>(uri, 1, 0, "QuickScrollDirection", "QuickScrollDirection is an enum type");
            qmlRegisterUncreatableType<TimePickerMode>(uri, 1, 0, "TimePickerMode", "TimePickerMode is an enum type");
        }

        // Singletons
        qmlRegisterSingletonInstance(uri, 1, 0, "Theme", Silica::Theme::instance());
        qmlRegisterSingletonInstance(uri, 1, 0, "Screen", Silica::Screen::instance());
        qmlRegisterSingletonType<Notices>(uri, 1, 0, "Notices",
            [](QQmlEngine*, QJSEngine*) -> QObject* { return Notices::instance(); });

        // Private types
        const char *priv = "Sailfish.Silica.private";
        qmlRegisterType<AutoScrollController>(priv, 1, 0, "AutoScrollController");
        qmlRegisterUncreatableType<AutoScroll>(priv, 1, 0, "AutoScroll", "Attached-only");
        qmlRegisterType<AutoFill>(priv, 1, 0, "AutoFill");
        qmlRegisterType<DeclarativeBounceEffect>(uri, 1, 0, "BounceEffect");
        qmlRegisterType<OverlayGradient>(priv, 1, 0, "OverlayGradientBase");
        qmlRegisterType<ProfileListener>(priv, 1, 0, "ProfileListener");
        qmlRegisterType<ProxyValidator>(priv, 1, 0, "ProxyValidator");
        qmlRegisterType<PulleyMenuLogic>(priv, 1, 0, "PulleyMenuLogic");
        qmlRegisterType<RemorseCache>(priv, 1, 0, "RemorseCache");
        qmlRegisterType<RoundedWindowCorners>(priv, 1, 0, "RoundedWindowCorners");

        // Enum/uncreatable helpers under public
        qmlRegisterUncreatableType<DeclarativeDateTime>(uri, 1, 0, "DateTime", "Enums only");
        qmlRegisterUncreatableType<DeclarativeDialogResult>(uri, 1, 0, "DialogResult", "Enums only");
        qmlRegisterUncreatableType<DeclarativeDialogStatus>(uri, 1, 0, "DialogStatus", "Enums only");
        qmlRegisterUncreatableType<DeclarativeDock>(uri, 1, 0, "Dock", "Enums only");
        qmlRegisterUncreatableType<DeclarativeOpacityRamp>(uri, 1, 0, "OpacityRamp", "Enums only");
        qmlRegisterUncreatableType<DeclarativeOrientation>(uri, 1, 0, "Orientation", "Enums only");
        qmlRegisterUncreatableType<DeclarativePageNavigation>(uri, 1, 0, "PageNavigation", "Enums only");
        qmlRegisterUncreatableType<DeclarativePageStackAction>(uri, 1, 0, "PageStackAction", "Enums only");
        qmlRegisterUncreatableType<DeclarativePageStatus>(uri, 1, 0, "PageStatus", "Enums only");
        qmlRegisterUncreatableType<DeclarativeTabViewAction>(uri, 1, 0, "TabViewAction", "Enums only");
        qmlRegisterUncreatableType<DeclarativeTextEditor>(uri, 1, 0, "TextEditor", "Enums only");
        qmlRegisterUncreatableType<DeclarativeTouchInteraction>(uri, 1, 0, "TouchInteraction", "Enums only");
        qmlRegisterSingletonType<DeclarativeClipboard>(uri, 1, 0, "Clipboard",
            [](QQmlEngine*, QJSEngine*) -> QObject* { return new DeclarativeClipboard; });
        qmlRegisterSingletonType<DeclarativeStandardPaths>(uri, 1, 0, "StandardPaths",
            [](QQmlEngine*, QJSEngine*) -> QObject* { return DeclarativeStandardPaths::instance(); });
        qmlRegisterSingletonType<DeclarativeUtil>(uri, 1, 0, "Util",
            [](QQmlEngine*, QJSEngine*) -> QObject* { return DeclarativeUtil::instance(); });

        // Private singletons
        qmlRegisterSingletonType<DeclarativeConfigApi>(priv, 1, 0, "Config",
            [](QQmlEngine*, QJSEngine*) -> QObject* { return DeclarativeConfigApi::instance(); });
        // Private creatable helpers
        qmlRegisterType<ApplicationBackground>(priv, 1, 0, "ApplicationBackground");
        qmlRegisterType<DeclarativeCoverActionArea>(priv, 1, 0, "CoverActionArea");
        qmlRegisterType<DeclarativeCoverWindow>(priv, 1, 0, "CoverWindow");
        qmlRegisterUncreatableType<DeclarativeDragFilter>(priv, 1, 0, "DragFilter", "Attached-only");
        qmlRegisterUncreatableType<DeclarativeEnterKey>(uri, 1, 0, "EnterKey", "Attached-only");
        qmlRegisterType<DeclarativePreeditText>(priv, 1, 0, "PreeditText");
        qmlRegisterType<DeclarativeQuickScrollButtonBase>(priv, 1, 0, "QuickScrollButtonBase");
        qmlRegisterType<DeclarativeTextBaseItem>(priv, 1, 0, "TextBaseItem");
        qmlRegisterType<DeclarativeUnderline>(priv, 1, 0, "Underline");
        qmlRegisterType<DeclarativeVariantInterpolator>(priv, 1, 0, "VariantInterpolator");
        qmlRegisterType<DeclarativeVisibilityCull>(priv, 1, 0, "VisibilityCull");
        qmlRegisterType<DeclarativeWindow>(priv, 1, 0, "Window");
        qmlRegisterType<DragDetectorItem>(priv, 1, 0, "DragDetectorItem");
        qmlRegisterType<DrawingArea>(priv, 1, 0, "DrawingArea");
        qmlRegisterType<LineGraph>(priv, 1, 0, "LineGraph");
        qmlRegisterType<LineItem>(priv, 1, 0, "LineItem");
    }

    void initializeEngine(QQmlEngine *engine, const char *uri) override
    {
        Q_UNUSED(uri)
        // Image provider for theme icons
        engine->addImageProvider("theme", new Silica::ImageProvider(Silica::ImageProvider::LoadDefaultTheme));
    }
};

#include "sailfishsilicaplugin.moc"
