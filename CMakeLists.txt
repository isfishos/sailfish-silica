cmake_minimum_required(VERSION 3.10)
project(sailfishsilica)

# Find Qt5 packages
find_package(Qt5 COMPONENTS Core Gui REQUIRED)

# Find mlite5 package
find_package(PkgConfig REQUIRED)
pkg_check_modules(MLITE5 REQUIRED IMPORTED_TARGET mlite5)

# Set version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 2)
set(VERSION_PATCH 143)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Add library subdirectory
add_subdirectory(lib)