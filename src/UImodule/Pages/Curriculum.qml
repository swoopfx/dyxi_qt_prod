import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/**
 * DYXI Curriculum Overview — Ages 5–12.
 * Connects to a curriculum data model and includes a Caregiver Review mode.
 */
Page {
    id: window
    visible: true
    // width: 1280
    // height: 800
    // minimumWidth: 900
    // minimumHeight: 620
    // title: "DYXI · My Curriculum Journey"
    // color: "#fffaf1"

    property bool isCaregiver: false
    property string launchingId: ""
    property color ink: "#24304b"
    property color violet: "#6b55db"
    property color coral: "#ef806f"
    property color cyan: "#36bad0"
    property color yellow: "#ffd66f"

    ListModel {
        id: curriculumModel
        ListElement {
            eventId: "sound-garden"
            title: "The Sound Garden"
            desc: "Discovering phonics through play."
            done: true
            icon: "🌱"
            score: 92
            route: "/boards/listening"
        }
        ListElement {
            eventId: "number-nest"
            title: "Number Nest"
            desc: "Counting and simple patterns."
            done: true
            icon: "🪺"
            score: 88
            route: "/math/younger"
        }
        ListElement {
            eventId: "word-weaver"
            title: "Word Weaver"
            desc: "Building your first short words."
            done: false
            icon: "🧶"
            score: 0
            route: "/boards/writing"
        }
        ListElement {
            eventId: "shape-shifter"
            title: "Shape Shifter"
            desc: "Exploring geometry and space."
            done: false
            icon: "💎"
            score: 0
            route: "/practical"
        }
        ListElement {
            eventId: "logic-leap"
            title: "Logic Leap"
            desc: "Solving big puzzles together."
            done: false
            icon: "🚀"
            score: 0
            route: "/boards/practice"
        }
    }

    function launch(id, route) {
        if (window.isCaregiver)
            return;
        window.launchingId = id;
        launchTimer.restart();
    }

    Timer {
        id: launchTimer
        interval: 1200
        onTriggered: window.launchingId = ""
    }

    Component {
        id: eventDelegate
        Rectangle {
            width: parent.width
            height: window.isCaregiver ? 104 : 92
            radius: 24
            color: "white"
            border.color: done ? "#e8f7f8" : "#efe5d8"
            border.width: 2
            MouseArea {
                anchors.fill: parent
                enabled: !done && !window.isCaregiver
                onClicked: window.launch(eventId, route)
            }
            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 18
                Rectangle {
                    Layout.preferredWidth: 56
                    Layout.preferredHeight: 56
                    radius: 16
                    color: done ? "#e8f7f8" : "#fff8e6"
                    Text {
                        anchors.centerIn: parent
                        text: icon
                        font.pixelSize: 28
                    }
                }
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2
                    Row {
                        spacing: 8
                        Text {
                            text: title
                            color: ink
                            font.family: "Fraunces"
                            font.pixelSize: 20
                            font.weight: Font.DemiBold
                        }
                        Rectangle {
                            visible: window.isCaregiver && done
                            Layout.preferredHeight: 18
                            width: 48
                            radius: 9
                            color: "#e8f7f8"
                            Text {
                                anchors.centerIn: parent
                                text: score + "%"
                                color: cyan
                                font.pixelSize: 10
                                font.weight: Font.Bold
                            }
                        }
                    }
                    Text {
                        text: desc
                        color: ink
                        opacity: .55
                        font.family: "Nunito Sans"
                        font.pixelSize: 14
                    }
                }
                Column {
                    visible: window.isCaregiver
                    spacing: 2
                    Layout.rightMargin: 10
                    Text {
                        text: "STATUS"
                        color: ink
                        opacity: .3
                        font.pixelSize: 9
                        font.weight: Font.Bold
                    }
                    Text {
                        text: done ? "Verified" : "Pending"
                        color: done ? cyan : coral
                        font.pixelSize: 12
                        font.weight: Font.Bold
                    }
                }
                Rectangle {
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 40
                    radius: 20
                    color: done ? cyan : (window.launchingId === eventId ? violet : "#f5f6f3")
                    Text {
                        anchors.centerIn: parent
                        text: window.launchingId === eventId ? "..." : (done ? "Done" : (window.isCaregiver ? "Details" : "Start"))
                        color: done || window.launchingId === eventId ? "white" : ink
                        opacity: done || window.launchingId === eventId ? 1 : .45
                        font.family: "Nunito Sans"
                        font.pixelSize: 13
                        font.weight: Font.Bold
                    }
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#fffaf1"
        Rectangle {
            width: parent.width * .36
            height: parent.height * .32
            x: parent.width * .72
            y: -parent.height * .08
            radius: width / 2
            color: "#e8f7f8"
        }
    }
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 0
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 58
            spacing: 12
            Image {
                Layout.preferredWidth: 42
                Layout.preferredHeight: 42
                source: "qrc:/assets/dyxi_logo_42.png"
                fillMode: Image.PreserveAspectCrop
                smooth: true
            }
            Column {
                Text {
                    text: "DYXI"
                    color: ink
                    font.family: "Nunito Sans"
                    font.pixelSize: 21
                    font.weight: Font.DemiBold
                }
                Text {
                    text: "MY CURRICULUM"
                    color: violet
                    font.family: "Nunito Sans"
                    font.pixelSize: 10
                    font.weight: Font.Bold
                    font.letterSpacing: 1.8
                }
            }
            Item {
                Layout.fillWidth: true
            }
            Button {
                text: window.isCaregiver ? "Caregiver Mode" : "View as Caregiver"
                onClicked: window.isCaregiver = !window.isCaregiver
                background: Rectangle {
                    radius: 20
                    color: window.isCaregiver ? violet : "white"
                    border.color: "#efe5d8"
                }
                contentItem: Text {
                    text: parent.text
                    color: window.isCaregiver ? "white" : violet
                    font.pixelSize: 12
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 15
                    rightPadding: 15
                }
            }
        }
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: ink
            opacity: .1
            Layout.topMargin: 20
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 130
            Column {
                Text {
                    text: window.isCaregiver ? "Reviewing Leo’s Journey" : "Your learning journey"
                    color: ink
                    font.family: "Fraunces"
                    font.pixelSize: 42
                    font.weight: Font.DemiBold
                }
                Text {
                    text: window.isCaregiver ? "Track progress, scores, and upcoming activities for age 7." : "Let’s see what you’ve discovered and what’s waiting for you."
                    color: ink
                    opacity: .6
                    font.family: "Nunito Sans"
                    font.pixelSize: 17
                }
            }
            Item {
                Layout.fillWidth: true
            }
        }
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 10
            model: curriculumModel
            delegate: eventDelegate
            spacing: 14
            clip: true
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 88
            Button {
                text: "‹  Back"
                background: Rectangle {
                    radius: 18
                    color: "white"
                    border.color: "#efe5d8"
                }
            }
            Item {
                Layout.fillWidth: true
            }
            Button {
                visible: !window.isCaregiver
                text: "Keep Learning  →"
                background: Rectangle {
                    radius: 18
                    color: violet
                }
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    font.family: "Nunito Sans"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
