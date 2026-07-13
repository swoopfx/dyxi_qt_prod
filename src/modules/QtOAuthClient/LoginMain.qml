import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtOAuthClient

Page {
    id: window
    width: 960
    height: 640
    visible: true
    title: qsTr("Enterprise Identity Authorization System v6.0")

    // Bind custom system palettes
    background: Rectangle {
        color: "#030712" // Deep Midnight
    }

    // Instanciate our overhaul backend controller
    AuthService {
        id: auth

        onStateChanged: {
            console.log("System transition detected: " + statusText);
        }

        onAuthErrorOccurred: {
            errorPopup.errorText = errorMessage;
            errorPopup.open();
        }

        onProfileUpdated: {
            // Signal received from C++ database, state changes propagate dynamically to active sub-views
        }
    }

    // Auto-scan for persistent keyring credentials on boot
    Component.onCompleted: {
        auth.checkSavedSession();
    }

    // App Body Layout
    StackLayout {
        anchors.fill: parent
        currentIndex: (auth.statusText.indexOf("SignedIn") !== -1) ? 1 : 0

        // STATE 0: Guest / Login Screen extracted into Login.qml
        Login {
            id: loginView
            anchors.fill: parent
            authService: auth
        }

        // STATE 1: Welcome redirection landing page is instantiated dynamically
        WelcomePage {
            id: welcomeView
            anchors.fill: parent
            authService: auth
        }
    }

    Popup {
        id: errorPopup
        width: 320
        height: 180
        modal: true
        focus: true
        anchors.centerIn: parent

        property string errorText: ""

        background: Rectangle {
            color: "#1e1b4b"
            border.color: "#f43f5e"
            border.width: 2
            radius: 16
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 12

            Text {
                text: "❌ Authorization Failed"
                color: "#f43f5e"
                font.bold: true
                font.pixelSize: 16
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: errorPopup.errorText
                color: "#cbd5e1"
                font.pixelSize: 11
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Button {
                text: "Acknowledge"
                Layout.alignment: Qt.AlignHCenter
                onClicked: errorPopup.close()
            }
        }
    }
}
