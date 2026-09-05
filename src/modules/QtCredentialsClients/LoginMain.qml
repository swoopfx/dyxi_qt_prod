import QtQuick
import QtQuick.Controls
import QtCredentialsClients

Page {
    id: window

    Connections {
        target: authService

        function onLoginSuccess() {
            console.log("Login successful");

            loader.sourceComponent = welcomeComponent;
        }

        function onAuthStateChanged() {
            console.log("Authentication state changed:", authService.isSignedIn);

            if (!authService.isSignedIn) {
                loader.sourceComponent = loginComponent;
            }
        }

        function onErrorOccurred(message) {
            console.error("Authentication error:", message);
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

        Item {
            Component.onCompleted: {
                stackView.replace("qrc:/ui/UIModule/Pages/WelcomePage.qml");
            }
        }
    }

    Component {
        id: editorComponent

        Item {
            anchors.fill: parent
        }
    }

    Component {
        id: debuggerComponent

        Item {
            anchors.fill: parent
        }
    }

    Component.onCompleted: {
        console.log("Checking saved authentication session");

        authService.checkSavedSession();
    }
}
