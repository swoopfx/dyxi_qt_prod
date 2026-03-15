import QtQuick 2.15
import QtQuick.Controls 2.15
import "../components"

PageContainer {
    id: root
    showBackButton: false
    showLogo: true
    
    Column {
        anchors.centerIn: parent
        spacing: 30
        
        Text {
            text: "Welcome to DYXI!"
            font.pixelSize: 40
            font.bold: true
            color: "#4A90E2"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        
        CustomTextField {
            id: usernameField
            placeholderText: "Enter your username"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        
        CustomTextField {
            id: passwordField
            placeholderText: "Enter your password"
            echoMode: TextInput.Password
            anchors.horizontalCenter: parent.horizontalCenter
        }
        
        CustomButton {
            text: "Login"
            color: "#6BCB77"
            width: 300
            height: 70
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                // In real app: validate login
                pageStack.push("WelcomeBackPage.qml", {username: usernameField.text || "Explorer"})
            }
        }
        
        Text {
            text: "Don't have an account? Sign up here!"
            font.pixelSize: 18
            color: "#4A90E2"
            font.underline: true
            anchors.horizontalCenter: parent.horizontalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: console.log("Sign up clicked")
            }
        }
    }
}
