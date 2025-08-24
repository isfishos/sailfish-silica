cmake_minimum_required(VERSION 3.10)
project(sailfishsilica)

# Enable Qt automoc for QObject/Q_PLUGIN classes
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

# Find Qt5 packages
find_package(Qt5 COMPONENTS Core Gui Quick Qml REQUIRED)

# Find mlite5 package
find_package(PkgConfig REQUIRED)
pkg_check_modules(MLITE5 REQUIRED IMPORTED_TARGET mlite5)

# Optional: Find KF5 Wayland for blur behind effect
find_package(KF5Wayland QUIET)
if(KF5Wayland_FOUND)
    message(STATUS "KWayland found - enabling blur behind effect")
    add_definitions(-DHAVE_KWAYLAND)
else()
    message(STATUS "KWayland not found - blur behind effect disabled")
endif()

# Set version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 2)
set(VERSION_PATCH 143)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Add library subdirectory
add_subdirectory(lib)
add_subdirectory(plugin)
