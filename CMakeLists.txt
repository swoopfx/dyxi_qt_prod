include(FetchContent)
cmake_minimum_required(VERSION 3.16)

project(Dyxi_prod VERSION 0.1 LANGUAGES CXX)
set(QT_QML_GENERATE_QMLLS_INI ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 Network Core NetworkAuth Multimedia)

qt_standard_project_setup(REQUIRES 6.11)


#
# ----------------------------------------------------------
# Sherpa-ONNX
# ----------------------------------------------------------
#

set(SHERPA_LIB
    ${CMAKE_SOURCE_DIR}/third_party/sherpa/lib/arm64-v8a
)



add_library(kaldi-decoder-core STATIC IMPORTED)

set_target_properties(kaldi-decoder-core PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libkaldi-decoder-core.a
)

#
# ONNX Runtime
#

add_library(onnxruntime SHARED IMPORTED)

set_target_properties(onnxruntime PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libonnxruntime.so
)

#
# Sherpa Core
#

add_library(sherpa-onnx-core STATIC IMPORTED)

set_target_properties(sherpa-onnx-core PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libsherpa-onnx-core.a
)

#
# Kaldi Native Fbank
#

add_library(kaldi-native-fbank STATIC IMPORTED)

set_target_properties(kaldi-native-fbank PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libkaldi-native-fbank-core.a
)

#
# KissFFT
#

add_library(kissfft STATIC IMPORTED)

set_target_properties(kissfft PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libkissfft-float.a
)

#
# FST
#

add_library(sherpa-fst STATIC IMPORTED)

set_target_properties(sherpa-fst PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libsherpa-onnx-fst.a
)

add_library(sherpa-fstfar STATIC IMPORTED)

set_target_properties(sherpa-fstfar PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libsherpa-onnx-fstfar.a
)

add_library(sherpa-kaldifst STATIC IMPORTED)

set_target_properties(sherpa-kaldifst PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libsherpa-onnx-kaldifst-core.a
)

#
# SentencePiece
#

add_library(sentencepiece STATIC IMPORTED)

set_target_properties(sentencepiece PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libssentencepiece_core.a
)

#
# Piper
#

add_library(piper_phonemize STATIC IMPORTED)

set_target_properties(piper_phonemize PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libpiper_phonemize.a
)

add_library(espeak STATIC IMPORTED)

set_target_properties(espeak PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libespeak-ng.a
)

add_library(ucd STATIC IMPORTED)

set_target_properties(ucd PROPERTIES
    IMPORTED_LOCATION
    ${SHERPA_LIB}/libucd.a
)


# =========================
# Crypto libs
# =========================
add_library(crypto3 SHARED IMPORTED)
set_target_properties(crypto3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/arm64-v8a/libcrypto_3.so"
)
# =========================
# SSL libs
# =========================
add_library(ssl3 SHARED IMPORTED)
set_target_properties(ssl3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libssl_3.so"
)


file(GLOB_RECURSE QML_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/pages/*.qml
    ${CMAKE_CURRENT_SOURCE_DIR}/components/*.qml
)
# =========================
# App
# =========================
qt_add_executable(appDyxi_prod
    main.cpp

)


qt_add_qml_module(appDyxi_prod
    URI Dyxi_prod
    VERSION 1.0

    QML_FILES
        Main.qml
        ${QML_FILES}

    RESOURCES
        assets/images/background.jpg
        assets/images/edu_icon.png
        assets/images/game_icon.png
        assets/images/puzzle_icon.png
        assets/logo.png
        assets/sounds/background_music.mp3
        assets/sounds/click.wav
        assets/sounds/success.wav
)
target_include_directories(appDyxi_prod
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/coreLib/General
    ${CMAKE_SOURCE_DIR}/src/modules/QtCredentialsClients
)

set_target_properties(appDyxi_prod PROPERTIES
    QT_ANDROID_MIN_VERSION 33
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"

)
set_property(TARGET appDyxi_prod APPEND PROPERTY QT_ANDROID_EXTRA_LIBS
    "${CMAKE_SOURCE_DIR}/aarch64/libomp.so"
     # "${CMAKE_SOURCE_DIR}/src/coreLib/Sherpa/sherpa-ncnn/build-android/arm64-v8a/libsherpa-ncnn-c-api.so"
)

set_target_properties(appDyxi_prod PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)





# =========================
# Android settings
# =========================
# if (ANDROID)
#     set_target_properties(appDyxi_prod PROPERTIES
#         QT_ANDROID_APP_NAME "Dyxi"
#     )
# endif()
set_property(TARGET appDyxi_prod PROPERTY
    # QT_ANDROID_PACKAGE_SOURCE_DIR
    # ${CMAKE_SOURCE_DIR}/android
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
)

if (ANDROID)
set_target_properties(appDyxi_prod PROPERTIES
    # Sets the human-readable app name on the launcher
    QT_ANDROID_APP_NAME "Dyxi"

)

endif()

#
# ----------------------------------------------------------
# Android Packaging
# ----------------------------------------------------------
#

if(ANDROID)

    set_property(TARGET appDyxi_prod APPEND PROPERTY
        QT_ANDROID_EXTRA_LIBS
            ${SHERPA_LIB}/libonnxruntime.so
    )

endif()


# =========================
# QtKeychain (FIXED)
# =========================
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_WITH_QT6 ON CACHE BOOL "" FORCE)
# =========================
# Subprojects
# =========================
add_subdirectory(qtkeychain EXCLUDE_FROM_ALL)
add_subdirectory(src)




# =========================
# Linking (CORRECT)
# =========================
target_link_libraries(appDyxi_prod
    PRIVATE
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Network
        Qt6::Core
        Qt6::NetworkAuth
        Qt6::Multimedia

        onnxruntime

        sherpa-onnx-core
        kaldi-native-fbank
        kissfft
         kaldi-decoder-core
        sherpa-fst
        sherpa-fstfar
        sherpa-kaldifst

        sentencepiece

        piper_phonemize
        espeak
        ucd

        UIModule
        Welcomeplugin
        General
        QtCredentialsClientsplugin
        crypto3
        ssl3
        # whisper
        # Sherpaplugin
        # ggml
        # OpenMP::OpenMP_CXX

)



target_sources(appDyxi_prod PRIVATE
    ${CMAKE_SOURCE_DIR}/assets.qrc
)
# set_property(SOURCE resources.qrc PROPERTY QT_RESOURCE_BIG ON)if (ANDROID)
#     include(C:/Users/otaba/AppData/Local/Android/sdk/android_openssl/CMakeLists.txt)
# endif()
