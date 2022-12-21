# Minimum CMake version for Qt6 not built statically
cmake_minimum_required(VERSION 3.16)

# Project setup
project(LunaUI 
        VERSION 1.0.0 
        DESCRIPTION "Multi-purpose user interface for the LunaSys project"
        LANGUAGES CXX)

# Qt6 requires version >= C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Attempt to look up Qt6 by first finding the Qt installaton
# When running cmake, pass the path to the installation via -DCMAKE_PREFIX_PATH option
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Configure project-wide defaults, including CMAKE_AUTOMOC ON
qt_standard_project_setup()

add_subdirectory(src/app)
