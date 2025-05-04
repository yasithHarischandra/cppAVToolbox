# Top‑level CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(AVToolbox VERSION 1.0 LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Where to drop plugin libraries
set(PLUGIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/plugins)

# Locate Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

# Enable automatic MOC, UIC, RCC processing
qt_standard_project_setup()

# Ensure plugin output directory exists
file(MAKE_DIRECTORY ${PLUGIN_OUTPUT_DIR})

# Add the application and plugins
add_subdirectory(src)
