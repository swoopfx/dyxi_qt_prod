
# Configure modern Qt 6.10 component modules (Using NetworkAuth component module for multi-provider OAuth)
find_package(Qt6 6.10 REQUIRED COMPONENTS Core Gui Qml Quick Network NetworkAuth)


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


if(WIN32)
    # Enable API parameters constraints on Windows SDK layers
    target_compile_definitions(QtCredentialsClient PRIVATE NTDDI_VERSION=0x0A000000)
endif()
