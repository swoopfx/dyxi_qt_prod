import QtQuick 2.15
import QtQuick.Controls 2.15
import "../components"

PageContainer {
    id: root
    showBackButton: true
    showLogo: true
    
    GridView {
        id: gameGrid
        anchors.fill: parent
        anchors.margins: 40
        cellWidth: 400
        cellHeight: 300
        clip: true
        
        model: ListModel {
            ListElement { name: "Word Matching"; color: "#4A90E2"; icon: "qrc:/assets/images/game_icon.png" }
            ListElement { name: "Letter Catch"; color: "#FF6B6B"; icon: "qrc:/assets/images/edu_icon.png" }
            ListElement { name: "Color Sorting"; color: "#6BCB77"; icon: "qrc:/assets/images/puzzle_icon.png" }
            ListElement { name: "Phonics Adventure"; color: "#FFD93D"; icon: "qrc:/assets/images/game_icon.png" }
            ListElement { name: "Number Fun"; color: "#A29BFE"; icon: "qrc:/assets/images/edu_icon.png" }
            ListElement { name: "Shape Match"; color: "#FAB1A0"; icon: "qrc:/assets/images/puzzle_icon.png" }
        }
        
        delegate: Item {
            width: gameGrid.cellWidth
            height: gameGrid.cellHeight
            
            Rectangle {
                width: 350
                height: 250
                anchors.centerIn: parent
                color: "white"
                radius: 30
                border.width: 4
                border.color: model.color
                
                Column {
                    anchors.centerIn: parent
                    spacing: 20
                    
                    Image {
                        source: model.icon
                        width: 120
                        height: 120
                        fillMode: Image.PreserveAspectFit
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    
                    Text {
                        text: model.name
                        font.pixelSize: 28
                        font.bold: true
                        color: model.color
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Starting " + model.name)
                        // Launch game...
                    }
                }
            }
        }
    }
}
