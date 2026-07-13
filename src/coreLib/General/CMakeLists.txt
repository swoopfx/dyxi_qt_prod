# find_package(Qt6 REQUIRED COMPONENTS Quick Gui Multimedia QuickControls2  Network Core TextToSpeech )#This provides
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Android)
find_package(Qt6 REQUIRED COMPONENTS Multimedia Core )



add_library(General STATIC

  # CoreSettings.hpp
  # neworkaccessmanager.h
  # neworkaccessmanager.cpp
  # usersession.h
  # usersession.cpp

)

target_include_directories(General
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/WhisperApp
)
qt_add_qml_module(General
  URI General
  # SOURCES CoreSettings.hpp
  # SOURCES neworkaccessmanager.h neworkaccessmanager.cpp
  # SOURCES usersession.h usersession.cpp
  SOURCES CoreSettings.hpp
  SOURCES neworkaccessmanager.h neworkaccessmanager.cpp
  SOURCES usersession.h usersession.cpp
  SOURCES networkaccessitemlist.h networkaccessitemlist.cpp
  SOURCES
  SOURCES abstracterrorprocessing.h abstracterrorprocessing.cpp
  # SOURCES poststudentactivity.h poststudentactivity.cpp
  # SOURCES postactivity.h postactivity.cpp
  SOURCES networkaccesspresets.h networkaccesspresets.cpp
  SOURCES postact.h postact.cpp
  SOURCES comparestrings.h comparestrings.cpp
  SOURCES audiomanipulator.h audiomanipulator.cpp
  SOURCES audiotranscoder.h audiotranscoder.cpp
  SOURCES networkinformation.h networkinformation.cpp
  SOURCES speechrecognizer.h speechrecognizer.cpp
  SOURCES audio/circularaudiobuffer.h audio/circularaudiobuffer.cpp
  SOURCES modelmanager.h modelmanager.cpp
  SOURCES inputeventfilter.h inputeventfilter.cpp
  SOURCES whisperloader.h whisperloader.cpp
  SOURCES sherpa/sherparecognizer.h sherpa/sherparecognizer.cpp
  SOURCES sherpa/audiocapture.h sherpa/audiocapture.cpp
  SOURCES sherpa/modelextractor.h sherpa/modelextractor.cpp
   SOURCES sherpa/recognitionworker.h sherpa/recognitionworker.cpp
  SOURCES sherpa/speechmanager.h sherpa/speechmanager.cpp
  # SOURCES networinfo.h networinfo.cpp
)

target_link_libraries(General
PUBLIC
  Qt6::Core
  Qt6::Qml
  Qt6::Network
  Qt6::Multimedia
  # Qt6::Android
  whisper
  ggml
  WhisperApp
  Sherpa
)

# if(ANDROID)
#     target_link_libraries(General PRIVATE Qt6::CorePrivate)
# endif()

target_include_directories(General
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/sherpa
)



target_link_libraries(General PRIVATE whisper_wrapper)



