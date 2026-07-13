 
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

set(SHERPA_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/sherpa-ncnn)
# add_library(sherpa-ncnn-c-api SHARED IMPORTED)

# set_target_properties(sherpa-ncnn-c-api PROPERTIES
#     IMPORTED_LOCATION
#     ${SHERPA_LIB_DIR}/libsherpa-ncnn-c-api.so
# )
set(SHERPA_LIB_DIR
    ${SHERPA_ROOT}/build-android/arm64-v8a
)

# add_subdirectory(${SHERPA_ROOT})
qt_standard_project_setup()

# qt_add_executable(QtSherpa
#     main.cpp
#     MainWindow.cpp
# )

qt_add_qml_module(Sherpa
    STATIC
    URI Sherpa

    RESOURCE_PREFIX "/sherpa-ncnn/"
    # SOURCES whisperworker.cpp whisperworker.h


)
# add_library(sherpa-ncnn-c-api SHARED IMPORTED)
# set_target_properties(sherpa-ncnn-c-api PROPERTIES
#     IMPORTED_LOCATION ${SHERPA_LIB_DIR}/libsherpa-ncnn-c-api.so
# )

add_library(sherpa-ncnn-c-api SHARED IMPORTED)

set_target_properties(sherpa-ncnn-c-api PROPERTIES
    IMPORTED_LOCATION "${SHERPA_LIB_DIR}/libsherpa-ncnn-c-api.so"
)

target_link_directories(Sherpa PRIVATE ${SHERPA_LIB_DIR})

target_include_directories(Sherpa
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(Sherpa PRIVATE
  ${SHERPA_ROOT}/sherpa-ncnn/c-api
)

target_link_directories(Sherpa PRIVATE
    ${SHERPA_ROOT}/build-android/arm64-v8a
)

target_link_libraries(Sherpa PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    # libsherpa-ncnn-c-api.so
    # libsherpa-ncnn-core.a
    #  libkaldi-native-fbank-core.a
    # libkissfft-float.a
    #  libsherpa-ncnn-fst.a
    #  libsherpa-ncnn-fstfar.a
    #  libsherpa-ncnn-kaldifst-core.a
    #   libncnn.so
    # sherpa-ncnn-core
    # sherpa-ncnn-c-api
    # ncnn
)
target_link_libraries(Sherpa PRIVATE
    sherpa-ncnn-c-api
)
target_link_libraries(Sherpa PRIVATE
    ${SHERPA_LIB_DIR}/libkaldi-native-fbank-core.a
)

set_property(TARGET appDyxi_prod APPEND PROPERTY QT_ANDROID_EXTRA_LIBS
    "${SHERPA_LIB_DIR}/libsherpa-ncnn-c-api.so"
    "${SHERPA_LIB_DIR}/libncnn.so"
    # "${SHERPA_LIB_DIR}/libkaldi-native-fbank-core.so"
    "${CMAKE_SOURCE_DIR}/aarch64/libomp.so"
)
# message(STATUS "QT_DEPLOY_LIB_DIR='${QT_DEPLOY_LIB_DIR}'")
# install(FILES
#     ${SHERPA_LIB_DIR}/libsherpa-ncnn-c-api.so
#     DESTINATION ${QT_DEPLOY_LIB_DIR}
# )