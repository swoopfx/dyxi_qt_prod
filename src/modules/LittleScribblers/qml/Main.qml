import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import LittleScribblers.Core

ApplicationWindow {
    id: rootWindow
    visible: true
    // Landscape tablet dimensions: 1280x800 with responsive constraints
    width: 1280
    height: 800
    minimumWidth: 800
    minimumHeight: 520
    title: qsTr("Little Scribblers - Qt 6.11 Landscape Tablet Edition")
    color: "#FAF7F2"

    // Global developer difficulty setting passed down to each App Type Page
    property int developerDifficulty: AppDifficulty.Level.Medium

    header: ToolBar {
        background: Rectangle {
            color: "#FFFFFF"
            border.color: "#E7E5E4"
            border.width: 1
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16
            spacing: 12

            Button {
                text: stackView.depth > 1 ? "← Back" : "🏠 Home"
                onClicked: {
                    cppMediaController.playPop()
                    if (stackView.depth > 1) {
                        stackView.pop()
                    }
                }
            }

            Label {
                text: "Little Scribblers (Qt 6.11 / QML)"
                font.bold: true
                font.pixelSize: 16
                color: "#1C1917"
            }

            Item { Layout.fillWidth: true }

            // Developer Difficulty Parameter Input
            Label {
                text: "Dev Difficulty:"
                font.bold: true
                font.pixelSize: 12
                color: "#78716C"
            }

            ComboBox {
                id: difficultySelector
                model: ["Easy (48px Corridor)", "Medium (32px Corridor)", "Hard (20px Corridor)"]
                currentIndex: 1
                onCurrentIndexChanged: {
                    if (currentIndex === 0) rootWindow.developerDifficulty = AppDifficulty.Level.Easy;
                    else if (currentIndex === 1) rootWindow.developerDifficulty = AppDifficulty.Level.Medium;
                    else rootWindow.developerDifficulty = AppDifficulty.Level.Hard;

                    cppTracingEngine.difficulty = rootWindow.developerDifficulty
                    cppMediaController.playSparkle()
                }
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: shapeSelectorPage
    }

    Component {
        id: shapeSelectorPage
        ShapeSelectorPage {
            difficulty: rootWindow.developerDifficulty
            onShapeSelected: (shapeId) => {
                stackView.push(shapeTracingPageComponent, {
                    shapeId: shapeId,
                    difficulty: rootWindow.developerDifficulty
                })
            }
            onOpenFreeDraw: {
                stackView.push(freeScribblePageComponent, {
                    difficulty: AppDifficulty.Level.Easy
                })
            }
            onOpenGallery: {
                stackView.push(artGalleryPageComponent, {
                    difficulty: rootWindow.developerDifficulty
                })
            }
        }
    }

    Component {
        id: shapeTracingPageComponent
        ShapeTracingPage {}
    }

    Component {
        id: freeScribblePageComponent
        FreeScribblePage {}
    }

    Component {
        id: artGalleryPageComponent
        ArtGalleryPage {}
    }
}
