import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * NoPageFound.qml
 * A simple 404 error page with Back and Home buttons centered.
 */
Page {
    id: root
    visible: true
    
    background: Rectangle {
        color: "#f4f4f4"
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 30

        Column {
            spacing: 10
            Layout.alignment: Qt.AlignHCenter

            Text {
                text: qsTr("404")
                font.pixelSize: 64
                font.bold: true
                color: "#333"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: qsTr("Oops! Page Not Found")
                font.pixelSize: 24
                color: "#666"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        RowLayout {
            spacing: 20
            Layout.alignment: Qt.AlignHCenter

            Button {
                id: backButton
                text: qsTr("Back")
                flat: false
                highlighted: false
                
                contentItem: Text {
                    text: backButton.text
                    font.pixelSize: 16
                    color: backButton.down ? "#000" : "#444"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    // Add your navigation logic here, e.g., stackView.pop()
                    console.log("Back button clicked")
                }
            }

            Button {
                id: homeButton
                text: qsTr("Home")
                highlighted: true
                
                contentItem: Text {
                    text: homeButton.text
                    font.pixelSize: 16
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 40
                    color: homeButton.down ? "#005a9e" : "#0078d4"
                    radius: 4
                }

                onClicked: {
                    // Add your navigation logic here, e.g., stackView.replace("Home.qml")
                    console.log("Home button clicked")
                }
            }
        }
    }
}
