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
    silicabackground/abstractfilter.cpp
    silicabackground/sequencefilter.cpp
    silicabackground/convolutionfilter.cpp
    silicabackground/resizefilter.cpp
    silicabackground/repeatfilter.cpp
    silicabackground/shaderfilter.cpp
    silicabackground/kernel.cpp
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

# Install rules
install(TARGETS sailfishsilica
    LIBRARY DESTINATION lib
)
