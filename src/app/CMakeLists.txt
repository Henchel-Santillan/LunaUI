# Building an executable, with the project as the target
add_executable(${PROJECT_NAME}
    # Base Classes
    BaseDialog.cpp

    # Video Player
    VideoControlsWidget.cpp
    VideoDialog.cpp
    VideoRecentListWidget.cpp
    VideoWidget.cpp
    main.cpp
)

# Inform CMake that the executable uses certain Qt6 modules.
# References the Qt6 module targets from find_package() in the top-level CMakeLists.txt and will
# add the right arguments to the linker and pass the right include dirs and compiler defs to the C++ compiler
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt::Gui Qt6::Multimedia Qt6::MultimediaWidgets Qt6::SerialPort)

# Prevent the creation of a console window on Windows
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
