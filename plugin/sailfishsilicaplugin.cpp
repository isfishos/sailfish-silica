// SPDX-License-Identifier: LGPL-2.1-only

#include <QQmlExtensionPlugin>
#include <QQmlEngine>

#include "silicaitem.h"
#include "silicacontrol.h"
#include "silicatheme.h"
#include "silicathemetransaction.h"
#include "silicascreen.h"
#include "silicaimageprovider.h"
#include "mousearea.h"
#include "highlightimagebase.h"
#include "text_p.h"
#include "declarativetruncationmode.h"
#include "silicapalette.h"

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
#include "declarativeglassitem.h"
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
#include "horizontalautoscroll.h"
#include "verticalautoscroll.h"

class SailfishSilicaPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override
    {
        if (strcmp(uri, "Sailfish.Silica") == 0) {
            // Public API types
            qmlRegisterType<Silica::Item>(uri, 1, 0, "SilicaItem");
            qmlRegisterType<Silica::Control>(uri, 1, 0, "SilicaControl");
            qmlRegisterType<Silica::Palette>(uri, 1, 0, "Palette");
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
            qmlRegisterType<DeclarativeBounceEffect>(uri, 1, 0, "BounceEffect");
            qmlRegisterType<DeclarativeEnterKey>(uri, 1, 0, "EnterKey");
            qmlRegisterType<DeclarativeGlassItem>(uri, 1, 0, "GlassItem");

            // Public singletons
            qmlRegisterSingletonType<Silica::Theme>(uri, 1, 0, "Theme",
                [](QQmlEngine*, QJSEngine*) -> QObject* { return new Silica::Theme; });
            qmlRegisterSingletonType<Silica::Screen>(uri, 1, 0, "Screen",
                [](QQmlEngine*, QJSEngine*) -> QObject* { return new Silica::Screen; });
            qmlRegisterSingletonType<Notices>(uri, 1, 0, "Notices",
                [](QQmlEngine*, QJSEngine*) -> QObject* { return new Notices; });
            qmlRegisterSingletonType<DeclarativeClipboard>(uri, 1, 0, "Clipboard",
                [](QQmlEngine*, QJSEngine*) -> QObject* { return new DeclarativeClipboard; });
            qmlRegisterSingletonType<DeclarativeStandardPaths>(uri, 1, 0, "StandardPaths",
                [](QQmlEngine*, QJSEngine*) -> QObject* { return new DeclarativeStandardPaths; });
            qmlRegisterSingletonType<DeclarativeUtil>(uri, 1, 0, "Util",
                [](QQmlEngine*, QJSEngine*) -> QObject* { return new DeclarativeUtil; });

            // Register meta types for QML property handling
            qRegisterMetaType<NoticeData>("NoticeData");

            // Public enum/uncreatable helpers
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
            qmlRegisterUncreatableType<DeclarativeTruncationMode>(uri, 1, 0, "TruncationMode", "Enums only");
            qmlRegisterUncreatableType<DeclarativeCutoutMode>(uri, 1, 0, "CutoutMode", "Enums only");

            // Attached-only auto scroll helpers
            qmlRegisterUncreatableType<HorizontalAutoScroll>(uri, 1, 0, "HorizontalAutoScroll", "Attached-only");
            qmlRegisterUncreatableType<VerticalAutoScroll>(uri, 1, 0, "VerticalAutoScroll", "Attached-only");

        } else if (strcmp(uri, "Sailfish.Silica.private") == 0) {
            // Private API types
            qmlRegisterType<Silica::MouseArea>(uri, 1, 0, "SilicaMouseArea");
            qmlRegisterType<Silica::HighlightImageBase>(uri, 1, 0, "HighlightImageBase");
            qmlRegisterType<Silica::Text>(uri, 1, 0, "SilicaText");
            qmlRegisterType<AnimatedLoader>(uri, 1, 0, "AnimatedLoader");
            qmlRegisterType<BackgroundRectangle>(uri, 1, 0, "BackgroundRectangle");
            qmlRegisterType<Slideable>(uri, 1, 0, "SlideableBase");
            qmlRegisterType<StringListModel>(uri, 1, 0, "StringListModel");
            qmlRegisterType<SwipeGestureArea>(uri, 1, 0, "SwipeGestureArea");
            qmlRegisterType<TextLayoutModel>(uri, 1, 0, "TextLayoutModel");
            qmlRegisterType<WallpaperWindow>(uri, 1, 0, "WallpaperWindow");
            qmlRegisterType<AutoScrollController>(uri, 1, 0, "AutoScrollController");
            qmlRegisterType<AutoFill>(uri, 1, 0, "AutoFill");
            qmlRegisterType<OverlayGradient>(uri, 1, 0, "OverlayGradientBase");
            qmlRegisterType<ProfileListener>(uri, 1, 0, "ProfileListener");
            qmlRegisterType<ProxyValidator>(uri, 1, 0, "ProxyValidator");
            qmlRegisterType<PulleyMenuLogic>(uri, 1, 0, "PulleyMenuLogic");
            qmlRegisterType<RemorseCache>(uri, 1, 0, "RemorseCache");
            qmlRegisterType<RoundedWindowCorners>(uri, 1, 0, "RoundedWindowCorners");
            qmlRegisterType<ApplicationBackground>(uri, 1, 0, "ApplicationBackground");
            qmlRegisterType<DeclarativeCoverActionArea>(uri, 1, 0, "CoverActionArea");
            qmlRegisterType<DeclarativeCoverWindow>(uri, 1, 0, "CoverWindow");
            qmlRegisterType<DeclarativePreeditText>(uri, 1, 0, "PreeditText");
            qmlRegisterType<DeclarativeQuickScrollButtonBase>(uri, 1, 0, "QuickScrollButtonBase");
            qmlRegisterType<DeclarativeTextBaseItem>(uri, 1, 0, "TextBaseItem");
            qmlRegisterType<DeclarativeUnderline>(uri, 1, 0, "Underline");
            qmlRegisterType<DeclarativeVariantInterpolator>(uri, 1, 0, "VariantInterpolator");
            qmlRegisterType<DeclarativeVisibilityCull>(uri, 1, 0, "VisibilityCull");
            qmlRegisterType<DeclarativeWindow>(uri, 1, 0, "Window");
            qmlRegisterType<DragDetectorItem>(uri, 1, 0, "DragDetectorItem");
            qmlRegisterType<DrawingArea>(uri, 1, 0, "DrawingArea");
            qmlRegisterType<LineGraph>(uri, 1, 0, "LineGraph");
            qmlRegisterType<LineItem>(uri, 1, 0, "LineItem");
            qmlRegisterType<Silica::ThemeTransaction>(uri, 1, 0, "ThemeTransaction");

            // Private uncreatable types
            qmlRegisterUncreatableType<Slide>(uri, 1, 0, "Slide", "Slide is an attached type");
            qmlRegisterUncreatableType<AutoScroll>(uri, 1, 0, "AutoScroll", "Attached-only");
            qmlRegisterUncreatableType<DeclarativeDragFilter>(uri, 1, 0, "DragFilter", "Attached-only");

            // Private enum types
            qmlRegisterUncreatableType<DeclarativeBusyIndicatorSize>(uri, 1, 0, "BusyIndicatorSize", "BusyIndicatorSize is an enum type");
            qmlRegisterUncreatableType<DeclarativeCutoutMode>(uri, 1, 0, "CutoutMode", "CutoutMode is an enum type");
            qmlRegisterUncreatableType<DeclarativeFocusBehavior>(uri, 1, 0, "FocusBehavior", "FocusBehavior is an enum type");
            qmlRegisterUncreatableType<QuickScrollDirection>(uri, 1, 0, "QuickScrollDirection", "QuickScrollDirection is an enum type");
            qmlRegisterUncreatableType<TimePickerMode>(uri, 1, 0, "TimePickerMode", "TimePickerMode is an enum type");
            qmlRegisterUncreatableType<DeclarativeTruncationMode>(uri, 1, 0, "TruncationMode", "TruncationMode is an enum type");

            // Private singletons
            qmlRegisterSingletonType<DeclarativeConfigApi>(uri, 1, 0, "Config",
                [](QQmlEngine*, QJSEngine*) -> QObject* { return DeclarativeConfigApi::instance(); });
        }
    }

    void initializeEngine(QQmlEngine *engine, const char *uri) override
    {
        Q_UNUSED(uri)
        // Image provider for theme icons
        engine->addImageProvider("theme", new Silica::ImageProvider(Silica::ImageProvider::LoadDefaultTheme));
    }
};

#include "sailfishsilicaplugin.moc"
