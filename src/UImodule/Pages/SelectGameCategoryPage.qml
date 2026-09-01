import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import "../Components"
import General
import ShapeRecog
import Writing
import MathDice2

Page {
    id: selectGamePage
    property string activeUserName: ""
    property string userAge: ""
    property var gameTypeId: ""
    property var gameCategoryId: ""
    property var gameProgrmId: ""

    // property var gameTypeId: {}
    // anchors.fill: parent

    CoreSettings {
        id: coreSettings
    }

    NetworkAccessPresets {
        id: networkPresets
    }

    Image {
        id: bg_image
        source: "qrc:/ui/images/playground.jpg"
        fillMode: Image.PreserveAspectCrop
        mipmap: true
        smooth: true
        anchors.fill: parent
    }

    ScrollView {
        // anchors.fill: parent
        anchors.fill: parent
        anchors.margins: 40

        Flickable {
            id: flick
            anchors.fill: parent
            contentHeight: contentColumn.implicitHeight
            clip: true

            Column {
                id: contentColumn
                width: flick.width
                spacing: 20

                // =========================
                // Literacy Games
                // =========================
                Rectangle {
                    width: parent.width
                    height: 50
                    radius: 10
                    color: "#E8F5E9"

                    Text {
                        anchors.centerIn: parent
                        text: "📚 Game type"
                        font.pixelSize: 22
                        font.bold: true
                        color: "#2E7D32"
                    }
                }

                // First section
                GridLayout {
                    id: gridTop
                    width: parent.width
                    // columns: 4
                    columnSpacing: 15
                    rowSpacing: 15

                    GameItem {
                        labelText: "Phonics Fun"
                        imageSource: "qrc:/ui/images/phonics.png"
                        onClicked: stackView.push("GameLoaderPage.qml", {
                            // "url": coreSettings.baseUrl+"/api/game-by-type"+"?type=1"+"age"+UserSession.userAge,
                            "url": coreSettings.dyxiGetGameList + "?type=" + gameTypeId[0].id,
                            "pageTitle": selectGamePage.gameTypeId[0].type,
                            "avatar": "qrc:/ui/images/phonics.png",
                            "resoucesUrl": selectGamePage.gameTypeId[0].type,
                            "stackView": stackView
                        })
                    }

                    GameItem {
                        labelText: "Word Tracing"
                        imageSource: "qrc:/ui/images/tracing.png"
                        onClicked: stackView.push("GameLoaderPage.qml", {
                            "url": coreSettings.dyxiGetGameList + "?type=" + gameTypeId[2].id,
                            "pageTitle": selectGamePage.gameTypeId[2].type,
                            "avatar": "qrc:/ui/images/tracing.png",
                            "resoucesUrl": selectGamePage.gameTypeId[2].type,
                            "stackView": stackView
                        })
                    }

                    GameItem {
                        labelText: "Cognition"
                        imageSource: "qrc:/ui/images/recognition.png"
                        onClicked: stackView.push("GameLoaderPage.qml", {
                            "url": coreSettings.dyxiGetGameList + "?type=" + gameTypeId[3].id,
                            "pageTitle": selectGamePage.gameTypeId[3].type,
                            "avatar": "qrc:/ui/images/recognition.png",
                            "resoucesUrl": selectGamePage.gameTypeId[3].type,
                            "stackView": stackView
                        })
                    }

                    GameItem {
                        labelText: "Reading"
                        imageSource: "qrc:/ui/images/reading.png"
                        onClicked: stackView.push("GameLoaderPage.qml", {
                            "url": coreSettings.dyxiGetGameList + "?type=" + gameTypeId[5].id,
                            "pageTitle": selectGamePage.gameTypeId[5].type,
                            "avatar": "qrc:/ui/images/reading.png",
                            "resoucesUrl": selectGamePage.gameTypeId[5].type
                        })
                    }
                }

                // Horizontal spacer / divider
                Rectangle {
                    width: parent.width
                    height: 3
                    color: "#38bdf8"
                    opacity: 0.4
                }

                // =========================
                // Literacy Games
                // =========================
                Rectangle {
                    width: parent.width
                    height: 50
                    radius: 10
                    color: "#E8F5E9"

                    Text {
                        anchors.centerIn: parent
                        text: "🧠 Game Category"
                        font.pixelSize: 22
                        font.bold: true
                        color: "#2E7D32"
                    }
                }

                // Second section
                GridLayout {
                    id: gridBottom
                    width: parent.width
                    // columns: 4
                    columnSpacing: 20
                    rowSpacing: 20

                    GameItem {
                        labelText: "Dyscalculia"
                        imageSource: "qrc:/ui/images/dyscalculia.png"
                        onClicked: stackView.push("GameLoaderPage.qml", {
                            "url": coreSettings.dyxiGetGameCategoryList + "?cat=" + gameCategoryId[2].id,
                            "pageTitle": selectGamePage.gameTypeId[0].gameCategory,
                            "avatar": "qrc:/ui/images/dyscalculia.png",
                            "resoucesUrl": selectGamePage.gameTypeId[0].gameCategory
                        })
                    }

                    GameItem {
                        labelText: "Dyslexia"
                        imageSource: "qrc:/ui/images/dyslexia.png"
                        onClicked: stackView.push("GameLoaderPage.qml", {
                            "url": coreSettings.dyxiGetGameCategoryList + "?cat=" + gameCategoryId[0].id,
                            "pageTitle": selectGamePage.gameTypeId[2].gameCategory,
                            "avatar": "qrc:/ui/images/dyslexia.png",
                            "resoucesUrl": selectGamePage.gameTypeId[2].gameCategory
                        })
                    }
                    GameItem {
                        labelText: "ADHD"
                        imageSource: "qrc:/ui/images/adhd.png"
                        onClicked: stackView.push("GameLoaderPage.qml", {
                            "url": coreSettings.dyxiGetGameCategoryList + "?cat=" + gameCategoryId[1].id,
                            "pageTitle": selectGamePage.gameTypeId[2].gameCategory,
                            "avatar": "qrc:/ui/images/adhd.png",
                            "resoucesUrl": selectGamePage.gameTypeId[2].gameCategory
                        })
                    }
                    GameItem {
                        labelText: "Dysgraphia"
                        imageSource: "qrc:/ui/images/writing.png"
                        onClicked: stackView.push("GameLoaderPage.qml", {
                            "url": coreSettings.dyxiGetGameCategoryList + "?cat=" + gameCategoryId[3].id,
                            "pageTitle": selectGamePage.gameTypeId[2].gameCategory,
                            "avatar": "qrc:/ui/images/writing.png",
                            "resoucesUrl": selectGamePage.gameTypeId[2].gameCategory
                        })
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        // console.log(coreSettings.dyxiGameTypeIds);
        networkPresets.getGameTypeIdRequest(coreSettings.dyxiGameTypeIds);
        networkPresets.getGameCategoryIdRequest(coreSettings.dyxiGameCategoryIds);
        networkPresets.getProgramsIdRequest(coreSettings.dyxiGameProgramIds);
    }

    Connections {
        target: networkPresets
        function onGameTypeIdsChanged() {
            if (true) {
                selectGamePage.gameTypeId = networkPresets.gameTypeIds;
                selectGamePage.gameCategoryId = networkPresets.gameCategoryIds;
                selectGamePage.gameProgrmId = networkPresets.gameProgramIds;
                console.log(selectGamePage.gameCategoryId[0].gameCategory);
            }
        }

        // function
    }
}

/**

🧠 Brain Training
📖 Reading Practice
🔤 Letter Learning
🌟 Reading Helper
💙 Learning Support
🧩 Word Builder
🚀 Learning Journey

🤝 Learning Support
💙 Reading Support
🌟 Guided Learning
🧭 Learning Path
🔤 Letter Helper
📖 Reading Companion
🧠 Brain Support
🌈 Learning Adventure
🚀 Learning Journey
⭐ Star Learner
🎉 Fun Learning
🦉 Wise Owl Mode
🌻 Growth Mode
🎨 Creative Learning
✨ Discovery Time

🧠 Memory Training
🎴 Memory Match
⚡ Brain Boost
🔄 Recall Practice
🎯 Concentration Mode
🏅 Skill Builder
🧩 Cognitive Challenge

🌟 Reading Helper
📖 Reading Practice
🔤 Letter Learning
🔡 Word Builder
📝 Reading Skills
🎯 Focus Reading
🧩 Word Puzzle
💡 Learn & Read
🌈 Easy Reading

  **/
