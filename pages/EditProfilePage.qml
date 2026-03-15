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
            text: "Edit Your Profile"
            font.pixelSize: 48
            font.bold: true
            color: "#4A90E2"
            anchors.horizontalCenter: parent.horizontalCenter
            font.letterSpacing: 2
        }
        
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 50
            
            Column {
                spacing: 20
                anchors.verticalCenter: parent.verticalCenter
                
                Rectangle {
                    width: 200
                    height: 200
                    radius: 100
                    color: "white"
                    border.width: 4
                    border.color: "#4A90E2"
                    clip: true
                    
                    Text {
                        text: "👦"
                        font.pixelSize: 120
                        anchors.centerIn: parent
                    }
                }
                
                CustomButton {
                    text: "Change Avatar"
                    color: "#4A90E2"
                    width: 200
                    height: 50
                    fontSize: 20
                    onClicked: console.log("Change avatar clicked")
                }
            }
            
            Column {
                spacing: 30
                anchors.verticalCenter: parent.verticalCenter
                
                CustomTextField {
                    id: nameField
                    placeholderText: "Your Name"
                    text: "Alex Explorer"
                    width: 400
                }
                
                CustomTextField {
                    id: ageField
                    placeholderText: "Your Age"
                    text: "7"
                    width: 400
                    validator: IntValidator { bottom: 4; top: 10 }
                }
                
                ComboBox {
                    id: gradeCombo
                    model: ["Preschool", "Grade 1", "Grade 2", "Grade 3", "Grade 4", "Grade 5"]
                    currentIndex: 2
                    width: 400
                    font.pixelSize: 24
                }
            }
        }
        
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 30
            
            CustomButton {
                text: "Save Changes"
                color: "#6BCB77"
                width: 300
                height: 80
                fontSize: 32
                onClicked: pageStack.pop()
            }
            
            CustomButton {
                text: "Cancel"
                color: "#FF6B6B"
                width: 300
                height: 80
                fontSize: 32
                onClicked: pageStack.pop()
            }
        }
    }
}
