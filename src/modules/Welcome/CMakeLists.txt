

# Find Qt6 components
find_package(Qt6 6.10 COMPONENTS Gui Qml Quick Multimedia REQUIRED)



# Qt Quick QML module linking and asset packaging
qt_add_qml_module(Welcome
    STATIC
    URI Welcome RESOURCE_PREFIX "/modules/welcome"
    VERSION 1.0
    QML_FILES
    WelcomeMain.qml
        DyxiLogoShape.qml

    SOURCES
        WelcomeMain.cpp
        audio_synth.h
        audio_synth.cpp
)

# Link platform libraries
target_link_libraries(Welcome PRIVATE
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Multimedia
)