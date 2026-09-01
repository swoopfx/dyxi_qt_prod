import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Enterprise.Security

// Location: qml/main.qml
ApplicationWindow {
    id: rootWindow
    visible: true
    width: 1080
    height: 720
    title: "HeroGate Secure Client - Qt 6.11"
    color: "#f8fafc"

    AuthService {
        id: authService

        onLoginSuccess: {
            console.log("Login success signal received in QML.")
            loader.sourceComponent = welcomeComponent
        }

        onAuthStateChanged: {
            if (!authService.isSignedIn) {
                loader.sourceComponent = loginComponent
            }
        }
    }

    // Top Header Bar
    header: ToolBar {
        background: Rectangle {
            color: "#ffffff"
            border.color: "#e2e8f0"
            border.width: 1
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 8

            Rectangle {
                width: 8
                height: 8
                radius: 4
                color: authService.isSignedIn ? "#10b981" : "#0284c7"
            }

            Label {
                text: "HEROGATE SECURE CLIENT"
                color: "#0f172a"
                font.pixelSize: 12
                font.bold: true
                font.family: "Monospace"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "OS KEYCHAIN ACTIVE"
                color: "#0284c7"
                font.pixelSize: 10
                font.bold: true
                font.family: "Monospace"
            }
        }
    }

    Loader {
        id: loader
        anchors.fill: parent
        sourceComponent: loginComponent
    }

    Component {
        id: loginComponent
        Login {}
    }

    Component {
        id: welcomeComponent
        WelcomePage {}
    }

    Component.onCompleted: {
        // Query Keychain on startup to restore session if available
        authService.checkSavedSession()
    }
}
