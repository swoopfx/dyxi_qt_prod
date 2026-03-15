import QtQuick 2.15
import QtQuick.Controls 2.15
// import QtMultimedia 5.15

Rectangle {
    id: root
    width: 1280
    height: 800
    color: "#F0F9FF"
    
    property string title: ""
    property bool showBackButton: true
    property bool showLogo: true
    
    // Background Image for ADHD engagement
    Image {
        id: backgroundImage
        source: "qrc:/assets/images/background.jpg"
        anchors.fill: parent
        opacity: 0.15 // Subtle background
        fillMode: Image.PreserveAspectCrop
    }

    // Background music (singleton-like or handled in main.qml is better, but here for simplicity)
    // Audio {
    //     id: bgMusic
    //     source: "qrc:/assets/sounds/background_music.mp3"
    //     autoPlay: true
    //     loops: Audio.Infinite
    //     volume: 0.3
    // }

    // Top Bar
    Item {
        id: topBar
        width: parent.width
        height: 100
        anchors.top: parent.top
        
        CustomButton {
            id: backBtn
            visible: root.showBackButton
            text: "←"
            fontSize: 32
            color: "#FF6B6B"
            width: 80
            height: 60
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.verticalCenter: parent.verticalCenter
            onClicked: pageStack.pop()
        }
        
        Image {
            id: logoImg
            visible: root.showLogo
            source: "../assets/logo.png"
            height: 80
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
        }
        
        Text {
            id: titleTxt
            visible: !root.showLogo && root.title !== ""
            text: root.title
            font.pixelSize: 40
            font.bold: true
            font.letterSpacing: 1.5
            color: "#4A90E2"
            anchors.centerIn: parent
        }
    }
    
    // Content area
    Item {
        id: contentArea
        anchors.top: topBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        
        // Dynamic content goes here via default property
        default property alias content: contentArea.children
    }
}
