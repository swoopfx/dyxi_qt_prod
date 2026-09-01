
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../js"
import "NavigationTracker.js" as NavigationTracker
import "../Components"

Page {
    id: recentPage
    anchors.fill: parent
    background: Rectangle { color: "#f0f0f0" }

    property var drawer: null // To be set by SelectGameCategoryPage

    header: ApplicationHeader {
        id: recentHeader
        titleText: "Recent Activity"
        leftContent: [
            ToolButton {
                width: 48
                height: 48
                Image {
                    source: "assets/hamburger.png"
                    anchors.centerIn: parent
                    width: 32
                    height: 32
                    fillMode: Image.PreserveAspectFit
                }
                onClicked: {
                    if (recentPage.drawer) recentPage.drawer.open()
                }
            }
        ]
    }

    Flickable {
        anchors.fill: parent
        anchors.topMargin: recentHeader.height
        contentHeight: columnLayout.implicitHeight + 20
        clip: true

        ColumnLayout {
            id: columnLayout
            width: parent.width
            spacing: 10
            padding: 20

            Label {
                text: "Recent Activity"
                font.pixelSize: 24
                font.bold: true
                color: "#333333"
                Layout.fillWidth: true
            }

            Button {
                text: "Export History to JSON"
                Layout.fillWidth: true
                onClicked: {
                    var historyJson = NavigationTracker.exportHistoryToJson();
                    console.log("Exported History:\n" + historyJson);
                    historyText.text = historyJson;
                    NavigationTracker.recordNavigation("RecentPage", "Export History");
                }
            }

            TextArea {
                id: historyText
                Layout.fillWidth: true
                Layout.fillHeight: true
                readOnly: true
                text: JSON.stringify(NavigationTracker.getNavigationHistory(), null, 2)
                font.family: "monospace"
                font.pixelSize: 14
                wrapMode: Text.WordWrap
                background: Rectangle { color: "#eeeeee"; radius: 5 }
            }
        }
    }
}
