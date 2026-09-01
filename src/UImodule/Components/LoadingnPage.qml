import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    anchors.fill: parent
    color: "#ffffff" // Background color, can be changed to match the app theme

    // The logo container
    Image {
        id: logo
        source: "qrc:/ui/images/logo.png"
        anchors.centerIn: parent

        // "Small but visible" - setting a fixed small size or relative to parent
        width: Math.min(parent.width, parent.height) * 0.2
        height: width
        fillMode: Image.PreserveAspectFit
        smooth: true
        antialiasing: true

        // Pulsation Animation
        SequentialAnimation on scale {
            loops: Animation.Infinite
            running: true

            NumberAnimation {
                from: 1.0
                to: 1.15
                duration: 1000
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                from: 1.15
                to: 1.0
                duration: 1000
                easing.type: Easing.InOutQuad
            }
        }

        // Optional: Subtle opacity pulsation for a "breathing" effect
        SequentialAnimation on opacity {
            loops: Animation.Infinite
            running: true

            NumberAnimation {
                from: 0.8
                to: 1.0
                duration: 1000
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                from: 1.0
                to: 0.8
                duration: 1000
                easing.type: Easing.InOutQuad
            }
        }
    }
}
