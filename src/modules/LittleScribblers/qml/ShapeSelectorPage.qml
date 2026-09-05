import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import LittleScribblers.Core

Page {
    id: selectorPage

    // Developer difficulty parameter definition
    property int difficulty: AppDifficulty.Level.Medium

    signal shapeSelected(string shapeId)
    signal openFreeDraw()
    signal openGallery()

    background: Rectangle {
        color: "#FAF7F2"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        // Page Header
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            ColumnLayout {
                Label {
                    text: qsTr("Choose a Shape to Trace")
                    font.bold: true
                    font.pixelSize: 22
                    color: "#1C1917"
                }
                Label {
                    text: qsTr("Developer Difficulty: %1 (Corridor: %2px)")
                        .arg(AppDifficulty.getLabel(selectorPage.difficulty))
                        .arg(AppDifficulty.getCorridorTolerance(selectorPage.difficulty))
                    font.pixelSize: 13
                    color: "#78716C"
                }
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "🎨 Free Scribble"
                onClicked: {
                    cppMediaController.playSparkle()
                    selectorPage.openFreeDraw()
                }
            }

            Button {
                text: "🖼️ Gallery"
                onClicked: {
                    cppMediaController.playPop()
                    selectorPage.openGallery()
                }
            }
        }

        // Shapes Grid
        GridView {
            id: shapesGrid
            Layout.fillWidth: true
            Layout.fillHeight: true
            cellWidth: Math.max(180, Math.floor((width - 16) / Math.max(3, Math.floor((width - 16) / 200))))
            cellHeight: 180
            clip: true
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            model: ListModel {
                ListElement { shapeId: "circle"; name: "Circle"; iconText: "⚪"; colorHex: "#EF4444" }
                ListElement { shapeId: "square"; name: "Square"; iconText: "🟧"; colorHex: "#F97316" }
                ListElement { shapeId: "triangle"; name: "Triangle"; iconText: "🔺"; colorHex: "#EAB308" }
                ListElement { shapeId: "star"; name: "Star"; iconText: "⭐"; colorHex: "#10B981" }
                ListElement { shapeId: "heart"; name: "Heart"; iconText: "💖"; colorHex: "#EC4899" }
                ListElement { shapeId: "diamond"; name: "Diamond"; iconText: "🔷"; colorHex: "#6366F1" }
            }

            delegate: Item {
                width: shapesGrid.cellWidth
                height: shapesGrid.cellHeight

                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 8
                    radius: 16
                    color: "#FFFFFF"
                    border.color: "#E7E5E4"
                    border.width: 1

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 8
                        Text {
                            text: model.iconText
                            font.pixelSize: 42
                            Layout.alignment: Qt.AlignHCenter
                        }
                        Label {
                            text: model.name
                            font.bold: true
                            font.pixelSize: 15
                            color: "#1C1917"
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            cppMediaController.playPop()
                            selectorPage.shapeSelected(model.shapeId)
                        }
                    }
                }
            }
        }
    }
}
