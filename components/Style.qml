import QtQuick 2.15

pragma Singleton

QtObject {
    // Dyslexia-friendly fonts: OpenDyslexic or similar high-readability sans-serif
    // ADHD-friendly: High contrast, clear spacing, limited clutter
    
    readonly property color bgColor: "#F0F9FF"
    readonly property color primaryColor: "#4A90E2"
    readonly property color secondaryColor: "#FF6B6B"
    readonly property color accentColor: "#FFD93D"
    readonly property color successColor: "#6BCB77"
    readonly property color textColor: "#2D3436"
    
    readonly property int fontSizeLarge: 32
    readonly property int fontSizeMedium: 24
    readonly property int fontSizeSmall: 18
    
    readonly property int spacingLarge: 30
    readonly property int spacingMedium: 20
    readonly property int spacingSmall: 10
    
    readonly property int borderRadius: 20
    
    // For dyslexia: increased letter and line spacing
    readonly property real letterSpacing: 1.5
    readonly property real lineSpacing: 1.2
}
