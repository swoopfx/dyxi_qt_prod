# project(UIModuleURI VERSION 0.1 LANGUAGES CXX)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY}
    CACHE STRING "Import Path for qt Creator"
    FORCE
)
# qt_add_qml_module(UIModule
#     URI UIModule
#     STATIC
#     QML_FILES
#         Main.qml
# )

find_package(Qt6 REQUIRED COMPONENTS Core5Compat Quick Multimedia
     QuickDialogs2 )




qt_add_qml_module(UIModule
   URI UIModule
   VERSION 1.0
    RESOURCE_PREFIX "/ui"

    # DEPENDENCIES
    #        QtQuick
    #        QtMultimedia #

   QML_FILES Pages/WelcomePage.qml
   QML_FILES Pages/LoginPage.qml
   QML_FILES Pages/SelectGameCategoryPage.qml
   QML_FILES js/NavigationTracker.js
   QML_FILES Components/GameItem.qml
   QML_FILES Pages/RecentPage.qml
   # Buttons
   QML_FILES Components/Buttons/ConfetiButton.qml
   QML_FILES Components/Buttons/DefaultButton.qml
   QML_FILES Components/Buttons/NeonButton.qml
   QML_FILES Components/Buttons/PulseButton.qml
   QML_FILES Components/LoadingnPage.qml
   QML_FILES Components/LoadingPage.qml
   QML_FILES Pages/GameLoaderPage.qml
   QML_FILES Pages/GameListPage.qml
   QML_FILES Pages/NoPageFound.qml
   QML_FILES Pages/NewGamepPage.qml
   QML_FILES Components/BackComponent.qml
   QML_FILES Pages/Sherpa.qml
   QML_FILES Pages/LoginPageNew.qml
   QML_FILES Pages/Tiles.qml
   QML_FILES Pages/StartEvaluation.qml
   QML_FILES Pages/Curriculum.qml
   QML_FILES Pages/StartCurriculum.qml


)
target_link_libraries(UIModule
    PRIVATE
    Qt6::Qml
    Qt6::Gui
    Qt6::Quick
    Qt6::Network
    Qt6::QuickDialogs2
    # Qt6::quickmultimedia
    # Qt6::TextToSpeech
    Qt6::Multimedia
    # Qt6::TextToSpeechQml
    Qt6::Core5Compat

    # coreLib

)

target_include_directories(UIModule
    PRIVATE ${CMAKE_SOURCE_DIR}/src/coreLib
)


set_property(TARGET UIModule PROPERTY QT_ANDROID_TARGET_SDK_VERSION 30)





qt_add_resources(UIModule
    "assets"
     PREFIX "/ui"
     FILES "images/logo.png"
     FILES "images/adhd.png"
     FILES "images/adhd_original.png"
     FILES "images/ADHD_Symbol.png"
     FILES "images/Artboard.png"
     FILES "images/background.png"
     FILES "images/cloud.png"
     FILES "images/dyscalculia.png"
     FILES "images/dyscalculia_original.png"
     FILES "images/dyslexia.png"
     FILES "images/dyslexia_original.png"
     FILES "images/Dyslexia_Symbol.png"
     FILES "images/dyxi_logo_42.png"
     FILES "images/grass.png"
     FILES "images/hamburger.png"
     FILES "images/hamburger_original.png"
     FILES "images/hole.png"
     FILES "images/hole_original.png"
     FILES "images/kidi.jpg"
     FILES "images/phonics.jpg"
     FILES "images/phonics.png"
     FILES "images/phonics_original.png"
     FILES "images/playground.jpg"
     FILES "images/profile.png"
     FILES "images/profile_original.png"
     FILES "images/rabbit.png"
     FILES "images/reading.png"
     FILES "images/recent.png"
     FILES "images/recent_original.png"
     FILES "images/recognition.png"
     FILES "images/recognition_original.png"
     FILES "images/robot_body.svg"
     FILES "images/robot_head.svg"
     FILES "images/robot_limb.svg"
     FILES "images/tracing.png"
     FILES "images/tracing_original.png"
     FILES "images/Welcome Dyxi.png"
     FILES "images/writing.png"
     FILES "images/writing_original.png"
     FILES "images/apple.svg"
     FILES "images/google.svg"
     FILES "images/dyxi-companion.png"
     FILES "images/dyxi-badge.png"
     FILES "images/dyxi-mark.png"
     FILES "images/dyxi-playful-bg.png"



 )


# qt_import_qml_plugins(UIModule)

# target_link_libraries(UIModule PRIVATE Writingplugin)
# Mark the QRC file so it is not processed by standard AUTORCC
# set_property(SOURCE resources.qrc PROPERTY SKIP_AUTORCC ON)

# # Process it using the native big resources function
# qt_add_big_resources(UIModule resources.qrc)
