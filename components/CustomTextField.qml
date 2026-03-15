import QtQuick 2.15
import QtQuick.Controls 2.15

TextField {
    id: control
    // property color color: "#FFFFFF"
    // property int fontSize: 24
    
    font.pixelSize: fontSize
    font.letterSpacing: 1.5
    font.weight: Font.DemiBold
    color: "#2D3436"
    
    placeholderTextColor: "#95A5A6"
    
    background: Rectangle {
        implicitWidth: 350
        implicitHeight: 60
        color: control.color
        radius: 15
        border.width: control.activeFocus ? 4 : 2
        border.color: control.activeFocus ? "#4A90E2" : "#BDC3C7"
        
        // ADHD-friendly: clear visual focus indicator
        Behavior on border.color { ColorAnimation { duration: 200 } }
    }
    
    // Clear padding for readability
    leftPadding: 20
    rightPadding: 20
    topPadding: 10
    bottomPadding: 10
}
