import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Welcome

Page {
    id: window
    // width: 760
    // height: 520
    // visible: true
    // title: qsTr("DYXI Phonics Playground")
    // color: "#faf8f5" // Soft dyslexia-friendly warm background

    AudioSynth {
        id: audioSynth
    }

    property string gameState: "welcome" // welcome, launching, lobby
    property bool isDyslexiaMode: true
    property bool isReducedMotion: false
    property bool isMuted: false

    // active character index state
    property int activeIndex: 0
    property var characters: [
        {
            "id": "ollie",
            "name": "Ollie Owl",
            "avatar": "🦉",
            "role": "WORD DETECTIVE",
            "title": "Syllable Spying",
            "primaryColor": "#059669",
            "accentColor": "#0284c7",
            "description": "Ollie helps your child spot hiding syllable word blocks!",
            "audioNotes": [311.13, 349.23, 392.00]
        },
        {
            "id": "penny",
            "name": "Penny Penguin",
            "avatar": "🐧",
            "role": "SOUND ARCHITECT",
            "title": "Phonics Builder",
            "primaryColor": "#d946ef",
            "accentColor": "#7c3aed",
            "description": "Penny makes physical sound chunks to solve matching puzzles!",
            "audioNotes": [392.00, 440.00, 493.88]
        },
        {
            "id": "barnaby",
            "name": "Barnaby Bear",
            "avatar": "🐻",
            "role": "RHYTHM MAESTRO",
            "title": "Clap-a-Syllable",
            "primaryColor": "#ea580c",
            "accentColor": "#db2777",
            "description": "Barnaby claps out long word bounds in clear physical beats!",
            "audioNotes": [261.63, 293.66, 329.63]
        },
        {
            "id": "mimi",
            "name": "Mimi Monkey",
            "avatar": "🐒",
            "role": "ACCURACY TRAINER",
            "title": "Visual Tracing",
            "primaryColor": "#2563eb",
            "accentColor": "#0d9488",
            "description": "Mimi traces gorgeous colorful guidepaths to help eye focus!",
            "audioNotes": [329.63, 392.00, 523.25]
        }
    ]

    property var currentCharacter: characters[activeIndex]

    // Multi-sensory layout settings for Dyslexia Mode
    property real readingWordSpacing: isDyslexiaMode ? 8.0 : 0.0
    property real readingLetterSpacing: isDyslexiaMode ? 2.5 : 0.0

    // Top Header Bar
    Rectangle {
        id: headerBar
        width: parent.width - 32
        height: 54
        anchors.top: parent.top
        anchors.topMargin: 12
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#ffffff"
        radius: 12
        border.color: "#e2e8f0"
        border.width: 1

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16
            spacing: 12

            // DYXI Brand Logo
            DyxiLogoShape {
                id: logo
                width: 48
                height: 38
                Layout.alignment: Qt.AlignVCenter
            }

            Text {
                text: "DYXI"
                font.pixelSize: 14
                font.bold: true
                color: "#1e293b"
                font.letterSpacing: 2.0
            }

            Rectangle {
                width: 1
                height: 20
                color: "#e2e8f0"
                Layout.leftMargin: 8
            }

            Text {
                text: "Phonics Playground"
                font.pixelSize: 11
                color: "#64748b"
                font.bold: true
            }

            Item {
                Layout.fillWidth: true
            }

            // Accessibility Controls Row
            RowLayout {
                spacing: 8

                // Dyslexia Font Toggle Button
                Button {
                    text: "READING FONT"
                    flat: true
                    background: Rectangle {
                        color: window.isDyslexiaMode ? "#ecfdf5" : "#f8fafc"
                        border.color: window.isDyslexiaMode ? "#10b981" : "#cbd5e1"
                        radius: 8
                    }
                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 10
                        font.bold: true
                        color: window.isDyslexiaMode ? "#065f46" : "#475569"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        window.isDyslexiaMode = !window.isDyslexiaMode;
                        if (!window.isMuted) {
                            audioSynth.playHapticSound("light", window.isDyslexiaMode ? 400 : 200, 25);
                        }
                    }
                }

                // Calm Motion Toggle Button
                Button {
                    text: "CALM MOTION"
                    flat: true
                    background: Rectangle {
                        color: window.isReducedMotion ? "#fffbeb" : "#f8fafc"
                        border.color: window.isReducedMotion ? "#f59e0b" : "#cbd5e1"
                        radius: 8
                    }
                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 10
                        font.bold: true
                        color: window.isReducedMotion ? "#78350f" : "#475569"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        window.isReducedMotion = !window.isReducedMotion;
                        if (!window.isMuted) {
                            audioSynth.playHapticSound("light", window.isReducedMotion ? 400 : 200, 25);
                        }
                    }
                }

                // Sounds Toggle Button
                Button {
                    text: window.isMuted ? "SOUNDS: OFF" : "SOUNDS: ON"
                    flat: true
                    background: Rectangle {
                        color: !window.isMuted ? "#eff6ff" : "#fef2f2"
                        border.color: !window.isMuted ? "#3b82f6" : "#f87171"
                        radius: 8
                    }
                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 10
                        font.bold: true
                        color: !window.isMuted ? "#1e40af" : "#991b1b"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        window.isMuted = !window.isMuted;
                        if (!window.isMuted) {
                            audioSynth.playHapticSound("light", 350, 15);
                        }
                    }
                }
            }
        }
    }

    // Main Interactive Stack
    StackView {
        id: screenStack
        anchors.top: headerBar.bottom
        anchors.bottom: footerBar.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 16
        initialItem: welcomeScreenComponent
    }

    // Welcome Screen Component
    Component {
        id: welcomeScreenComponent
        Item {
            anchors.fill: parent

            ColumnLayout {
                anchors.centerIn: parent
                width: parent.width - 64
                spacing: 16

                // Character Carousel Board
                Rectangle {
                    Layout.fillWidth: true
                    height: 200
                    color: "#ffffff"
                    radius: 16
                    border.color: "#e2e8f0"
                    border.width: 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 20

                        // Mascot Visual Stage Left
                        Rectangle {
                            width: 140
                            height: 140
                            color: "#f8fafc"
                            radius: 12
                            border.color: "#f1f5f9"
                            Layout.alignment: Qt.AlignVCenter

                            // Colored background glow
                            Rectangle {
                                anchors.centerIn: parent
                                width: 80
                                height: 80
                                radius: 40
                                color: window.currentCharacter.primaryColor
                                opacity: 0.15
                            }

                            // Carousel Prev Button
                            Button {
                                anchors.left: parent.left
                                anchors.leftMargin: -12
                                anchors.verticalCenter: parent.verticalCenter
                                width: 32
                                height: 32
                                text: "◀"
                                background: Rectangle {
                                    color: "#ffffff"
                                    border.color: "#cbd5e1"
                                    radius: 16
                                }
                                onClicked: {
                                    window.activeIndex = (window.activeIndex - 1 + window.characters.length) % window.characters.length;
                                    triggerMascotSound();
                                }
                            }

                            // Centered Mascot Avatar
                            Text {
                                text: window.currentCharacter.avatar
                                font.pixelSize: 64
                                anchors.centerIn: parent
                            }

                            // Carousel Next Button
                            Button {
                                anchors.right: parent.right
                                anchors.rightMargin: -12
                                anchors.verticalCenter: parent.verticalCenter
                                width: 32
                                height: 32
                                text: "▶"
                                background: Rectangle {
                                    color: "#ffffff"
                                    border.color: "#cbd5e1"
                                    radius: 16
                                }
                                onClicked: {
                                    window.activeIndex = (window.activeIndex + 1) % window.characters.length;
                                    triggerMascotSound();
                                }
                            }
                        }

                        // Mascot Biography Details Right
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 6

                            Rectangle {
                                width: roleText.width + 12
                                height: 18
                                color: "#f1f5f9"
                                radius: 9
                                border.color: "#e2e8f0"

                                Text {
                                    id: roleText
                                    anchors.centerIn: parent
                                    text: window.currentCharacter.role
                                    font.pixelSize: 8
                                    font.bold: true
                                    font.letterSpacing: 1.0
                                    color: window.currentCharacter.primaryColor
                                }
                            }

                            Text {
                                text: window.currentCharacter.name
                                font.pixelSize: 22
                                font.bold: true
                                color: "#1e293b"
                                font.letterSpacing: window.readingLetterSpacing
                            }

                            Text {
                                // text: """ + window.currentCharacter.title + """
                                font.pixelSize: 11
                                font.italic: true
                                color: "#64748b"
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                height: 1
                                color: "#f1f5f9"
                            }

                            Text {
                                text: window.currentCharacter.description
                                font.pixelSize: 11
                                color: "#475569"
                                Layout.fillWidth: true
                                wrapMode: Text.WordWrap
                                font.wordSpacing: window.readingWordSpacing
                                font.letterSpacing: window.readingLetterSpacing * 0.5
                            }
                        }
                    }
                }

                // Interactive PLAY Trigger
                Button {
                    id: playBtn
                    Layout.alignment: Qt.AlignHCenter
                    width: 140
                    height: 54
                    text: "PLAY"

                    background: Rectangle {
                        color: window.currentCharacter.primaryColor
                        radius: 27
                        border.color: "#ffffff"
                        border.width: 2

                        Rectangle {
                            anchors.fill: parent
                            radius: 27
                            color: "#ffffff"
                            opacity: playBtn.pressed ? 0.15 : (playBtn.hovered ? 0.08 : 0.0)
                        }
                    }

                    contentItem: RowLayout {
                        anchors.centerIn: parent
                        spacing: 8
                        Text {
                            text: "⭐"
                            font.pixelSize: 14
                        }
                        Text {
                            text: "PLAY"
                            font.pixelSize: 13
                            font.bold: true
                            color: "#ffffff"
                            font.letterSpacing: 1.5
                        }
                    }

                    onClicked: {
                        if (!window.isMuted) {
                            audioSynth.playHapticSound("heavy", 120, 45);
                        }
                        window.gameState = "launching";
                        screenStack.push(launchingScreenComponent);
                    }
                }
            }

            function triggerMascotSound() {
                if (!window.isMuted) {
                    audioSynth.playHapticSound("double", 300, 70);
                    audioSynth.playTransition(window.currentCharacter.audioNotes);
                }
            }
        }
    }

    // Launch Warp Tunnel Screen Component
    Component {
        id: launchingScreenComponent
        Item {
            anchors.fill: parent

            Rectangle {
                anchors.fill: parent
                color: "#030712" // Deep space black
                radius: 16

                Canvas {
                    id: warpCanvas
                    anchors.fill: parent
                    property var stars: []

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.fillStyle = "rgba(3, 7, 18, 0.25)";
                        ctx.fillRect(0, 0, width, height);

                        // Draw velocity lines
                        ctx.strokeStyle = window.currentCharacter.primaryColor;
                        ctx.lineWidth = 2.0;

                        for (var i = 0; i < stars.length; ++i) {
                            var s = stars[i];
                            ctx.beginPath();
                            ctx.moveTo(s.px, s.py);
                            ctx.lineTo(s.x, s.y);
                            ctx.stroke();
                        }
                    }

                    Timer {
                        interval: 16
                        running: window.gameState === "launching" && !window.isReducedMotion
                        repeat: true
                        onTriggered: {
                            // Update dynamic particle lines
                            var w = warpCanvas.width;
                            var h = warpCanvas.height;
                            var cx = w / 2;
                            var cy = h / 2;

                            if (warpCanvas.stars.length < 40) {
                                warpCanvas.stars.push({
                                    x: cx + (Math.random() - 0.5) * 10,
                                    y: cy + (Math.random() - 0.5) * 10,
                                    px: cx,
                                    py: cy,
                                    speed: 1.1 + Math.random() * 0.1
                                });
                            }

                            for (var i = 0; i < warpCanvas.stars.length; ++i) {
                                var s = warpCanvas.stars[i];
                                s.px = s.x;
                                s.py = s.y;
                                var dx = s.x - cx;
                                var dy = s.y - cy;
                                s.x += dx * 0.12 * s.speed;
                                s.y += dy * 0.12 * s.speed;

                                // Reset if out of bounds
                                if (s.x < 0 || s.x > w || s.y < 0 || s.y > h) {
                                    s.x = cx + (Math.random() - 0.5) * 20;
                                    s.y = cy + (Math.random() - 0.5) * 20;
                                    s.px = cx;
                                    s.py = cy;
                                }
                            }
                            warpCanvas.requestPaint();
                        }
                    }
                }

                // Centered Launch HUD
                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 12

                    Text {
                        text: window.currentCharacter.avatar
                        font.pixelSize: 54
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        text: "LAUNCHING PLAYGROUND..."
                        font.pixelSize: 13
                        font.bold: true
                        color: "#ffffff"
                        font.letterSpacing: 2.0
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        text: "Preparing " + window.currentCharacter.name + "'s customized visual deck"
                        font.pixelSize: 10
                        color: "#9ca3af"
                        Layout.alignment: Qt.AlignHCenter
                    }
                }

                // Auto Transition timer
                Timer {
                    interval: 2200
                    running: window.gameState === "launching"
                    onTriggered: {
                        window.gameState = "lobby";
                        if (!window.isMuted) {
                            audioSynth.playMatchFound();
                        }
                        screenStack.push(lobbyScreenComponent);
                    }
                }
            }
        }
    }

    // Matchmaking Game Lobby Screen Component
    Component {
        id: lobbyScreenComponent
        Item {
            anchors.fill: parent

            Rectangle {
                anchors.fill: parent
                color: "#0f172a" // Deep Slate lobby background
                radius: 16

                ColumnLayout {
                    anchors.centerIn: parent
                    width: parent.width - 64
                    spacing: 20

                    // Radar Indicator
                    Rectangle {
                        width: 100
                        height: 100
                        radius: 50
                        color: "transparent"
                        border.color: window.currentCharacter.primaryColor
                        border.width: 1
                        Layout.alignment: Qt.AlignHCenter

                        Rectangle {
                            anchors.centerIn: parent
                            width: 80
                            height: 80
                            radius: 40
                            color: "transparent"
                            border.color: window.currentCharacter.primaryColor
                            border.width: 2
                            opacity: 0.5
                        }

                        // Rotating scanning line
                        Rectangle {
                            id: scanLine
                            width: 1
                            height: 50
                            color: window.currentCharacter.primaryColor
                            anchors.bottom: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            transformOrigin: Item.Bottom

                            RotationAnimation on rotation {
                                from: 0
                                to: 360
                                duration: 2500
                                loops: Animation.Infinite
                                running: window.gameState === "lobby" && !window.isReducedMotion
                            }
                        }

                        Text {
                            text: window.currentCharacter.avatar
                            font.pixelSize: 32
                            anchors.centerIn: parent
                        }
                    }

                    Text {
                        text: "MATCHMAKING IN PROGRESS"
                        font.pixelSize: 13
                        font.bold: true
                        color: "#ffffff"
                        font.letterSpacing: 1.5
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        text: "Connecting with other child spelling readers..."
                        font.pixelSize: 10
                        color: "#94a3b8"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    // Cancel / Back Button
                    Button {
                        text: "CANCEL"
                        Layout.alignment: Qt.AlignHCenter
                        width: 120
                        height: 38

                        background: Rectangle {
                            color: "transparent"
                            border.color: "#ef4444"
                            border.width: 1
                            radius: 19
                        }

                        contentItem: Text {
                            text: parent.text
                            font.pixelSize: 10
                            font.bold: true
                            color: "#ef4444"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            window.gameState = "welcome";
                            if (!window.isMuted) {
                                audioSynth.playHapticSound("light", 220, 20);
                            }
                            screenStack.pop();
                        }
                    }
                }
            }
        }
    }

    // Footer Bar Branding
    Rectangle {
        id: footerBar
        width: parent.width - 32
        height: 24
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 12
        anchors.horizontalCenter: parent.horizontalCenter
        color: "transparent"

        Text {
            anchors.centerIn: parent
            text: "DYXI Phonics • Built with Qt 6.10 C++ & QML Quick Controls"
            font.pixelSize: 8
            font.bold: true
            color: "#94a3b8"
            font.letterSpacing: 1.0
        }
    }
}
