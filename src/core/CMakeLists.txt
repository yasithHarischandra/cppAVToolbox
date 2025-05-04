# File: src/core/CMakeLists.txt

set(CORE_SOURCES
    ConfigManager.cpp
    ExternalToolJob.cpp
    VmafResult.cpp
    PluginManager.cpp
)

set(CORE_HEADERS
    ConfigManager.h
    ExternalToolJob.h
    VmafResult.h
    IConsoleToolPlugin.h
    PluginManager.h
)

add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(core Qt6::Core Qt6::Widgets)
