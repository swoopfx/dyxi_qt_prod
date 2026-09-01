import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: window
    readonly property color colorBg: "#020617"
    readonly property color colorCard: "#0f172a"
    readonly property color colorAccent: "#38bdf8"
    readonly property color colorText: "#f8fafc"
    readonly property int gridSpacing: 24
    readonly property int cardRadius: 16
    readonly property string homeUrl: "qrc:/ui/UIModule/Pages/SelectGameCategoryPage.qml"
    Row {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 16
        spacing: 12

        // Back Button
        Rectangle {
            width: 80
            height: 36
            radius: 18
            color: backMouse.containsMouse ? window.colorAccent : Qt.rgba(0, 0, 0, 0.6)
            border.color: "white"
            border.width: 1

            Behavior on color {
                ColorAnimation {
                    duration: 100
                }
            }

            RowLayout {
                anchors.centerIn: parent
                spacing: 4

                Text {
                    text: "←"
                    color: backMouse.containsMouse ? window.colorBg : "white"
                    font.pixelSize: 16
                    font.bold: true
                }

                Text {
                    text: "Back"
                    color: backMouse.containsMouse ? window.colorBg : "white"
                    font.pixelSize: 12
                    font.bold: true
                }
            }

            MouseArea {
                id: backMouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: stackView.pop()
            }
        }

        // Home Button
        Rectangle {
            width: 100
            height: 36
            radius: 18
            color: homeBackMouse.containsMouse ? window.colorAccent : Qt.rgba(0, 0, 0, 0.6)
            border.color: "white"
            border.width: 1

            Behavior on color {
                ColorAnimation {
                    duration: 100
                }
            }

            RowLayout {
                anchors.centerIn: parent
                spacing: 4

                Text {
                    text: "🏠"
                    color: homeBackMouse.containsMouse ? window.colorBg : "white"
                    font.pixelSize: 16
                }

                Text {
                    text: "Home"
                    color: homeBackMouse.containsMouse ? window.colorBg : "white"
                    font.pixelSize: 12
                    font.bold: true
                }
            }

            MouseArea {
                id: homeBackMouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: stackView.replace(homeUrl)
            }
        }
    }
}
