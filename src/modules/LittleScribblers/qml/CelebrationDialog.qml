import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import LittleScribblers.Core

Dialog {
    id: celebrationDialog
    modal: true
    anchors.centerIn: parent
    width: Math.min(parent.width * 0.9, 540)
    height: Math.min(parent.height * 0.92, 580)
    padding: 16

    property qreal accuracy: 0.0
    property int score: 0
    property int stars: 0
    property string shapeId: ""
    property string appTypePage: "ShapeTracingPage.qml"
    property int difficulty: AppDifficulty.Level.Medium

    signal submitClicked()
    signal retryClicked()

    background: Rectangle {
        color: "#FFFFFF"
        radius: 24
        border.color: "#FDE68A"
        border.width: 3
    }

    ScrollView {
        id: dialogScroll
        anchors.fill: parent
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        contentWidth: availableWidth

        ColumnLayout {
            width: dialogScroll.availableWidth
            spacing: 14

        // Header
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 8
            Repeater {
                model: 3
                Text {
                    text: (index + 1) <= celebrationDialog.stars ? "⭐" : "☆"
                    font.pixelSize: 36
                }
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: celebrationDialog.stars === 3 ? qsTr("🌟 Superstar Tracing! 🌟") : qsTr("🎉 Great Effort! 🎉")
            font.bold: true
            font.pixelSize: 22
            color: "#1C1917"
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Shape: %1 • Developer Difficulty: %2").arg(celebrationDialog.shapeId).arg(AppDifficulty.getLabel(celebrationDialog.difficulty))
            font.pixelSize: 13
            color: "#78716C"
        }

        // Yardstick Score Card
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            color: "#FEF3C7"
            radius: 16
            border.color: "#F59E0B"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 12

                ColumnLayout {
                    Layout.fillWidth: true
                    Label { text: qsTr("Yardstick Accuracy"); font.pixelSize: 11; color: "#92400E" }
                    Label { text: qsTr("%1%").arg(Math.round(celebrationDialog.accuracy)); font.bold: true; font.pixelSize: 20; color: "#78350F" }
                }
                ColumnLayout {
                    Layout.fillWidth: true
                    Label { text: qsTr("Total Points"); font.pixelSize: 11; color: "#92400E" }
                    Label { text: qsTr("%1 pts").arg(celebrationDialog.score); font.bold: true; font.pixelSize: 20; color: "#78350F" }
                }
                ColumnLayout {
                    Layout.fillWidth: true
                    Label { text: qsTr("Status"); font.pixelSize: 11; color: "#92400E" }
                    Label { text: qsTr("Completed"); font.bold: true; font.pixelSize: 16; color: "#065F46" }
                }
            }
        }

        // Telemetry Submission Status Banner (if submitted)
        Rectangle {
            visible: cppTelemetryManager.lastStatusCode > 0
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            color: "#ECFDF5"
            radius: 12
            border.color: "#10B981"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 8
                Text { text: "📡"; font.pixelSize: 16 }
                Label {
                    text: qsTr("Telemetry sent to dummy endpoint (HTTP %1 OK)").arg(cppTelemetryManager.lastStatusCode)
                    font.bold: true
                    font.pixelSize: 12
                    color: "#065F46"
                }
            }
        }

        // PRIMARY ACTION: Submit Button (replacing next shape button)
        Button {
            id: submitBtn
            Layout.fillWidth: true
            Layout.preferredHeight: 52
            enabled: !cppTelemetryManager.isSubmitting

            background: Rectangle {
                color: submitBtn.down ? "#15803D" : "#16A34A"
                radius: 14
            }

            contentItem: RowLayout {
                anchors.centerIn: parent
                spacing: 8
                Text { text: cppTelemetryManager.isSubmitting ? "⏳" : "📤"; font.pixelSize: 18 }
                Label {
                    text: cppTelemetryManager.isSubmitting ? qsTr("Submitting Telemetry JSON...") : qsTr("Submit Tracing & Send Telemetry")
                    font.bold: true
                    font.pixelSize: 15
                    color: "#FFFFFF"
                }
            }

            onClicked: {
                cppMediaController.playFanfare()
                celebrationDialog.submitClicked()
            }
        }

        // Secondary: Retry
        Button {
            Layout.fillWidth: true
            text: qsTr("Trace Again (Reset)")
            onClicked: {
                cppMediaController.playPop()
                celebrationDialog.retryClicked()
            }
        }
    }
}
}
