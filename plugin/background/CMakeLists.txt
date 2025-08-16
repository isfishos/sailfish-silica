add_library(SailfishSilicaBackgroundPlugin SHARED
    plugin.cpp
    squareimageprovider.cpp
    squaretexturefactory.cpp
)

target_link_libraries(SailfishSilicaBackgroundPlugin
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
    sailfishsilica
)

set_target_properties(SailfishSilicaBackgroundPlugin PROPERTIES
    OUTPUT_NAME SailfishSilicaBackgroundPlugin
)

# Install the background plugin to the Sailfish.Silica.Background QML module directory
set(BACKGROUND_QML_INSTALL_DIR ${QT_INSTALL_QML}/Sailfish/Silica/Background)
install(TARGETS SailfishSilicaBackgroundPlugin
    LIBRARY DESTINATION ${BACKGROUND_QML_INSTALL_DIR}
)

# Install QML files for the background module if they exist
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/qml)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/qml/
        DESTINATION ${BACKGROUND_QML_INSTALL_DIR}
        FILES_MATCHING PATTERN "*.qml"
        PATTERN "*.js"
        PATTERN "qmldir"
    )
endif()
