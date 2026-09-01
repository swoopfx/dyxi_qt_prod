cmake_minimum_required(VERSION 3.22)

project(QtOAuth VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.11 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    Network
    NetworkAuth

)

# Optional: find qtkeychain library (or use bundled fallback)
find_package(Qt6Keychain QUIET)
if(NOT Qt6Keychain_FOUND)
    find_package(QtKeychain QUIET)
endif()

qt_standard_project_setup(REQUIRES 6.11)



qt_add_qml_module(QtOAuth
     STATIC
    URI QtOAuth
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/Login.qml
        qml/WelcomePage.qml
    SOURCES
        src/authservice.h
        src/authservice.cpp
        keychainhelper.h
        keychainhelper.cpp
        src/envconfig.h
        src/envconfig.cpp
)

target_include_directories(QtOAuth PRIVATE src)

target_link_libraries(QtOAuth PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Network
    Qt6::NetworkAuth
     qt6keychain
)

# if(TARGET qt6keychain)
#     target_link_libraries(QtOAuth PRIVATE qt6keychain)
# elseif(TARGET Qt6Keychain::Qt6Keychain)
#     target_link_libraries(QtOAuth PRIVATE Qt6Keychain::Qt6Keychain)
# endif()

# Deployment and install settings
install(TARGETS QtOAuth
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
