# Library target
add_library(sailfishsilica SHARED
    mousearea.cpp
    rectangle.cpp
    silicacontrol.cpp
    silicaitem.cpp
    silicapalette.cpp
    silicascreen.cpp
    silicatheme.cpp
    silicathemeiconresolver.cpp
    text.cpp
    themecolors.cpp
    logging.cpp
    silicaimageprovider.cpp
    themedistancefield.cpp
    silicabackground/abstractfilter.cpp
    silicabackground/sequencefilter.cpp
    silicabackground/convolutionfilter.cpp
    silicabackground/resizefilter.cpp
    silicabackground/repeatfilter.cpp
    silicabackground/shaderfilter.cpp
    silicabackground/kernel.cpp
    silicabackground/fill.cpp
)

# Link Qt5 libraries
target_link_libraries(sailfishsilica
    Qt5::Core
    Qt5::Gui
    Qt5::Quick
    Qt5::QuickPrivate
    PkgConfig::MLITE5
)

# Set library properties
set_target_properties(sailfishsilica PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}
)

target_compile_definitions(sailfishsilica PRIVATE SAILFISH_SILICA_BUILD_LIBRARY UNIT_TEST)

# Install rules for the library
install(TARGETS sailfishsilica
    LIBRARY DESTINATION lib
)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/sailfish-silica
    FILES_MATCHING PATTERN "*.h"
    PATTERN "silicabackground/*.h"
)

# Install pkg-config file
configure_file(sailfish-silica.pc.in sailfish-silica.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sailfish-silica.pc
    DESTINATION lib/pkgconfig
)
