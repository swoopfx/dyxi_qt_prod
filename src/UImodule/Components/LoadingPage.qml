import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Rectangle {
    id: root
    // width: 600
    // height: 800
    anchors.fill: parent
    visible: true
    // title: "DYXI Loading Page"
    color: "#FFFFFF"

    // Main Container
    Item {
        anchors.fill: parent

        // Logo Container with floating animation
        Item {
            id: logoContainer
            width: 300
            height: 300
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -50

            Image {
                id: logoImage
                source: "qrc:/ui/images/logo.png" // Ensure the logo is in the same directory
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                opacity: 0
                scale: 0.8

                // Initial Entrance Animation
                Component.onCompleted: {
                    entranceAnim.start()
                }

                ParallelAnimation {
                    id: entranceAnim
                    NumberAnimation { target: logoImage; property: "opacity"; to: 1; duration: 1000; easing.type: Easing.OutCubic }
                    NumberAnimation { target: logoImage; property: "scale"; to: 1; duration: 1000; easing.type: Easing.OutBack }
                }

                // Continuous Floating Animation
                SequentialAnimation on y {
                    loops: Animation.Infinite
                    running: logoImage.opacity === 1
                    NumberAnimation { from: 0; to: -15; duration: 2000; easing.type: Easing.InOutQuad }
                    NumberAnimation { from: -15; to: 0; duration: 2000; easing.type: Easing.InOutQuad }
                }
            }

            // Glow effect behind the logo
            Rectangle {
                anchors.centerIn: parent
                width: 200
                height: 200
                radius: 100
                color: "#8A2BE2" // Purple from logo
                opacity: 0.1
                z: -1
                
                SequentialAnimation on scale {
                    loops: Animation.Infinite
                    NumberAnimation { from: 1; to: 1.5; duration: 3000; easing.type: Easing.InOutSine }
                    NumberAnimation { from: 1.5; to: 1; duration: 3000; easing.type: Easing.InOutSine }
                }
            }
        }

        // Loading Progress Section
        Column {
            anchors.top: logoContainer.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
            width: parent.width * 0.6

            // Custom Progress Bar
            Rectangle {
                width: parent.width
                height: 6
                color: "#F0F0F0"
                radius: 3
                clip: true

                Rectangle {
                    id: progressBar
                    height: parent.height
                    width: 0
                    radius: 3
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.0; color: "#8A2BE2" } // Purple
                        GradientStop { position: 1.0; color: "#00BFFF" } // Blue
                    }

                    NumberAnimation on width {
                        id: progressAnim
                        from: 0
                        to: parent.width
                        duration: 5000
                        easing.type: Easing.InOutSine
                        running: true
                    }
                }
            }

            // Loading Text
            Text {
                id: loadingText
                text: "Loading Experience..."
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 16
                font.family: "Segoe UI", "Roboto", "Arial"
                color: "#333333"
                opacity: 0.7

                SequentialAnimation on opacity {
                    loops: Animation.Infinite
                    NumberAnimation { from: 0.3; to: 0.8; duration: 1000; easing.type: Easing.InOutSine }
                    NumberAnimation { from: 0.8; to: 0.3; duration: 1000; easing.type: Easing.InOutSine }
                }
            }
        }

        // Footer version/info
        Text {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            text: "v1.0.42"
            font.pixelSize: 12
            color: "#CCCCCC"
        }
    }
}
