import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/**
 * DYXI Evaluation Start — Calm Learning Garden
 *
 * Bundle the supplied DYXI logo as assets/dyxi_logo_42.png and the companion
 * artwork as assets/dyxi-companion.png in the Qt resource system. The
 * butterfly is deliberately anchored to the upper-right so it cannot cover
 * the primary Start Evaluation action.
 */

Page {
    id: window
    visible: true
    // width: 1280
    // height: 800
    // minimumWidth: 820
    // minimumHeight: 600
    // title: "DYXI Evaluation"
    // color: "#fbfaf8"

    property color ink: "#18203a"
    property color muted: "#596177"
    property color violet: "#5137e8"
    property color cyan: "#11b8df"
    property color magenta: "#e035c8"
    property bool started: false

    Rectangle {
        anchors.fill: parent
        color: "#fbfaf8"

        Rectangle {
            width: parent.width * 0.35
            height: parent.height * 0.7
            x: parent.width * 0.68
            y: -parent.height * 0.2
            radius: width / 2
            color: "#e8f1ff"
            opacity: 0.72
        }

        Rectangle {
            width: parent.width * 0.4
            height: parent.height * 0.45
            x: -parent.width * 0.12
            y: parent.height * 0.7
            radius: width / 2
            color: "#f4e7fb"
            opacity: 0.6
        }

        // Subtle paper-dot texture.
        Repeater {
            model: 160
            delegate: Rectangle {
                width: 1.5
                height: 1.5
                radius: 1
                color: "#b8bdd0"
                opacity: 0.22
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

            Row {
                spacing: 8
                Text {
                    text: "⌑"
                    color: "#72798f"
                    font.pixelSize: 19
                }
                Text {
                    text: "PRIVATE & SECURE"
                    color: "#72798f"
                    font.family: "Nunito Sans"
                    font.pixelSize: 11
                    font.weight: Font.Bold
                    font.letterSpacing: 1.3
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#18203a"
            opacity: 0.1
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 21
            Layout.bottomMargin: 15

            Column {
                id: copyColumn
                width: Math.min(parent.width * 0.5, 610)
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                spacing: 0

                Row {
                    spacing: 11
                    bottomPadding: 21
                    Row {
                        spacing: 6
                        Rectangle {
                            width: 12
                            height: 8
                            radius: 6
                            color: cyan
                        }
                        Rectangle {
                            width: 8
                            height: 6
                            radius: 4
                            color: "#c5bfdc"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Rectangle {
                            width: 8
                            height: 6
                            radius: 4
                            color: "#c5bfdc"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Text {
                        text: "STEP ONE · A BRIGHT BEGINNING"
                        color: violet
                        font.family: "Nunito Sans"
                        font.pixelSize: 12
                        font.weight: Font.Bold
                        font.letterSpacing: 2.1
                    }
                }

                Text {
                    width: parent.width
                    text: "Let’s see how you <font color='#5137e8'>think</font> ."
                    textFormat: Text.RichText
                    color: ink
                    font.family: "Fraunces"
                    font.pixelSize: Math.max(34, Math.min(106, window.width * 0.055))
                    font.weight: Font.DemiBold
                    lineHeight: 0.64
                    wrapMode: Text.WordWrap
                }

                Text {
                    width: Math.min(parent.width - 70, 460)
                    topPadding: 21
                    text: "A few small activities can help us understand your child’s unique way of learning."
                    color: muted
                    font.family: "Nunito Sans"
                    font.pixelSize: 15
                    font.weight: Font.Medium
                    lineHeight: 1.35
                    wrapMode: Text.WordWrap
                }

                Row {
                    topPadding: 34
                    spacing: 18

                    Button {
                        id: startButton
                        width: 264
                        height: 68
                        text: window.started ? "Let’s begin!   →" : "Start evaluation   →"
                        font.family: "Nunito Sans"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                        contentItem: Text {
                            text: startButton.text
                            color: "white"
                            font: startButton.font
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            radius: 22
                            color: startButton.down ? "#442dd4" : violet
                            scale: startButton.hovered ? 1.02 : 1
                            Behavior on scale {
                                NumberAnimation {
                                    duration: 160
                                    easing.type: Easing.OutCubic
                                }
                            }
                        }
                        onClicked: window.started = true

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
                                    duration: 1150
                                    easing.type: Easing.InOutQuad
                                }
                                NumberAnimation {
                                    from: 1.08
                                    to: 1
                                    duration: 1150
                                    easing.type: Easing.InOutQuad
                                }
                            }
                        }
                    }

                    Text {
                        width: 170
                        anchors.verticalCenter: parent.verticalCenter
                        text: "About 10–15 minutes, with a grown-up nearby."
                        color: "#7a8195"
                        font.family: "Nunito Sans"
                        font.pixelSize: 14
                        font.weight: Font.DemiBold
                        lineHeight: 1.3
                        wrapMode: Text.WordWrap
                    }
                }

                Text {
                    topPadding: 22
                    text: "◉  Sound is optional and can be changed anytime."
                    color: "#72798f"
                    font.family: "Nunito Sans"
                    font.pixelSize: 14
                    font.weight: Font.DemiBold
                }
            }

            // Upper-right visual zone: intentionally independent from the CTA column.
            Item {
                id: visualZone
                width: Math.min(parent.width * 0.44, 560)
                height: Math.min(parent.height * 0.8, 530)
                anchors.right: parent.right
                anchors.top: parent.top

                Rectangle {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: parent.width * 0.9
                    height: parent.height * 0.86
                    radius: width * 0.46
                    color: "#f0eaff"
                    opacity: 0.85
                }

                Rectangle {
                    width: 14
                    height: 14
                    radius: 7
                    color: magenta
                    x: parent.width * 0.78
                    y: 12
                }

                Image {
                    id: companion
                    source: "qrc:/ui/images/dyxi-companion.png"
                    width: parent.width * 0.68
                    height: width
                    fillMode: Image.PreserveAspectFit
                    anchors.right: parent.right
                    anchors.top: parent.top
                    smooth: true
                    opacity: 0.94
                    transform: Rotation {
                        origin.x: companion.width / 2
                        origin.y: companion.height / 2
                        angle: -1
                    }
                    SequentialAnimation on y {
                        loops: Animation.Infinite
                        NumberAnimation {
                            from: 4
                            to: -5
                            duration: 2300
                            easing.type: Easing.InOutSine
                        }
                        NumberAnimation {
                            from: -5
                            to: 4
                            duration: 2300
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
                    y: parent.height * 0.16
                    rotation: -10
                    Text {
                        anchors.centerIn: parent
                        text: "✦"
                        color: magenta
                        font.pixelSize: 30
                    }
                }

                Rectangle {
                    width: 190
                    height: 76
                    radius: 20
                    color: "white"
                    opacity: 0.94
                    x: parent.width * 0.08
                    y: parent.height * 0.78
                    Text {
                        anchors.centerIn: parent
                        text: "You’re ready\nwhen they are."
                        color: ink
                        font.family: "Fraunces"
                        font.pixelSize: 20
                        font.weight: Font.DemiBold
                        lineHeight: 0.9
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#18203a"
            opacity: 0.1
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            Text {
                text: "FOR CURIOUS MINDS, AGES 3–12"
                color: "#9399aa"
                font.family: "Nunito Sans"
                font.pixelSize: 11
                font.weight: Font.Bold
                font.letterSpacing: 1.3
            }
            Item {
                Layout.fillWidth: true
            }
            Text {
                text: "MADE FOR A CALMER FIRST STEP"
                color: "#9399aa"
                font.family: "Nunito Sans"
                font.pixelSize: 11
                font.weight: Font.Bold
                font.letterSpacing: 1.3
            }
        }
    }
}
