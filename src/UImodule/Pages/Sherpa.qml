import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Text {
            text: "Sherpa-ncnn Offline STT"
            font.pixelSize: 24
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        // The Real-time Transcript Area
        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent
                TextArea {
                    id: transcriptArea
                    placeholderText: "Transcription will appear here..."
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                    font.pixelSize: 18
                }
            }
        }

        // Connection logic
        // Connections {
        //     target: speechManager
        //     function onPartialTextReady(text) {
        //         transcriptArea.text = text; // Update UI instantly
        //     }
        // }
        Connections {
            target: speechManager

            function onPartialTextReady(text) {
                console.log("QML:", text);
                transcriptArea.text = text;
            }
        }

        Button {
            id: micButton
            text: checked ? "STOP LISTENING" : "START LISTENING"
            checkable: true
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            onCheckedChanged: {
                if (checked) {
                    speechManager.startListening();
                } else {
                    speechManager.stopListening();
                }
            }

            // Visual feedback that the UI isn't hung
            background: Rectangle {
                color: micButton.checked ? "#ff4444" : "#44ff44"
                radius: 10
            }
        }
    }
}
