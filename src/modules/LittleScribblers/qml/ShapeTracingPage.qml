import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import LittleScribblers.Core

Page {
    id: tracingPage

    // Definition of difficulty entered as a parameter by the developer
    property int difficulty: AppDifficulty.Level.Medium
    property string shapeId: "circle"

    onDifficultyChanged: {
        cppTracingEngine.difficulty = tracingPage.difficulty
    }

    Component.onCompleted: {
        cppTracingEngine.difficulty = tracingPage.difficulty
        cppTracingEngine.startTracingSession(shapeId, width, height)
        cppMediaController.speakShapeName(shapeId)
    }

    background: Rectangle {
        color: "#FAF7F2"
    }

    ScrollView {
        id: pageScrollView
        anchors.fill: parent
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        contentWidth: availableWidth

        ColumnLayout {
            width: pageScrollView.availableWidth
            spacing: 12

            // Top Info & Realtime Accuracy Yardstick
            RowLayout {
                Layout.fillWidth: true
                Layout.margins: 16
                spacing: 16

                Label {
                    text: qsTr("Tracing: %1").arg(tracingPage.shapeId.toUpperCase())
                    font.bold: true
                    font.pixelSize: 18
                    color: "#292524"
                }

                Rectangle {
                    color: "#E0F2FE"
                    radius: 8
                    border.color: "#38BDF8"
                    Layout.preferredHeight: 28
                    Layout.preferredWidth: 160
                    Label {
                        anchors.centerIn: parent
                        text: qsTr("Mode: %1").arg(AppDifficulty.getLabel(tracingPage.difficulty))
                        font.pixelSize: 11
                        font.bold: true
                        color: "#0369A1"
                    }
                }

                Item { Layout.fillWidth: true }

                // Accuracy Score & Stars
                Row {
                    spacing: 4
                    Repeater {
                        model: 3
                        Text {
                            text: (index + 1) <= cppTracingEngine.stars ? "⭐" : "☆"
                            font.pixelSize: 20
                        }
                    }
                }
            }

            // Live Yardstick Precision Ruler Component
            YardstickRuler {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Layout.leftMargin: 16
                Layout.rightMargin: 16
                accuracyPercent: cppTracingEngine.accuracy
                difficulty: tracingPage.difficulty
            }

            // Heavy-duty C++ Canvas Tracing Area
            Rectangle {
                id: canvasContainer
                Layout.fillWidth: true
                Layout.preferredHeight: Math.max(420, tracingPage.height - 180)
                Layout.minimumHeight: 360
                Layout.leftMargin: 16
                Layout.rightMargin: 16
                Layout.bottomMargin: 16
                color: "#FFFFFF"
                radius: 16
                border.color: "#E7E5E4"
                border.width: 2
                clip: true

                Canvas {
                    id: traceCanvas
                    anchors.fill: parent

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, width, height);

                        // Draw guide rail based on developer corridor tolerance
                        var corridorWidth = AppDifficulty.getCorridorTolerance(tracingPage.difficulty);
                        ctx.beginPath();
                        ctx.arc(width / 2, height / 2, Math.min(width, height) * 0.35, 0, 2 * Math.PI);
                        ctx.lineWidth = corridorWidth * 2;
                        ctx.strokeStyle = "#FEF3C7"; // Subtle pleasant tint corridor
                        ctx.stroke();

                        // Center guideline
                        ctx.beginPath();
                        ctx.arc(width / 2, height / 2, Math.min(width, height) * 0.35, 0, 2 * Math.PI);
                        ctx.lineWidth = 2;
                        ctx.setLineDash([8, 6]);
                        ctx.strokeStyle = "#F59E0B";
                        ctx.stroke();
                        ctx.setLineDash([]);
                    }
                }

                // Touch Handler forwarding touch data to C++ backend
                MultiPointTouchArea {
                    anchors.fill: parent
                    touchPoints: [
                        TouchPoint {
                            id: touch1
                            onPressedChanged: {
                                if (pressed) {
                                    cppMediaController.playPop()
                                } else {
                                    cppTracingEngine.finalizeStroke()
                                }
                            }
                            onXChanged: {
                                if (pressed) {
                                    var inCorridor = cppTracingEngine.processRawTouchPoint(x, y, Date.now());
                                    if (!inCorridor) {
                                        cppMediaController.playDeviationBuzz()
                                    }
                                }
                            }
                        }
                    ]
                }
            }
        }
    }

    // Celebration & Submit Dialog (Shown when C++ engine detects completion)
    CelebrationDialog {
        id: celebrationDialog
        visible: cppTracingEngine.isCompleted
        accuracy: cppTracingEngine.accuracy
        score: cppTracingEngine.totalScore
        stars: cppTracingEngine.stars
        shapeId: tracingPage.shapeId
        appTypePage: "ShapeTracingPage.qml"
        difficulty: tracingPage.difficulty

        onSubmitClicked: {
            // Gather telemetry in form of a JSON and send to dummy endpoint
            cppTelemetryManager.submitSessionTelemetry(cppTracingEngine, "ShapeTracingPage.qml")
        }
        onRetryClicked: {
            cppTracingEngine.resetSession()
        }
    }
}
