import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// import QtCredentialsClient

ApplicationWindow {
    id: appWindow
    visible: true

    // LoginMain {}
    // Button {
    //     onClicked: {
    //         stackView.push("qrc:/modules/oauth/LoginMainqml");
    //     }
    // }
    contentOrientation: Qt.LandscapeOrientation

    // StackView {
    //     id: stackView
    //     anchors.fill: parent
    //     initialItem: "qrc:/ui/UIModule/Pages/WelcomePage.qml" // Relative path to the file
    // }

    StackView {
        id: stackView
        anchors.fill: parent /* * Default page while PageRouter restores the * authentication session. */
        initialItem: "qrc:/ui/UIModule/Pages/WelcomePage.qml"
        Connections {
            target: pageRouter
            function onCurrentPageChanged() {
                if (!pageRouter.currentPage)
                    return;
                console.log("[PageRouter] Loading:", pageRouter.currentPage); /* * Replace the current root page. * * This prevents users from pressing Back * and returning to WelcomePage after login. */ stackView.replace(pageRouter.currentPage);
            }
        }
    }
    Component.onCompleted: {
        console.log("[main.qml] Starting PageRouter...");
        pageRouter.initialize();
    }

    onClosing: function (closeEvent) {
        if (stackView.depth > 1) {
            // We are deeper in the stack; go back one screen
            stackView.pop();
            closeEvent.accepted = false; // Prevent the app from closing
        } else {
            // We are on the root view; let the app close normally
            closeEvent.accepted = true;
        }
    }
}
