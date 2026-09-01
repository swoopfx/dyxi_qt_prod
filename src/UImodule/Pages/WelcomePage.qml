import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/**
 * DYXI Welcome — Calm Learning Garden
 *
 * First screen shown when the tablet app starts. The layout keeps the
 * companion butterfly in the upper-right, away from the Enter button.
 * Bundle assets/dyxi_logo_42.png and assets/dyxi-companion.png through the
 * project's Qt resource file so the qrc paths resolve on-device.
 */
Page {
    id: window
    visible: true
    // width: 1280
    // height: 800
    // minimumWidth: 820
    // minimumHeight: 600
    // title: "Welcome to DYXI"
    // color: "#fbfaf8"

    property color ink: "#18203a"
    property color muted: "#687088"
    property color violet: "#5137e8"
    property color cyan: "#11b8df"
    property color magenta: "#e035c8"
    property bool entering: false

    Rectangle {
        anchors.fill: parent
        color: "#fbfaf8"

        Rectangle {
            width: parent.width * 0.42
            height: parent.height * 0.7
            x: parent.width * 0.68
            y: -parent.height * 0.14
            radius: width / 2
            color: "#edf1ff"
            opacity: 0.78
        }

        Rectangle {
            width: parent.width * 0.42
            height: parent.height * 0.42
            x: -parent.width * 0.13
            y: parent.height * 0.78
            radius: width / 2
            color: "#f4e7fb"
            opacity: 0.62
        }

        Repeater {
            model: 140
            delegate: Rectangle {
                width: 1.5
                height: 1.5
                radius: 1
                color: "#b8bdd0"
                opacity: 0.2
                x: (index % 20) * (parent.width / 20) + 10
                y: Math.floor(index / 20) * 18 + 18
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 0

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 58
            spacing: 12

            Image {
                Layout.preferredWidth: 44
                Layout.preferredHeight: 44
                source: "qrc:/ui/images/dyxi_logo_42.png"
                fillMode: Image.PreserveAspectCrop
                smooth: true
            }

            Column {
                spacing: 2
                Text {
                    text: "DYXI"
                    color: ink
                    font.family: "Nunito Sans"
                    font.pixelSize: 21
                    font.weight: Font.DemiBold
                    font.letterSpacing: -0.8
                }
                Text {
                    text: "LITTLE SIGNALS"
                    color: "#72798f"
                    font.family: "Nunito Sans"
                    font.pixelSize: 10
                    font.weight: Font.Bold
                    font.letterSpacing: 2.4
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                text: "A QUIET PLACE TO BEGIN"
                color: "#9399aa"
                font.family: "Nunito Sans"
                font.pixelSize: 11
                font.weight: Font.Bold
                font.letterSpacing: 1.3
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: ink
            opacity: 0.1
        }

        Item {
            id: content
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 26
            Layout.bottomMargin: 13

            Column {
                id: welcomeCopy
                width: Math.min(parent.width * 0.55, 650)
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                spacing: 0
                opacity: 0
                y: 18

                Behavior on opacity {
                    NumberAnimation {
                        duration: 480
                        easing.type: Easing.OutCubic
                    }
                }
                Behavior on y {
                    NumberAnimation {
                        duration: 520
                        easing.type: Easing.OutCubic
                    }
                }

                Row {
                    spacing: 12
                    bottomPadding: 15
                    Row {
                        spacing: 6
                        Rectangle {
                            width: 12
                            height: 6
                            radius: 6
                            color: cyan
                        }
                        Rectangle {
                            width: 8
                            height: 8
                            radius: 4
                            color: violet
                            anchors.verticalCenter: parent.verticalCenter
                            opacity: 0.35
                        }
                        Rectangle {
                            width: 8
                            height: 6
                            radius: 4
                            color: magenta
                            anchors.verticalCenter: parent.verticalCenter
                            opacity: 0.35
                        }
                    }
                    Text {
                        text: "A LITTLE HELLO FROM DYXI"
                        color: violet
                        font.family: "Nunito Sans"
                        font.pixelSize: 12
                        font.weight: Font.Bold
                        font.letterSpacing: 2.1
                    }
                }

                Text {
                    width: parent.width
                    text: "Hello,\ncurious <font color='#5137e8'>mind.</font>"
                    textFormat: Text.RichText
                    color: ink
                    font.family: "Fraunces"
                    font.pixelSize: Math.max(64, Math.min(112, window.width * 0.065))
                    font.weight: Font.DemiBold
                    lineHeight: 0.61
                    wrapMode: Text.WordWrap
                }

                Text {
                    width: Math.min(parent.width - 70, 480)
                    topPadding: 22
                    text: "This is your space to explore, notice, and show us how you learn."
                    color: muted
                    font.family: "Nunito Sans"
                    font.pixelSize: 18
                    font.weight: Font.Medium
                    lineHeight: 1.25
                    wrapMode: Text.WordWrap
                }

                Button {
                    id: enterButton
                    width: 238
                    height: 68
                    topPadding: 32
                    text: window.entering ? "Let’s go!   →" : "Enter   →"
                    font.family: "Nunito Sans"
                    font.pixelSize: 19
                    font.weight: Font.Bold
                    contentItem: Text {
                        text: enterButton.text
                        color: "white"
                        font: enterButton.font
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 22
                        color: enterButton.down ? "#442dd4" : violet
                        scale: enterButton.hovered ? 1.03 : 1
                        Behavior on scale {
                            NumberAnimation {
                                duration: 160
                                easing.type: Easing.OutCubic
                            }
                        }
                    }
                    onClicked: {
                        // console.log("Welcome clicked!");
                        // stackView.push("LoginPageNew.qml");
                        stackView.push("qrc:/modules/oauth2/QtCredentialsClients/LoginMain.qml");
                    }

                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: -8
                        radius: 28
                        color: "transparent"
                        border.width: 3
                        border.color: violet
                        opacity: 0.22
                        z: -1
                        SequentialAnimation on scale {
                            loops: Animation.Infinite
                            NumberAnimation {
                                from: 1
                                to: 1.08
                                duration: 1200
                                easing.type: Easing.InOutQuad
                            }
                            NumberAnimation {
                                from: 1.08
                                to: 1
                                duration: 1200
                                easing.type: Easing.InOutQuad
                            }
                        }
                    }
                }
            }

            Item {
                id: companionZone
                width: Math.min(parent.width * 0.43, 540)
                height: Math.min(parent.height * 0.82, 530)
                anchors.right: parent.right
                anchors.top: parent.top

                Rectangle {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: parent.width * 0.9
                    height: parent.height * 0.86
                    radius: width * 0.46
                    color: "#f0eaff"
                    opacity: 0.8
                }

                Image {
                    id: companion
                    source: "qrc:/ui/images/dyxi-companion.png"
                    width: parent.width * 0.7
                    height: width
                    anchors.right: parent.right
                    anchors.top: parent.top
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    opacity: 0
                    scale: 0.94

                    Behavior on opacity {
                        NumberAnimation {
                            duration: 580
                            easing.type: Easing.OutCubic
                        }
                    }
                    Behavior on scale {
                        NumberAnimation {
                            duration: 620
                            easing.type: Easing.OutBack
                        }
                    }

                    SequentialAnimation on y {
                        loops: Animation.Infinite
                        NumberAnimation {
                            from: 4
                            to: -5
                            duration: 2400
                            easing.type: Easing.InOutSine
                        }
                        NumberAnimation {
                            from: -5
                            to: 4
                            duration: 2400
                            easing.type: Easing.InOutSine
                        }
                    }
                }

                Rectangle {
                    width: 72
                    height: 72
                    radius: 24
                    color: "white"
                    opacity: 0.95
                    x: 0
                    y: parent.height * 0.23
                    rotation: -10
                    Text {
                        anchors.centerIn: parent
                        text: "✦"
                        color: magenta
                        font.pixelSize: 30
                    }
                }

                Rectangle {
                    width: 184
                    height: 72
                    radius: 20
                    color: "white"
                    opacity: 0.94
                    x: parent.width * 0.08
                    y: parent.height * 0.8
                    Text {
                        anchors.centerIn: parent
                        text: "Let’s make\na little discovery."
                        color: ink
                        font.family: "Fraunces"
                        font.pixelSize: 19
                        font.weight: Font.DemiBold
                        lineHeight: 0.9
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        welcomeCopy.opacity = 1;
        welcomeCopy.y = 0;
        companion.opacity = 0.94;
        companion.scale = 1;
    }
}
