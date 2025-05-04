# src/plugins/FFmpegPlugin/CMakeLists.txt

qt_standard_project_setup()          # turn on AUTOMOC/AUTOUIC
qt6_wrap_ui(SETTINGS_UI_HDR FFmpegSettingsWidget.ui)

qt_add_library(FFmpegPlugin STATIC
    FFmpegPlugin.cpp
    FFmpegPlugin.h
    FFmpegSettingsWidget.cpp
    FFmpegSettingsWidget.h
    CalculateVmafWidget.cpp
    CalculateVmafWidget.h
    ${SETTINGS_UI_HDR}
)

target_link_libraries(FFmpegPlugin
    PRIVATE Qt6::Core Qt6::Widgets core
)

# place resulting .dll/.so into the plugins folder next to the exe
set_target_properties(FFmpegPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
)
