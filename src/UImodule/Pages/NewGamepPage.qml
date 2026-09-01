import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: window
    visible: true
    width: 960
    height: 640
    title: "Nexus Game Library"

    // Palette customizer
    readonly property color colorBg: "#020617"
    readonly property color colorCard: "#0f172a"
    readonly property color colorAccent: "#38bdf8"
    readonly property color colorText: "#f8fafc"
    readonly property int gridSpacing: 24
    readonly property int cardRadius: 16

    background: Rectangle {
        color: window.colorBg
    }

    // StackView deals with page navigation smoothly
    StackView {
        id: mainStack
        anchors.fill: parent
        initialItem: gameGridComponent

        pushEnter: Transition {
            NumberAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 250
                easing.type: Easing.OutQuad
            }
            NumberAnimation {
                property: "scale"
                from: 0.95
                to: 1.0
                duration: 250
                easing.type: Easing.OutQuad
            }
        }
        pushExit: Transition {
            NumberAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 200
            }
        }
        popEnter: Transition {
            NumberAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 250
            }
        }
        popExit: Transition {
            NumberAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 200
                easing.type: Easing.InQuad
            }
            NumberAnimation {
                property: "scale"
                from: 1.0
                to: 0.95
                duration: 200
                easing.type: Easing.InQuad
            }
        }
    }

    // ==========================================
    // 1. GAME GRID SCREEN COMPONENT (Main View)
    // ==========================================
    Component {
        id: gameGridComponent

        Page {
            background: Rectangle {
                color: window.colorBg
            }

            // Split into Header and Scrollable Grid Layout
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                // Header Section
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Text {
                        text: "Nexus Game Library"
                        font.pixelSize: 26
                        font.bold: true
                        color: window.colorText
                    }

                    Text {
                        text: "Archive of high-fidelity interactive experiences and technical benchmarks."
                        font.pixelSize: 14
                        color: Qt.rgba(window.colorText.r, window.colorText.g, window.colorText.b, 0.6)
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }

                    // Separation line
                    Rectangle {
                        Layout.fillWidth: true
                        height: 2
                        color: window.colorAccent
                        opacity: 0.4
                        Layout.topMargin: 8
                        Layout.bottomMargin: 8
                    }
                }

                // Scrollable Grid Showcase
                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    ScrollBar.vertical.policy: ScrollBar.AsNeeded

                    GridView {
                        id: gridView
                        anchors.fill: parent
                        model: gamesModel
                        cellWidth: parent.width / 3
                        cellHeight: 320
                        snapMode: GridView.SnapToRow

                        delegate: Item {
                            width: gridView.cellWidth
                            height: gridView.cellHeight

                            // Main card container with padding around to create spacing between elements
                            Rectangle {
                                id: cardContainer
                                anchors.fill: parent
                                anchors.margins: window.gridSpacing
                                radius: window.cardRadius
                                color: window.colorCard
                                border.color: hoverMouseArea.containsMouse ? window.colorAccent : "transparent"
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
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        color: Qt.lighter(window.colorCard, 1.2)
                                        radius: window.cardRadius
                                        clip: true

                                        Image {
                                            anchors.fill: parent
                                            source: model.coverImage
                                            fillMode: Image.PreserveAspectCrop
                                            asynchronous: true
                                            opacity: hoverMouseArea.containsMouse ? 0.95 : 0.8

                                            Behavior on opacity {
                                                NumberAnimation {
                                                    duration: 150
                                                }
                                            }
                                        }

                                        // Genre visual tag in card top
                                        Rectangle {
                                            anchors.top: parent.top
                                            anchors.left: parent.left
                                            anchors.margins: 10
                                            color: Qt.rgba(0, 0, 0, 0.7)
                                            radius: 4
                                            width: genreText.contentWidth + 12
                                            height: genreText.contentHeight + 6

                                            Text {
                                                id: genreText
                                                anchors.centerIn: parent
                                                text: model.genre
                                                color: window.colorText
                                                font.pixelSize: 10
                                                font.bold: true
                                            }
                                        }

                                        // Info Icon Overlay in top-right for Popup
                                        Rectangle {
                                            id: infoIconButton
                                            anchors.top: parent.top
                                            anchors.right: parent.right
                                            anchors.margins: 10
                                            width: 32
                                            height: 32
                                            radius: 16
                                            color: infoMouseArea.containsMouse ? window.colorAccent : Qt.rgba(0, 0, 0, 0.6)
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
                                                color: infoMouseArea.containsMouse ? window.colorBg : "white"
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
                                            text: model.title
                                            color: window.colorText
                                            font.pixelSize: 15
                                            font.bold: true
                                            elide: Text.ElideRight
                                            Layout.fillWidth: true
                                        }

                                        RowLayout {
                                            Layout.fillWidth: true
                                            spacing: 4

                                            Text {
                                                text: "⭐ " + model.rating
                                                font.pixelSize: 11
                                                color: "#FFCA28" // Gold rating star
                                            }

                                            Text {
                                                text: "• " + model.developer
                                                font.pixelSize: 11
                                                color: Qt.rgba(window.colorText.r, window.colorText.g, window.colorText.b, 0.5)
                                                elide: Text.ElideRight
                                                Layout.fillWidth: true
                                            }
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

                                    onClicked: {
                                        // Ignore if clicking on info icon
                                        var infoGlobalPos = infoIconButton.mapToItem(cardContainer, 0, 0);
                                        if (mouseX >= infoGlobalPos.x && mouseX <= infoGlobalPos.x + infoIconButton.width && mouseY >= infoGlobalPos.y && mouseY <= infoGlobalPos.y + infoIconButton.height) {
                                            return;
                                        }

                                        // Leads to detail screen
                                        mainStack.push(gameDetailComponent, {
                                            "gameTitle": model.title,
                                            "gameGenre": model.genre,
                                            "gameRating": model.rating,
                                            "gameCover": model.coverImage,
                                            "gameScreenshot": model.screenshot,
                                            "gameRelease": model.releaseDate,
                                            "gameDev": model.developer,
                                            "gameDesc": model.description
                                        });
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // ==========================================
            // POPUP OVERLAY FOR FILE METADATA & ARGS
            // ==========================================
            Popup {
                id: descriptionPopup
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                width: Math.min(parent.width * 0.8, 480)
                height: contentLayout.implicitHeight + 40
                modal: true
                focus: true
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

                background: Rectangle {
                    color: window.colorCard
                    radius: window.cardRadius
                    border.color: window.colorAccent
                    border.width: 1.5
                }

                property string popupTitle: ""
                property string popupDesc: ""

                function showPopup(tName, tDesc) {
                    popupTitle = tName;
                    popupDesc = tDesc;
                    descriptionPopup.open();
                }

                ColumnLayout {
                    id: contentLayout
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 12

                    RowLayout {
                        Layout.fillWidth: true

                        Text {
                            text: descriptionPopup.popupTitle
                            font.pixelSize: 18
                            font.bold: true
                            color: window.colorText
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }

                        // Close "X" Button
                        Rectangle {
                            width: 24
                            height: 24
                            radius: 12
                            color: closeMouse.containsMouse ? Qt.rgba(255, 0, 0, 0.2) : "transparent"

                            Text {
                                anchors.centerIn: parent
                                text: "×"
                                color: "white"
                                font.pixelSize: 18
                            }

                            MouseArea {
                                id: closeMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: descriptionPopup.close()
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: window.colorAccent
                        opacity: 0.3
                    }

                    Text {
                        text: descriptionPopup.popupDesc
                        font.pixelSize: 13
                        lineHeight: 1.2
                        color: Qt.rgba(window.colorText.r, window.colorText.g, window.colorText.b, 0.8)
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 10
                        color: "transparent"
                    }

                    Button {
                        text: "Close"
                        Layout.alignment: Qt.AlignRight

                        contentItem: Text {
                            text: "Close Details"
                            font.pixelSize: 12
                            font.bold: true
                            color: window.colorBg
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 32
                            color: parent.hovered ? Qt.lighter(window.colorAccent, 1.1) : window.colorAccent
                            radius: 4
                        }

                        onClicked: descriptionPopup.close()
                    }
                }
            }
        }
    }

    // ==========================================
    // 2. DETAILED GAME INFO PAGE COMPONENT (Active Page)
    // ==========================================
    Component {
        id: gameDetailComponent

        Page {
            id: detailPage

            // Expected properties passed during pushes
            property string gameTitle: ""
            property string gameGenre: ""
            property string gameRating: ""
            property string gameCover: ""
            property string gameScreenshot: ""
            property string gameRelease: ""
            property string gameDev: ""
            property string gameDesc: ""

            background: Rectangle {
                color: window.colorBg
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                // Banner Screenshot Section with dynamic cover
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 240
                    color: "black"
                    clip: true

                    Image {
                        anchors.fill: parent
                        source: detailPage.gameScreenshot
                        fillMode: Image.PreserveAspectCrop
                        opacity: 0.7
                    }

                    // Shadow Gradient overlay
                    Rectangle {
                        anchors.fill: parent
                        gradient: Gradient {
                            GradientStop {
                                position: 0.0
                                color: "transparent"
                            }
                            GradientStop {
                                position: 1.0
                                color: window.colorBg
                            }
                        }
                    }

                    // Go Back Nav Button in detail page top left
                    Rectangle {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.margins: 16
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
                            onClicked: mainStack.pop()
                        }
                    }
                }

                // Detailed Info Meta Area
                RowLayout {
                    Layout.fillWidth: true
                    Layout.margins: 20
                    Layout.topMargin: -40 // pull up content into screenshot overlay area
                    spacing: 20

                    // Mini Floating Cover
                    Rectangle {
                        width: 100
                        height: 140
                        radius: window.cardRadius
                        color: window.colorCard
                        border.color: window.colorAccent
                        border.width: 2
                        clip: true

                        Image {
                            anchors.fill: parent
                            source: detailPage.gameCover
                            fillMode: Image.PreserveAspectCrop
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Text {
                            text: detailPage.gameTitle
                            font.pixelSize: 24
                            font.bold: true
                            color: window.colorText
                        }

                        Text {
                            text: "Genre: " + detailPage.gameGenre + "  |  Rating: " + "⭐ " + detailPage.gameRating
                            font.pixelSize: 12
                            color: window.colorAccent
                        }

                        Text {
                            text: "Developer: " + detailPage.gameDev
                            font.pixelSize: 12
                            color: Qt.rgba(window.colorText.r, window.colorText.g, window.colorText.b, 0.5)
                        }

                        Text {
                            text: "Release Date: " + detailPage.gameRelease
                            font.pixelSize: 12
                            color: Qt.rgba(window.colorText.r, window.colorText.g, window.colorText.b, 0.5)
                        }
                    }
                }

                // Description Box scroll
                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.leftMargin: 20
                    Layout.rightMargin: 20
                    Layout.bottomMargin: 20
                    clip: true

                    ColumnLayout {
                        width: parent.width
                        spacing: 12

                        Text {
                            text: "About the Game"
                            font.pixelSize: 16
                            font.bold: true
                            color: window.colorText
                        }

                        Text {
                            text: detailPage.gameDesc
                            font.pixelSize: 13
                            lineHeight: 1.4
                            color: Qt.rgba(window.colorText.r, window.colorText.g, window.colorText.b, 0.8)
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }
    }

    // ==========================================
    // BACKEND GAMES DATABASE (Standalone QML model)
    // ==========================================
    ListModel {
        id: gamesModel
        ListElement {
            gameId: "g1"
            title: "Cyberbound 2088"
            genre: "Action Sci-Fi RPG"
            rating: "4.8"
            coverImage: "https://images.unsplash.com/photo-1542751371-adc38448a05e?auto=format&fit=crop&w=600&q=80"
            screenshot: "https://images.unsplash.com/photo-1607604276583-eef5d076aa5f?auto=format&fit=crop&w=1200&q=80"
            releaseDate: "2025-11-10"
            developer: "Quantum Shell Labs"
            description: "Navigate a sprawling neon-drenched metropolis controlled by megacorporations. Upgrade your neural implants, hack secure corporate networks, and write your own legend in the digital underworld."
        }
        ListElement {
            gameId: "g2"
            title: "Elysium: Wild Chronicles"
            genre: "Fantasy Open-World"
            rating: "4.9"
            coverImage: "https://images.unsplash.com/photo-1511512578047-dfb367046420?auto=format&fit=crop&w=600&q=80"
            screenshot: "https://images.unsplash.com/photo-1518709268805-4e9042af9f23?auto=format&fit=crop&w=1200&q=80"
            releaseDate: "2024-05-18"
            developer: "Studio Forgebound"
            description: "An open-world adventure focusing on fluid traversal, visual exploration, and ancient magic physics. Uncover the secrets of the floating islands and restore balance to the natural elements."
        }
        ListElement {
            gameId: "g3"
            title: "Vanguard: Stellar Horizon"
            genre: "Space Sim & Strategy"
            rating: "4.7"
            coverImage: "https://images.unsplash.com/photo-1612287230202-1bf1d85d1bdf?auto=format&fit=crop&w=600&q=80"
            screenshot: "https://images.unsplash.com/photo-1451187580459-43490279c0fa?auto=format&fit=crop&w=1200&q=80"
            releaseDate: "2026-02-28"
            developer: "Aetherial Interactive"
            description: "Captain a highly detailed starship through unexplored warp sectors. Manage power-routing grids, negotiate space treaties with rogue factions, and survey mineral-dense cosmic horizons."
        }
        ListElement {
            gameId: "g4"
            title: "Retro Drift '89"
            genre: "Retro Arcade Racer"
            rating: "4.5"
            coverImage: "https://images.unsplash.com/photo-1550745165-9bc0b252726f?auto=format&fit=crop&w=600&q=80"
            screenshot: "https://images.unsplash.com/photo-1515260268569-9271009adfdb?auto=format&fit=crop&w=1200&q=80"
            releaseDate: "2023-08-15"
            developer: "HyperWave Interactive"
            description: "Satisfy your nostalgia with high-speed synthwave racing. Master drift angles on neon rain-slicked mountain passes and unlock customizable retro sports cars with full analog dynamics."
        }
        ListElement {
            gameId: "g5"
            title: "Gridlock Tactics"
            genre: "Tactical Turn-Based"
            rating: "4.6"
            coverImage: "https://images.unsplash.com/photo-1538481199705-c710c4e965fc?auto=format&fit=crop&w=600&q=80"
            screenshot: "https://images.unsplash.com/photo-1611162617213-7d7a39e9b1d7?auto=format&fit=crop&w=1200&q=80"
            releaseDate: "2025-01-20"
            developer: "ByteLayer Games"
            description: "A turn-based strategic tabletop simulator inside a digital holographic board. Outsmart tactical squad AI, positional traps, and structural hazards in simulated industrial networks."
        }
        ListElement {
            gameId: "g6"
            title: "Shadow of the Shogun"
            genre: "Stealth Action / Adventure"
            rating: "4.9"
            coverImage: "https://images.unsplash.com/photo-1553481187-be93c21490a9?auto=format&fit=crop&w=600&q=80"
            screenshot: "https://images.unsplash.com/photo-1522069169874-c58ec4b76be5?auto=format&fit=crop&w=1200&q=80"
            releaseDate: "2024-10-05"
            developer: "Sumi-E Chronicles Studio"
            description: "Embark on an immersive stealth action journey across ancient Feudal villages. Master traditional sword mastery, smoke bombs, grappling hooks, and shadow-climbing techniques to avenge your master."
        }
    }
}
