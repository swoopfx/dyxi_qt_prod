import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 200
    height: 250

    property alias imageSource: iconImage.source
    property alias labelText: titleLabel.text
    property color accentColor: "#FF6B6B"
    property string description: ""
    property string nextPage: ""
    signal clicked

    // Main Card
    Rectangle {
        id: card
        anchors.fill: parent
        anchors.margins: 10
        radius: 30
        color: "white"
        border.color: root.accentColor
        border.width: 4
        
        // Image Section
        Rectangle {
            id: iconContainer
            width: parent.width - 30
            height: width
            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 20
            color: Qt.lighter(root.accentColor, 1.8)
            clip: true

            Image {
                id: iconImage
                anchors.fill: parent
                anchors.margins: 15
                fillMode: Image.PreserveAspectFit
                antialiasing: true
            }
        }

        // Label Section (Distinct and Unique)
        Text {
            id: titleLabel
            anchors.top: iconContainer.bottom
            anchors.topMargin: 12
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            color: "#333333"
            font.family: "Verdana", "Arial"
            font.pixelSize: 20
            font.bold: true
            // Dyslexia friendly: increased letter spacing
            // letterSpacing: 1.2
        }

        // Interaction Area
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked:  root.clicked()
        }

        // Animations
        states: [
            State {
                name: "hovered"
                when: mouseArea.containsMouse
                PropertyChanges {
                    target: card
                    scale: 1.15
                    z: 10
                    border.width: 6
                }
                PropertyChanges {
                    target: iconImage
                    rotation: 5
                }
            }
        ]

        transitions: [
            Transition {
                from: ""; to: "hovered"
                ParallelAnimation {
                    NumberAnimation { properties: "scale"; duration: 250; easing.type: Easing.OutBack }
                    NumberAnimation { target: iconImage; property: "rotation"; duration: 250; easing.type: Easing.OutQuad }
                }
            },
            Transition {
                from: "hovered"; to: ""
                ParallelAnimation {
                    NumberAnimation { properties: "scale"; duration: 200; easing.type: Easing.OutQuad }
                    NumberAnimation { target: iconImage; property: "rotation"; duration: 200; easing.type: Easing.OutQuad }
                }
            }
        ]

        SequentialAnimation {
            id: clickAnim
            NumberAnimation { target: card; property: "scale"; to: 0.9; duration: 50 }
            NumberAnimation { target: card; property: "scale"; to: 1.2; duration: 150; easing.type: Easing.OutBack }
            NumberAnimation { target: card; property: "scale"; to: 1.15; duration: 100 }
        }
    }
}
