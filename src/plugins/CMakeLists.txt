# src/plugins/CMakeLists.txt
# ------------------------
# Add each console‑tool plugin as a subdirectory

# FFmpeg plugin
file(GLOB PLUGIN_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} */CMakeLists.txt)

foreach(PLUGIN_CMAKELIST ${PLUGIN_DIRS})
    get_filename_component(PLUGIN_DIR ${PLUGIN_CMAKELIST} DIRECTORY)
    message(STATUS "Adding plugin: ${PLUGIN_DIR}")
    add_subdirectory(${PLUGIN_DIR})
endforeach()

# TSDuck plugin (when ready)
# add_subdirectory(TSDuckPlugin)

# Add more plugins here:
# add_subdirectory(AnotherToolPlugin)
