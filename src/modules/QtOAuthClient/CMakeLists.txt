cmake_minimum_required(VERSION 3.16)

project(QtOAuthClient VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure Qt 6 component modules
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network NetworkAuth)

# Ensure automatic code generation is configured
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Source files list for unified cross-platform build target

qt_add_qml_module(QtOAuthClient
    STATIC
    URI QtOAuthClient
    VERSION 1.0
    RESOURCE_PREFIX "/modules/auth"
    QML_FILES
     Login.qml
     LoginMain.qml
     WelcomePage.qml

    SOURCES
    authservice.h authservice.cpp
     # main.cpp

    RESOURCES
         resources.qrc
)

# Compile-time overrides for mobile platform capabilities
# if(ANDROID)
#     add_compile_definitions(QT_OAUTH_MOBILE_BUILD=1 ANDROID_MOBILE_PLATFORM=1)
# elseif(IOS)
#     add_compile_definitions(IOS_TABLET_PLATFORM=1)
# endif()

# # Setup target binary / Apple application bundle
# if(IOS)
#     add_executable(QtOAuthClient MACOSX_BUNDLE ${SOURCES})
# else()
#     add_executable(QtOAuthClient ${SOURCES})
# endif()

# Platform-specific properties configurations
if(ANDROID)
    # Define directory holding native android files like AndroidManifest.xml, icons, and assets
    set_target_properties(QtOAuthClient PROPERTIES
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
    )
elseif(IOS)
    # Define Apple bundle properties tailored for iOS/iPadOS tablet dimensions and system keychain access
    set_target_properties(QtOAuthClient PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
        
        # KEY SETTING FOR TABLET: 
        # 1: iPhone, 2: iPad (Tablet), 1,2: Universal (both iPhone and iPad)
        # We explicitly configure Universal '1,2' and '2' to fully support elegant high-contrast tablet configurations.
        XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
        
        # Force oriented multitasking support for standard iPad models
        XCODE_ATTRIBUTE_UISupportedInterfaceOrientations "UIInterfaceOrientationPortrait,UIInterfaceOrientationPortraitUpsideDown,UIInterfaceOrientationLandscapeLeft,UIInterfaceOrientationLandscapeRight"
        
        # Keychain access isolation groups (iOS isolation)
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/ios/Entitlements.plist"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "ABC123XYZ4"
    )
endif()

# Link platform libraries for clean runtime executions
target_link_libraries(QtOAuthClient PRIVATE

    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Network
    Qt6::NetworkAuth
        qt6keychain

)

# Support linkage of third-party QtKeychain wrapper builds 
# For mobile platforms (Android/iOS) and desktops, discover the qtkeychain package and link it
#find_package(qtkeychain REQUIRED)


# The target will resolve to "qtkeychain" automatically
#target_link_libraries(QtOAuthApp PRIVATE qtkeychain)
# target_link_libraries(QtOAuthClient PRIVATE qtkeychain)



if(WIN32)
    # Enable API parameters constraints on Windows SDK layers
    target_compile_definitions(QtOAuthClient PRIVATE NTDDI_VERSION=0x0A000000)
endif()
