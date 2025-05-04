# src/CMakeLists.txt
# ------------------
# Build the main application and its plugins

# First, build plugins
add_subdirectory(plugins)
add_subdirectory(core)

# Gather application sources
set(APP_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
)

# Automatically handle Qt .ui and moc
qt_standard_project_setup()
qt6_wrap_ui(UI_HEADERS MainWindow.ui)

# Define the executable target
add_executable(AVToolbox
    ${APP_SOURCES}
    ${UI_HEADERS}
)

# Link against Qt libraries
target_link_libraries(AVToolbox
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Charts
        core
        FFmpegPlugin
)

# Ensure the plugins folder is next to the executable at runtime
set_target_properties(AVToolbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Optionally, on Windows copy Qt DLLs and plugins automatically
if (WIN32)
  # Locate the windeployqt tool next to qmake:
  get_target_property(_qmake_exe Qt6::qmake IMPORTED_LOCATION)
  get_filename_component(_qt_bin_dir "${_qmake_exe}" DIRECTORY)
  find_program(WINDEPLOYQT_EXE
               NAMES windeployqt.exe windeployqt
               HINTS "${_qt_bin_dir}")

  if (WINDEPLOYQT_EXE)
    add_custom_command(TARGET AVToolbox
                       POST_BUILD
                       COMMAND "${WINDEPLOYQT_EXE}"
                               --pdb                # include debug symbols
                               --dir "$<TARGET_FILE_DIR:AVToolbox>"
                               "$<TARGET_FILE:AVToolbox>"
                       COMMENT "Running windeployqt to gather Qt DLLs..." )
  else()
    message(WARNING "windeployqt not found; your build may miss Qt DLLs at runtime.")
  endif()
endif()