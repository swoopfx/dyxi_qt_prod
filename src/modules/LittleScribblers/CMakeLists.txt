

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.11 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    Qml
    Network
    Multimedia
)

qt_standard_project_setup(REQUIRES 6.11)

qt_add_executable(LittleScribblers
    main.cpp
    backend/AppDifficulty.hpp
    backend/AppDifficulty.cpp
    backend/TracingEngine.hpp
    backend/TracingEngine.cpp
    backend/MediaController.hpp
    backend/MediaController.cpp
    backend/TelemetryManager.hpp
    backend/TelemetryManager.cpp
)

qt_add_qml_module(LittleScribblers
    URI LittleScribblers.Core
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/ShapeTracingPage.qml
        qml/FreeScribblePage.qml
        qml/ShapeSelectorPage.qml
        qml/ArtGalleryPage.qml
        qml/CelebrationDialog.qml
        qml/YardstickRuler.qml
    SOURCES
        backend/AppDifficulty.hpp
        backend/TracingEngine.hpp
        backend/MediaController.hpp
        backend/TelemetryManager.hpp
)

target_link_libraries(LittleScribblers PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::Network
    Qt6::Multimedia
)


