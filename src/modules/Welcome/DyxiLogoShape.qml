import QtQuick
import QtQuick.Shapes

Item {
    id: root
    width: 120
    height: 96

    property color leftWingColor1: "#d946ef"
    property color leftWingColor2: "#06b6d4"
    property color rightWingColor1: "#06b6d4"
    property color rightWingColor2: "#7c3aed"

    Shape {
        id: butterflyShape
        anchors.fill: parent
        anchors.margins: 4
        layer.enabled: true

        // Left Antenna feeler
        ShapePath {
            strokeColor: "#a855f7"
            strokeWidth: 4
            capStyle: ShapePath.RoundCap
            fillColor: "transparent"
            startX: 60
            startY: 35
            PathQuad {
                x: 44
                y: 15
                controlX: 52
                controlY: 18
            }
        }

        // Right Antenna feeler
        ShapePath {
            strokeColor: "#7c3aed"
            strokeWidth: 4
            capStyle: ShapePath.RoundCap
            fillColor: "transparent"
            startX: 60
            startY: 35
            PathQuad {
                x: 76
                y: 15
                controlX: 68
                controlY: 18
            }
        }

        // Left Wing - D Shape
        ShapePath {
            strokeWidth: 0
            fillGradient: LinearGradient {
                x1: 10
                y1: 20
                x2: 60
                y2: 80
                GradientStop {
                    position: 0.0
                    color: root.leftWingColor1
                }
                GradientStop {
                    position: 1.0
                    color: root.leftWingColor2
                }
            }
            startX: 58
            startY: 42
            PathCubic {
                x: 20
                y: 18
                control1X: 52
                control1Y: 32
                control2X: 36
                control2Y: 16
            }
            // PathCubic { x: 8; y: 42; control1X: 8; startY: 18; control2X: 6; control2Y: 28 }
            PathCubic {
                x: 28
                y: 82
                control1X: 10
                control1Y: 58
                control2X: 16
                control2Y: 74
            }
            PathCubic {
                x: 58
                y: 70
                control1X: 40
                control1Y: 88
                control2X: 52
                control2Y: 80
            }
            PathLine {
                x: 58
                y: 42
            }
        }

        // Right Wing - X Shape
        ShapePath {
            strokeWidth: 0
            fillGradient: LinearGradient {
                x1: 110
                y1: 20
                x2: 60
                y2: 80
                GradientStop {
                    position: 0.0
                    color: root.rightWingColor1
                }
                GradientStop {
                    position: 1.0
                    color: root.rightWingColor2
                }
            }
            startX: 62
            startY: 42
            PathCubic {
                x: 100
                y: 18
                control1X: 68
                control1Y: 32
                control2X: 84
                control2Y: 16
            }
            PathCubic {
                x: 112
                y: 42
                control1X: 112
                control1Y: 18
                control2X: 114
                control2Y: 28
            }
            PathCubic {
                x: 92
                y: 82
                control1X: 110
                control1Y: 58
                control2X: 104
                control2Y: 74
            }
            PathCubic {
                x: 62
                y: 70
                control1X: 80
                control1Y: 88
                control2X: 68
                control2Y: 80
            }
            PathLine {
                x: 62
                y: 42
            }
        }
    }
}
