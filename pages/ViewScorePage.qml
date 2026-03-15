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
        spacing: 30
        
        Text {
            text: "Your High Scores!"
            font.pixelSize: 48
            font.bold: true
            color: "#FF6B6B"
            anchors.horizontalCenter: parent.horizontalCenter
            font.letterSpacing: 2
        }
        
        ListView {
            id: scoreList
            width: parent.width * 0.8
            height: parent.height * 0.6
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
            clip: true
            
            model: ListModel {
                ListElement { game: "Word Matching"; score: "2500"; date: "2024-03-10" }
                ListElement { game: "Letter Catch"; score: "1800"; date: "2024-03-12" }
                ListElement { game: "Color Sorting"; score: "3200"; date: "2024-03-14" }
                ListElement { game: "Phonics Adventure"; score: "2100"; date: "2024-03-15" }
            }
            
            delegate: Rectangle {
                width: scoreList.width
                height: 80
                color: "white"
                radius: 15
                border.width: 2
                border.color: "#BDC3C7"
                
                Row {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 40
                    
                    Text {
                        text: model.game
                        font.pixelSize: 28
                        font.bold: true
                        color: "#4A90E2"
                        width: parent.width * 0.4
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    Text {
                        text: "Score: " + model.score
                        font.pixelSize: 28
                        color: "#6BCB77"
                        width: parent.width * 0.3
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    Text {
                        text: model.date
                        font.pixelSize: 20
                        color: "#95A5A6"
                        width: parent.width * 0.2
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
        
        CustomButton {
            text: "Back to Home"
            color: "#4A90E2"
            width: 250
            height: 60
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: pageStack.pop()
        }
    }
}
