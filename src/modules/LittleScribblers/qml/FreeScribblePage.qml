import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import LittleScribblers.Core

Page {
    id: scribblePage

    // Developer difficulty parameter definition
    property int difficulty: AppDifficulty.Level.Easy

    background: Rectangle {
        color: "#FAF7F2"
    }

    ScrollView {
        id: scribbleScrollView
        anchors.fill: parent
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        contentWidth: availableWidth

        ColumnLayout {
            width: scribbleScrollView.availableWidth
            spacing: 12

            // Top Toolbar
            RowLayout {
                Layout.fillWidth: true
                Layout.margins: 16
                spacing: 12

                Label {
                    text: qsTr("Free Scribble Canvas")
                    font.bold: true
                    font.pixelSize: 18
                    color: "#1C1917"
                }

                Rectangle {
                    color: "#FEF3C7"
                    radius: 8
                    border.color: "#F59E0B"
                    Layout.preferredHeight: 28
                    Layout.preferredWidth: 160
                    Label {
                        anchors.centerIn: parent
                        text: qsTr("Mode: %1").arg(AppDifficulty.getLabel(scribblePage.difficulty))
                        font.pixelSize: 11
                        font.bold: true
                        color: "#92400E"
                    }
                }

                Item { Layout.fillWidth: true }

                Button {
                    text: "✨ Clear"
                    onClicked: {
                        cppMediaController.playPop()
                        scribbleCanvas.requestPaint()
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: Math.max(420, scribblePage.height - 120)
                Layout.minimumHeight: 360
                Layout.leftMargin: 16
                Layout.rightMargin: 16
                Layout.bottomMargin: 16
                color: "#FFFFFF"
                radius: 16
                border.color: "#E7E5E4"
                border.width: 2
                clip: true

                Canvas {
                    id: scribbleCanvas
                    anchors.fill: parent
                    // Drawing engine implementation
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: cppMediaController.playSparkle()
                }
            }
        }
    }
}
