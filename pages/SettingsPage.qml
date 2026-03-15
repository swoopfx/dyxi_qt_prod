import QtQuick 2.15
import QtQuick.Controls 2.15
import "../components"

PageContainer {
    id: root
    showBackButton: true
    showLogo: true
    
    Column {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 40
        
        Text {
            text: "Settings"
            font.pixelSize: 48
            font.bold: true
            color: "#4A90E2"
            anchors.horizontalCenter: parent.horizontalCenter
            font.letterSpacing: 2
        }
        
        Column {
            width: parent.width * 0.8
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 30
            
            // Sound Toggle
            Row {
                width: parent.width
                spacing: 40
                Text {
                    text: "Music & Sound"
                    font.pixelSize: 32
                    color: "#2D3436"
                    width: parent.width * 0.6
                }
                Switch {
                    id: soundSwitch
                    checked: true
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            
            // Dyslexia Font Toggle
            Row {
                width: parent.width
                spacing: 40
                Text {
                    text: "Dyslexia-Friendly Font"
                    font.pixelSize: 32
                    color: "#2D3436"
                    width: parent.width * 0.6
                }
                Switch {
                    id: fontSwitch
                    checked: true
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            
            // Volume Slider
            Row {
                width: parent.width
                spacing: 40
                Text {
                    text: "Volume Level"
                    font.pixelSize: 32
                    color: "#2D3436"
                    width: parent.width * 0.6
                }
                Slider {
                    id: volumeSlider
                    from: 0; to: 100; value: 75
                    width: parent.width * 0.3
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            
            // App Language
            Row {
                width: parent.width
                spacing: 40
                Text {
                    text: "App Language"
                    font.pixelSize: 32
                    color: "#2D3436"
                    width: parent.width * 0.6
                }
                ComboBox {
                    id: langCombo
                    model: ["English", "Spanish", "French"]
                    width: parent.width * 0.3
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
        
        CustomButton {
            text: "Save Settings"
            color: "#6BCB77"
            width: 250
            height: 60
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: pageStack.pop()
        }
    }
}
