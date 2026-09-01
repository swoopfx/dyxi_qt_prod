import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import "../Components"

Page {
    id: gameListPage

    property string pageTitle: ""
    required property var model
    property string avatar: "qrc:/ui/images/writing.png"
    property string  resoucesUrl: ""
    property string moduleName: ""
    property int typeId: 0
    property int id: 0

    // Palette customizer
    readonly property color colorBg: "#020617"
    readonly property color colorCard: "#0f172a"
    readonly property color colorAccent: "#38bdf8"
    readonly property color colorText: "#f8fafc"
    readonly property int gridSpacing: 24
    readonly property int cardRadius: 16

    background: Image {
        id: bg_image
        source: "qrc:/ui/images/kidi.jpg"
        fillMode: Image.PreserveAspectCrop
        mipmap: true
        smooth: true
        anchors.fill: parent
        opacity: 0.4
    }


    //     Rectangle {
    //     color: gameListPage.colorBg
    // }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

    // Header Section
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 20


        Text {
            text: ""
            font.pixelSize: 26
            font.bold: true
            color: gameListPage.colorText
        }



        // Separation line
        Rectangle {
            Layout.fillWidth: true
            height: 2
            color: gameListPage.colorAccent
            opacity: 0.4
            Layout.topMargin: 8
            Layout.bottomMargin: 8
        }
    }

    ScrollView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
    GridView {
        id: gridView
        anchors.fill: parent

        anchors.margins: 10
        cellWidth: parent.width / 3.2
        cellHeight: 320
        model: gameListPage.model
        delegate: delegate


        // spacing: 1 // Small spacing between delegates

        // ScrollIndicator.vertical: ScrollIndicator {
        //     id: verticalIndicator
        // }
    }
    }
}

    Component.onCompleted: {
        // console.log(gameItemListPage.model[0].gameName)
    }
    BackComponent{

    }

    Component{
        id: delegate

        Item {
            width: parent.width / 3.2
            height: 320

            // Main card container with padding around to create spacing between elements
            Rectangle {
                id: cardContainer
                anchors.fill: parent
                anchors.margins: gameListPage.gridSpacing
                radius: gameListPage.cardRadius
                color: gameListPage.colorCard
                border.color: hoverMouseArea.containsMouse ? gameListPage.colorAccent : "transparent"
                border.width: 1.5

                Behavior on border.color {
                    ColorAnimation {
                        duration: 150
                    }
                }

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Well-spaced Cover Image Area
                    Rectangle {
                        // Layout.fillWidth: true
                        // Layout.fillHeight: true
                        // color: Qt.lighter(gameListPage.colorCard, 1.2)
                        // radius: gameListPage.cardRadius
                        // clip: true

                        Layout.fillWidth: true
                           Layout.preferredHeight: 230
                           Layout.maximumHeight: 230
                           Layout.minimumHeight: 230

                           color: Qt.lighter(gameListPage.colorCard, 1.2)
                           radius: gameListPage.cardRadius
                           clip: true

                        Image {
                            anchors.fill: parent
                            source: gameListPage.avatar
                            fillMode: Image.PreserveAspectCrop
                            asynchronous: true
                            opacity: hoverMouseArea.containsMouse ? 0.95 : 0.8

                            // B/*ehavior on opacity {
                            //     NumberAnimation {
                            //         duration: 150
                            //     }
                            // }*/
                        }

                        // Genre visual tag in card top
                        // Rectangle {
                        //     anchors.top: parent.top
                        //     anchors.left: parent.left
                        //     anchors.margins: 10
                        //     color: Qt.rgba(0, 0, 0, 0.7)
                        //     radius: 4
                        //     width: genreText.contentWidth + 12
                        //     height: genreText.contentHeight + 6

                        //     Text {
                        //         id: genreText
                        //         anchors.centerIn: parent
                        //         text: ""
                        //         color: gameListPage.colorText
                        //         font.pixelSize: 10
                        //         font.bold: true
                        //     }
                        // }

                        // Info Icon Overlay in top-right for Popup
                        Rectangle {
                            id: infoIconButton
                            anchors.top: parent.top
                            anchors.right: parent.right
                            anchors.margins: 10
                            width: 32
                            height: 32
                            radius: 16
                            color: infoMouseArea.containsMouse ? gameListPage.colorAccent : Qt.rgba(0, 0, 0, 0.6)
                            border.color: "white"
                            border.width: 1
                            z: 10

                            Behavior on color {
                                ColorAnimation {
                                    duration: 100
                                }
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "i"
                                color: infoMouseArea.containsMouse ? gameListPage.colorBg : "white"
                                font.pixelSize: 15
                                font.bold: true
                            }

                            MouseArea {
                                id: infoMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    descriptionPopup.showPopup(model.title, model.description);
                                }
                            }
                        }
                    }

                    // Text Information Section
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 75
                        Layout.margins: 12
                        spacing: 4

                        Text {
                            text: modelData.games.gameName
                            color: gameListPage.colorText
                            font.pixelSize: 15
                            font.bold: true
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            // Text {
                            //     text: "⭐ " + model.rating
                            //     font.pixelSize: 11
                            //     color: "#FFCA28" // Gold rating star
                            // }

                            // Text {
                            //     text: "• " + model.developer
                            //     font.pixelSize: 11
                            //     color: Qt.rgba(gameListPage.colorText.r, gameListPage.colorText.g, gameListPage.colorText.b, 0.5)
                            //     elide: Text.ElideRight
                            //     Layout.fillWidth: true
                            // }
                        }
                    }
                }

                // Interactive Core Click leads to Page View
                MouseArea {
                    id: hoverMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    // Make sure info icon clicked area doesnt bubble up to the card page transition
                    propagateComposedEvents: true

                    // onClicked: {
                    //     // Ignore if clicking on info icon
                    //     var infoGlobalPos = infoIconButton.mapToItem(cardContainer, 0, 0);
                    //     if (mouseX >= infoGlobalPos.x && mouseX <= infoGlobalPos.x + infoIconButton.width && mouseY >= infoGlobalPos.y && mouseY <= infoGlobalPos.y + infoIconButton.height) {
                    //         return;
                    //     }

                    //     // Leads to detail screen
                    //     // mainStack.push(gameDetailComponent, {
                    //     //     "gameTitle": model.title,
                    //     //     "gameGenre": model.genre,
                    //     //     "gameRating": model.rating,
                    //     //     "gameCover": model.coverImage,
                    //     //     "gameScreenshot": model.screenshot,
                    //     //     "gameRelease": model.releaseDate,
                    //     //     "gameDev": model.developer,
                    //     //     "gameDesc": model.description
                    //     // });
                    // }

                    onClicked: {
                                          console.log("Main delegate clicked for item: " + String(modelData.gameName))
                                          // You can add your main delegate click action here
                                          stackView.push(
                                              // "qrc:/"+gameListPage.resoucesUrl+"/"+gameListPage.moduleName+"/"+modelData.games.gamePage+".qml",
                                              "qrc:/modules/modules/EntryPoint.qml",
                                                         {
                                                             "id":modelData.games.id,
                                                             "routeName":modelData.games.gamePage,
                                                             "typeId":gameListPage.typeId,
                                                  "stackView":stackView
                                                         })
                                      }
                }
            }
        }

    }


}
