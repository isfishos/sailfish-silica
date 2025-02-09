# Library target
add_library(sailfishsilica SHARED
    silicapalette.cpp
    silicascreen.cpp
    silicatheme.cpp
    silicathemeiconresolver.cpp
    themecolors.cpp
)

# Link Qt5 libraries
target_link_libraries(sailfishsilica
    Qt5::Core
    Qt5::Gui
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
