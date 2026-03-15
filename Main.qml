import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import "pages"

Window {
    id: window
    width: 1280
    height: 800
    visible: true
    title: "DYXI - Kids Education Game"
    color: "#F0F9FF"

    // Dyslexia-friendly: use a high-readability font if available
    // font.family: "OpenDyslexic"

    StackView {
        id: pageStack
        anchors.fill: parent
        initialItem: "pages/LoginPage.qml"

        // Custom transitions for ADHD: smooth but not distracting
        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0; to: 1
                duration: 400
            }
            PropertyAnimation {
                property: "x"
                from: window.width; to: 0
                duration: 400
                easing.type: Easing.OutQuad
            }
        }

        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1; to: 0
                duration: 400
            }
        }

        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0; to: 1
                duration: 400
            }
        }

        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1; to: 0
                duration: 400
            }
            PropertyAnimation {
                property: "x"
                from: 0; to: window.width
                duration: 400
                easing.type: Easing.OutQuad
            }
        }
    }
}
