import QtQuick 2.15
import QtQuick.Controls 2.15
// import QtMultimedia 5.15

Button {
    id: control
    property color color: "#4A90E2"
    property int fontSize: 24
    property bool isIcon: false
    property string iconSource: ""

    // Sound effect for button click
    // SoundEffect {
    //     id: clickSound
    //     source: "qrc:/assets/sounds/click.wav"
    // }

    contentItem: Item {
        Row {
            anchors.centerIn: parent
            spacing: 10
            Image {
                visible: control.isIcon
                source: control.iconSource
                width: 32
                height: 32
                fillMode: Image.PreserveAspectFit
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: control.text
                font.pixelSize: control.fontSize
                font.bold: true
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 60
        color: control.down ? Qt.darker(control.color, 1.1) : (control.hovered ? Qt.lighter(control.color, 1.1) : control.color)
        radius: 30
        border.width: 4
        border.color: Qt.darker(color, 1.2)
        
        // ADHD-friendly visual feedback
        layer.enabled: control.hovered
        layer.effect: ShaderEffect {
            // Placeholder for glow/shadow
        }
    }
    
    // Dyslexia/ADHD-friendly interaction
    scale: control.pressed ? 0.95 : 1.0
    Behavior on scale { NumberAnimation { duration: 100 } }

    // onClicked: {
    //     clickSound.play()
    // }
}
