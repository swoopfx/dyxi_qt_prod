import QtQuick
import QtQuick.Controls
import QtCredentialsClient

Page {
    id: window
    // width: 320
    // height: 560
    // visible: true
    // title: qsTr("Enterprise Security Gate")
    // color: "#020617"

    AuthService {
        id: authService

        onLoginSuccess: {
            loader.sourceComponent = welcomeComponent;
        }

        onAuthStateChanged: {
            if (!authService.isSignedIn) {
                loader.sourceComponent = loginComponent;
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
        Login {
            // Signal connections automatically compiled
        }
    }

    Component {
        id: welcomeComponent
        WelcomePage {
            // Renders session profile
        }
    }

    Timer {
        interval: 100
        running: true
        repeat: false

        onTriggered: authService.checkSavedSession()
    }

    // Component.onCompleted: {
    //     authService.checkSavedSession();
    // }
}
