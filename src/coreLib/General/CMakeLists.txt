# find_package(Qt6 REQUIRED COMPONENTS Quick Gui Multimedia QuickControls2  Network Core TextToSpeech )#This provides
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Android)
find_package(Qt6 REQUIRED COMPONENTS Multimedia Core NetworkAuth)



add_library(General STATIC

  # CoreSettings.hpp
  # neworkaccessmanager.h
  # neworkaccessmanager.cpp
  # usersession.h
  # usersession.cpp

)

# target_include_directories(General
#     PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}
#         ${CMAKE_SOURCE_DIR}/WhisperApp
# )
qt_add_qml_module(General
  URI General
  SOURCES CoreSettings.hpp
  SOURCES neworkaccessmanager.h neworkaccessmanager.cpp
  SOURCES usersession.h usersession.cpp
  SOURCES networkaccessitemlist.h networkaccessitemlist.cpp
  SOURCES
  SOURCES abstracterrorprocessing.h abstracterrorprocessing.cpp

  SOURCES networkaccesspresets.h networkaccesspresets.cpp
  SOURCES postact.h postact.cpp
  SOURCES comparestrings.h comparestrings.cpp
  SOURCES audiomanipulator.h audiomanipulator.cpp
  SOURCES audiotranscoder.h audiotranscoder.cpp
  SOURCES networkinformation.h networkinformation.cpp

  SOURCES inputeventfilter.h inputeventfilter.cpp

  SOURCES sherpa2/audio/audiocapture.h sherpa2/audio/audiocapture.cpp
  SOURCES sherpa2/vad/vadworker.h sherpa2/vad/vadworker.cpp
  SOURCES sherpa2/recognition/offlinerecognizerworker.h sherpa2/recognition/offlinerecognizerworker.cpp
  SOURCES sherpa2/core/recognitionpipeline.h sherpa2/core/recognitionpipeline.cpp
  SOURCES sherpa2/mainviewmodel.h sherpa2/mainviewmodel.cpp
  SOURCES sherpa2/modelextractoronnx.h sherpa2/modelextractoronnx.cpp
  SOURCES wardprofile.h wardprofile.cpp
  SOURCES pagerouter.h pagerouter.cpp
  SOURCES sessionmanager.h sessionmanager.cpp
)

target_link_libraries(General
PUBLIC
  Qt6::Core
  Qt6::Qml
  Qt6::Network
  Qt6::Multimedia
  Qt6::NetworkAuth
  # Qt6::Android
  # whisper
  # ggml
  # WhisperApp
  # Sherpa
)

target_include_directories(General
  PUBLIC
      ${CMAKE_SOURCE_DIR}/third_party/sherpa
      ${CMAKE_SOURCE_DIR}/src/modules/QtCredentialsClients
)

