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
            text: "Your Profile"
            font.pixelSize: 48
            font.bold: true
            color: "#4A90E2"
            anchors.horizontalCenter: parent.horizontalCenter
            font.letterSpacing: 2
        }
        
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 50
            
            Rectangle {
                width: 250
                height: 250
                radius: 125
                color: "#F0F9FF"
                border.width: 8
                border.color: "#4A90E2"
                clip: true
                
                Text {
                    text: "👦"
                    font.pixelSize: 150
                    anchors.centerIn: parent
                }
            }
            
            Column {
                spacing: 20
                anchors.verticalCenter: parent.verticalCenter
                
                Text {
                    text: "Name: Alex Explorer"
                    font.pixelSize: 32
                    font.bold: true
                    color: "#2D3436"
                }
                
                Text {
                    text: "Age: 7 years old"
                    font.pixelSize: 28
                    color: "#2D3436"
                }
                
                Text {
                    text: "Level: 5 (Star Learner)"
                    font.pixelSize: 28
                    color: "#FF6B6B"
                    font.bold: true
                }
                
                Text {
                    text: "Total Points: 12,500"
                    font.pixelSize: 28
                    color: "#6BCB77"
                    font.bold: true
                }
            }
        }
        
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 30
            
            CustomButton {
                text: "Edit Profile"
                color: "#4A90E2"
                width: 250
                height: 70
                fontSize: 32
                onClicked: pageStack.push("EditProfilePage.qml")
            }
            
            CustomButton {
                text: "Back to Home"
                color: "#BDC3C7"
                width: 250
                height: 70
                fontSize: 32
                onClicked: pageStack.pop()
            }
        }
    }
}
