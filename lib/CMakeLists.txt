# Library target
add_library(libsailfishsilica SHARED
    silicatheme.cpp
    silicathemeiconresolver.cpp
)

# Link Qt5 libraries
target_link_libraries(libsailfishsilica
    Qt5::Core
    Qt5::Gui
    PkgConfig::MLITE5
)

# Set library properties
set_target_properties(libsailfishsilica PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}
    OUTPUT_NAME libsailfishsilica
)

# Install rules
install(TARGETS libsailfishsilica
    LIBRARY DESTINATION lib
)
