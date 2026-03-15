import QtQuick 2.15
import QtQuick.Controls 2.15
import "../components"

PageContainer {
    id: root
    showBackButton: true
    showLogo: true
    
    property string username: "Explorer"
    
    Column {
        anchors.centerIn: parent
        spacing: 40
        
        Text {
            text: "Welcome back, " + root.username + "!"
            font.pixelSize: 48
            font.bold: true
            color: "#FF6B6B"
            anchors.horizontalCenter: parent.horizontalCenter
            font.letterSpacing: 2
        }
        
        Text {
            text: "Ready for another adventure today?"
            font.pixelSize: 28
            color: "#2D3436"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        
        Row {
            spacing: 40
            anchors.horizontalCenter: parent.horizontalCenter
            
            CustomButton {
                text: "Start Game"
                color: "#6BCB77"
                width: 250
                height: 80
                fontSize: 32
                onClicked: pageStack.push("StartGamePage.qml")
            }
            
            CustomButton {
                text: "View Profile"
                color: "#4A90E2"
                width: 250
                height: 80
                fontSize: 32
                onClicked: pageStack.push("ViewProfilePage.qml")
            }
        }
        
        CustomButton {
            text: "Settings"
            color: "#FFD93D"
            width: 200
            height: 60
            fontSize: 24
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: pageStack.push("SettingsPage.qml")
        }
    }
}
