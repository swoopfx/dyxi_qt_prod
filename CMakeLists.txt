cmake_minimum_required(VERSION 3.16)

project(Dyxi_prod VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 Network Core)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appDyxi_prod
    main.cpp
    assets.qrc
)

file(GLOB_RECURSE QML_FILES RELATIVE
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/pages/*.qml"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/*.qml"
)

qt_add_qml_module(appDyxi_prod
    URI Dyxi_prod
    QML_FILES
        Main.qml
    QML_FILES ${QML_FILES}
    RESOURCES assets.qrc

)

qt_add_resources(appDyxi_prod "assets"
    PREFIX "/"
    FILES "assets/images/background.jpg"
    FILES "assets/images/edu_icon.png"
    FILES "assets/images/game_icon.png"
    FILES "assets/images/puzzle_icon.png"

    FILES "assets/logo.png"
    FILES "assets/sounds/background_music.mp3"
    FILES "assets/sounds/click.wav"
    FILES "assets/sounds/success.wav"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appDyxi_prod PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDyxi_prod
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDyxi_prod
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appDyxi_prod
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
