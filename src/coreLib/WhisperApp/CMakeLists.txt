

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (ANDROID)
    # set(GGML_NATIVE OFF CACHE BOOL "" FORCE)
    # set(GGML_OPENMP OFF CACHE BOOL "" FORCE)
    # set(GGML_CPU_NATIVE OFF CACHE BOOL "" FORCE)
    # set(GGML_CPU_FLAGS "" CACHE STRING "" FORCE)
    # set(GGML_NATIVE OFF CACHE BOOL "" FORCE)

set(GGML_VULKAN OFF CACHE BOOL "Disable Vulkan" FORCE)
    set(GGML_CPU_ALL_VARIANTS OFF CACHE BOOL "" FORCE)
    set(GGML_CPU_AARCH64 ON CACHE BOOL "" FORCE)
    set(GGML_NATIVE OFF CACHE BOOL "" FORCE)
    set(GGML_LLAMAFILE OFF CACHE BOOL "" FORCE)
    set(GGML_CPU_REPACK OFF CACHE BOOL "" FORCE)
endif()

# if (ANDROID)
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=generic")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=generic")
# endif()



set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# -------------------------
# Qt
# -------------------------
find_package(Qt6 REQUIRED COMPONENTS Core Qml Network)
qt_standard_project_setup()

# -------------------------
# Whisper.cpp (IMPORTANT)
# -------------------------
set(WHISPER_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/whisper.cpp)


add_subdirectory(${WHISPER_ROOT})


qt_add_qml_module(WhisperApp
    STATIC
    URI WhisperApp

    RESOURCE_PREFIX "/whisper"
    SOURCES whisperworker.cpp whisperworker.h


)


target_link_libraries(WhisperApp PRIVATE whisper_wrapper)
# Link Libraries (whisper requires threading capabilities)
find_package(Threads REQUIRED)



target_include_directories(WhisperApp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Platform Specific Optimizations
if(APPLE)
    # Enable Apple hardware acceleration optimizations via Metal and Accelerate
    target_compile_definitions(WhisperApp PRIVATE GGML_USE_ACCELERATE)
    target_link_libraries(WhisperApp PRIVATE
        "-framework Accelerate"
        "-framework Foundation"
        "-framework Metal"
    )
elseif(ANDROID)
    # Enable native optimizations for ARMv8 systems
    target_compile_definitions(WhisperApp PRIVATE GGML_USE_NEON)
    find_library(LOG_LIB log)
    target_link_libraries(WhisperApp PRIVATE ${LOG_LIB})
    target_compile_definitions(WhisperApp PRIVATE PLATFORM_ANDROID)

      target_compile_options(WhisperApp PRIVATE
          -O3
      )
endif()

target_link_libraries(WhisperApp PRIVATE
    Qt6::Core
    Qt6::Qml
    Qt6::Network
    whisper
    ggml
    Threads::Threads
)
target_link_libraries(WhisperApp PRIVATE whisper_wrapper)