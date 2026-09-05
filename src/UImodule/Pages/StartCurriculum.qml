import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/**
 * DYXI Start Curriculum — Ages 5–12.
 * A clear entry point to begin the curriculum evaluation.
 */
Page {
    id: window
    visible: true
    // width: 1280
    // height: 800
    // minimumWidth: 900
    // minimumHeight: 620
    // title: "DYXI · Start My Curriculum"
    // color: "#fffaf1"

    property color ink: "#24304b"
    property color violet: "#6b55db"
    property color coral: "#ef806f"

    Rectangle {
        anchors.fill: parent
        color: "#fffaf1"
        Rectangle {
            width: parent.width * .45
            height: parent.height * .4
            x: parent.width * .65
            y: parent.height * .6
            radius: width / 2
            color: "#fef1f0"
        }
    }
    ColumnLayout {
        anchors.centerIn: parent
        width: Math.min(parent.width * .8, 720)
        spacing: 32
        Image {
            Layout.preferredWidth: 88
            Layout.preferredHeight: 88
            anchors.horizontalCenter: parent.horizontalCenter
            source: "qrc:/assets/dyxi_logo_42.png"
            fillMode: Image.PreserveAspectCrop
            smooth: true
        }
        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 12
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Ready to explore?"
                color: ink
                font.family: "Fraunces"
                font.pixelSize: 52
                font.weight: Font.DemiBold
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Your new curriculum is ready for you. Let’s start the evaluation to see what you can do!"
                color: ink
                opacity: .6
                font.family: "Nunito Sans"
                font.pixelSize: 19
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                width: parent.width
            }
        }
        Button {
            id: startBtn
            Layout.preferredWidth: 280
            Layout.preferredHeight: 68
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Start Evaluation"
            background: Rectangle {
                radius: 34
                color: violet
                scale: startBtn.pressed ? 0.96 : 1.0
                Behavior on scale {
                    NumberAnimation {
                        duration: 120
                    }
                }
            }
            contentItem: Text {
                text: parent.text
                color: "white"
                font.family: "Nunito Sans"
                font.pixelSize: 20
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "It takes about 10–15 minutes."
            color: ink
            opacity: .45
            font.family: "Nunito Sans"
            font.pixelSize: 14
            font.weight: Font.DemiBold
        }
    }
    Button {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 40
        text: "‹  Back"
        background: Rectangle {
            radius: 18
            color: "white"
            border.color: "#efe5d8"
        }
    }
}
