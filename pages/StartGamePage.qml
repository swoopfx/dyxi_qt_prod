import QtQuick 2.15
import QtQuick.Controls 2.15
import "../components"

PageContainer {
    id: root
    showBackButton: true
    showLogo: true
    
    Column {
        anchors.centerIn: parent
        spacing: 50
        
        Text {
            text: "Ready to Play?"
            font.pixelSize: 64
            font.bold: true
            color: "#FF6B6B"
            anchors.horizontalCenter: parent.horizontalCenter
            font.letterSpacing: 4
        }
        
        Text {
            text: "Let's start your learning journey!"
            font.pixelSize: 32
            color: "#2D3436"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        
        Row {
            spacing: 40
            anchors.horizontalCenter: parent.horizontalCenter
            
            CustomButton {
                text: "Select Game"
                color: "#6BCB77"
                width: 350
                height: 120
                fontSize: 48
                onClicked: pageStack.push("SelectGamePage.qml")
            }
            
            CustomButton {
                text: "View Scores"
                color: "#4A90E2"
                width: 350
                height: 120
                fontSize: 48
                onClicked: pageStack.push("ViewScorePage.qml")
            }
        }
    }
}
