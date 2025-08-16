add_library(sailfishsilicaplugin SHARED
    animatedloader.cpp
    applicationbackground.cpp
    autoscroll.cpp
    autoscrollcontroller.cpp
    backgroundrectangle.cpp
    buttonlayout.cpp
    declarativebounceeffect.cpp
    declarativebusyindicatorsize.cpp
    declarativeclipboard.cpp
    declarativeconfig.cpp
    declarativecover.cpp
    declarativecoveraction.cpp
    declarativecoveractionarea.cpp
    declarativecoveractionlist.cpp
    declarativecoverwindow.cpp
    declarativecutoutmode.cpp
    declarativedatetime.cpp
    declarativedialogresult.cpp
    declarativedialogstatus.cpp
    declarativedimmedregion.cpp
    declarativedock.cpp
    declarativedragfilter.cpp
    declarativeenterkey.cpp
    declarativefocusbehavior.cpp
    declarativeformatter.cpp
    declarativeframerate.cpp
    declarativeopacityramp.cpp
    declarativeorientation.cpp
    declarativepagenavigation.cpp
    declarativepagestackaction.cpp
    declarativepagestackbase.cpp
    declarativepagestatus.cpp
    declarativepreedittext.cpp
    declarativequickscrollbuttonbase.cpp
    declarativestandardpaths.cpp
    declarativetabviewaction.cpp
    declarativetextbaseitem.cpp
    declarativetexteditor.cpp
    declarativetouchblocker.cpp
    declarativetouchinteraction.cpp
    declarativeunderline.cpp
    declarativeutil.cpp
    declarativevariantinterpolator.cpp
    declarativevisibilitycull.cpp
    declarativewindow.cpp
    dragdetectoritem.cpp
    drawingarea.cpp
    formattingproxymodel.cpp
    linegraph.cpp
    lineitem.cpp
    minversemousearea.cpp
    notice.cpp
    notices.cpp
    overlaygradient.cpp
    pagedview.cpp
    profilelistener.cpp
    proxyvalidator.cpp
    pulleymenulogic.cpp
    quickscrolldirection.cpp
    remorsecache.cpp
    roundedwindowcorners.cpp
    sailfishsilicaplugin.cpp
    slideable.cpp
    stringlistmodel.cpp
    swipegesturearea.cpp
    textlayoutmodel.cpp
    timepickermode.cpp
    timezoneupdater.cpp
    transientimageprovider.cpp
    wallpaperwindow.cpp
)

target_link_libraries(sailfishsilicaplugin
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
    sailfishsilica
)

set_target_properties(sailfishsilicaplugin PROPERTIES
    OUTPUT_NAME sailfishsilicaplugin
)

# Add include directories for the library headers
target_include_directories(sailfishsilicaplugin PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
)

# Function to find Qt QML install directory using qmake
function(FindQtInstallQml)
    find_program(QMAKE NAMES qmake5 qmake)
    if(NOT QMAKE)
        message(FATAL_ERROR "qmake not found")
    endif()
    execute_process(
        COMMAND ${QMAKE} -query QT_INSTALL_QML
        OUTPUT_VARIABLE PROC_RESULT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(QT_INSTALL_QML ${PROC_RESULT} PARENT_SCOPE)
endfunction()

# Get Qt QML install directory
FindQtInstallQml()

# Add background plugin subdirectory
add_subdirectory(background)

# Install the plugin to the QML module directory
install(TARGETS sailfishsilicaplugin
    LIBRARY DESTINATION ${QT_INSTALL_QML}/Sailfish/Silica
)

# Install QML files if they exist
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/qml)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/qml/
        DESTINATION ${QT_INSTALL_QML}/Sailfish/Silica
        FILES_MATCHING PATTERN "*.qml"
        PATTERN "*.js"
        PATTERN "qmldir"
    )
endif()

# Install plugin headers for development
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/sailfish-silica/plugin
    FILES_MATCHING PATTERN "*.h"
)
