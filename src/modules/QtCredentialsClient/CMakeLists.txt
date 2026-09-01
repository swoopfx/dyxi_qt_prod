# cmake_minimum_required(VERSION 3.20)

# project(QtCredentialsClient VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure modern Qt 6.10 component modules (Using NetworkAuth component module for multi-provider OAuth)
find_package(Qt6 6.10 REQUIRED COMPONENTS Core Gui Qml Quick Network NetworkAuth)

# Ensure automatic code generation is configured
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Setup executable target (resources are handled automatically by qt_add_qml_module)
# add_executable(QtCredentialsClient main.cpp)


# Create dynamic modern QML module mapping files to the executable
qt_add_qml_module(QtCredentialsClient
    STATIC
    URI QtCredentialsClient
    VERSION 1.0
    RESOURCE_PREFIX "/modules/oauth"
    SOURCES
        authservice.h
        authservice.cpp
        envconfig.h
        envconfig.cpp
    QML_FILES
        LoginMain.qml
        Login.qml
        WelcomePage.qml
)

# Link platform libraries for clean runtime executions
target_link_libraries(QtCredentialsClient PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Network
    Qt6::NetworkAuth
    qt6keychain
)


# Support linkage of modern QtKeychain keyring wrapper packages
# find_package(qtkeychain REQUIRED)
# target_link_libraries(QtCredentialsClient PRIVATE qtkeychain)

if(WIN32)
    # Enable API parameters constraints on Windows SDK layers
    target_compile_definitions(QtCredentialsClient PRIVATE NTDDI_VERSION=0x0A000000)
endif()
