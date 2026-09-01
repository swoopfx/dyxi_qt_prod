import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window
    width: 320
    height: 560
    visible: true
    title: qsTr("Enterprise Security Gate")
    color: "#020617"

    AuthService {
        id: authService
        
        // Dynamically configured redirect landing page on session check or sign in
        redirectPage: "design" 
        
        onLoginSuccess: {
            if (authService.redirectPage === "welcome") {
                loader.sourceComponent = welcomeComponent
            } else if (authService.redirectPage === "edit") {
                loader.sourceComponent = editorComponent
            } else if (authService.redirectPage === "debug") {
                loader.sourceComponent = debuggerComponent
            } else {
                loader.sourceComponent = welcomeComponent // Default secure fallback
            }
        }
        
        onAuthStateChanged: {
            if (!authService.isSignedIn) {
                loader.sourceComponent = loginComponent
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

    Component {
        id: editorComponent
        Item {
            // Simulated secure code editor target page
        }
    }

    Component {
        id: debuggerComponent
        Item {
            // Simulated memory registers and security enclaves target page
        }
    }

    Component.onCompleted: {
        authService.checkSavedSession()
    }
}
