import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import LittleScribblers.Core

Item {
    id: yardstickRoot
    implicitHeight: 36

    property qreal accuracyPercent: 100.0
    property int difficulty: AppDifficulty.Level.Medium

    Rectangle {
        anchors.fill: parent
        radius: 12
        color: "#F5F5F4"
        border.color: "#E7E5E4"
        border.width: 1

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            spacing: 10

            Label {
                text: "🎯 Accuracy"
                font.bold: true
                font.pixelSize: 11
                color: "#57534E"
            }

            // Meter bar
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 12
                radius: 6
                color: "#E7E5E4"
                clip: true

                Rectangle {
                    width: parent.width * Math.min(1.0, Math.max(0.0, yardstickRoot.accuracyPercent / 100.0))
                    height: parent.height
                    radius: 6
                    color: yardstickRoot.accuracyPercent >= 85 ? "#10B981" : (yardstickRoot.accuracyPercent >= 65 ? "#F59E0B" : "#EF4444")
                }
            }

            Label {
                text: qsTr("%1%").arg(Math.round(yardstickRoot.accuracyPercent))
                font.bold: true
                font.pixelSize: 13
                color: "#1C1917"
            }
        }
    }
}
