import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import LittleScribblers.Core

Page {
    id: galleryPage

    property int difficulty: AppDifficulty.Level.Easy

    background: Rectangle {
        color: "#FAF7F2"
    }

    ScrollView {
        id: galleryScrollView
        anchors.fill: parent
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        contentWidth: availableWidth

        ColumnLayout {
            width: galleryScrollView.availableWidth
            spacing: 16

            RowLayout {
                Layout.fillWidth: true
                Layout.margins: 20
                Label {
                    text: qsTr("My Art Gallery")
                    font.bold: true
                    font.pixelSize: 22
                    color: "#1C1917"
                }
                Item { Layout.fillWidth: true }
                Label {
                    text: qsTr("Saved Masterpieces")
                    font.pixelSize: 13
                    color: "#78716C"
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: Math.max(450, galleryPage.height - 120)
                Layout.minimumHeight: 380
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.bottomMargin: 20
                color: "#FFFFFF"
                radius: 16
                border.color: "#E7E5E4"
                border.width: 1

                Label {
                    anchors.centerIn: parent
                    text: qsTr("🎨 All pinned creations and traced shapes appear here.")
                    font.pixelSize: 14
                    color: "#A8A29E"
                }
            }
        }
    }
}
