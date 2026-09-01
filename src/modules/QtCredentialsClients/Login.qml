import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCredentialsClients

Item {
    id: loginRoot
    anchors.fill: parent

    signal loginRequested(string username, string password)

    property bool isRegisterMode: false
    property bool showPassword: false
    property bool showTechDetails: false

    // Structured Error State
    property string errorCode: ""
    property string errorTitle: ""
    property string errorCategory: ""
    property string errorMessage: ""
    property string suggestedAction: ""
    property string technicalDetails: ""
    property bool hasError: errorMessage !== ""

    function clearError() {
        errorCode = "";
        errorTitle = "";
        errorCategory = "";
        errorMessage = "";
        suggestedAction = "";
        technicalDetails = "";
        showTechDetails = false;
    }

    function setStructuredError(code, cat, title, msg, action, tech) {
        errorCode = code || "ERR_AUTH_GENERIC";
        errorCategory = cat || "auth";
        errorTitle = title || "Authentication Notice";
        errorMessage = msg || "An error occurred during authentication.";
        suggestedAction = action || "Please check your credentials or network and try again.";
        technicalDetails = tech || "";
        showTechDetails = false;
    }

    Connections {
        target: authService
        function onErrorOccurred(errorMsg) {
            // Parses structured error strings or formats gracefully
            if (errorMsg.indexOf("ERR_") !== -1) {
                // Parse format "CODE|CATEGORY|TITLE|MESSAGE|ACTION|TECH" if supplied
                var parts = errorMsg.split("|");
                if (parts.length >= 4) {
                    loginRoot.setStructuredError(parts[0], parts[1], parts[2], parts[3], parts[4] || "", parts[5] || "");
                    return;
                }
            }
            // Smart classification based on text heuristics
            var low = errorMsg.toLowerCase();
            if (low.indexOf("network") !== -1 || low.indexOf("timeout") !== -1 || low.indexOf("connection") !== -1) {
                loginRoot.setStructuredError("ERR_NETWORK_ERROR", "network", "Network Connection Failure", errorMsg, "Verify your network connection or server endpoint URL.", "QNetworkReply::NetworkError / Socket failure");
            } else if (low.indexOf("passcode") !== -1 || low.indexOf("username") !== -1 || low.indexOf("empty") !== -1) {
                loginRoot.setStructuredError("ERR_INPUT_VALIDATION", "validation", "Invalid Input Format", errorMsg, "Please complete all mandatory credential fields correctly.", "Client-side QML inputMethodHints & regex validator");
            } else if (low.indexOf("keychain") !== -1 || low.indexOf("vault") !== -1 || low.indexOf("store") !== -1) {
                loginRoot.setStructuredError("ERR_KEYCHAIN_FAILURE", "keychain", "OS Keyring Storage Issue", errorMsg, "Ensure your OS Credential Manager is unlocked.", "QtKeychain::WritePasswordJob / Access Control List");
            } else {
                loginRoot.setStructuredError("ERR_AUTH_FAILED", "auth", "Verification Failed", errorMsg, "Double-check your credentials and retry.", "REST 401 Unauthorized / Signature Mismatch");
            }
        }
        function onLoginSuccess() {
            loginRoot.clearError();
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#f7f9ff"

        // -------------------------------------------------------------
        // CHILD-FRIENDLY BACKGROUND
        // -------------------------------------------------------------
        // Soft decorative shapes keep the screen playful without
        // distracting from the authentication form.
        Rectangle {
            width: parent.width * 0.28
            height: width
            radius: width / 2
            color: "#e9f5ff"
            opacity: 0.75
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: -width * 0.42
            anchors.bottomMargin: -height * 0.42
        }

        Rectangle {
            width: parent.width * 0.22
            height: width
            radius: width / 2
            color: "#f1eaff"
            opacity: 0.75
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: -width * 0.30
            anchors.bottomMargin: -height * 0.35
        }

        Rectangle {
            width: 18
            height: 18
            radius: 9
            color: "#ffd86b"
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.08
            anchors.verticalCenter: parent.verticalCenter
            opacity: 0.85
        }

        Rectangle {
            width: 12
            height: 12
            radius: 6
            color: "#a8e6cf"
            anchors.right: parent.right
            anchors.rightMargin: parent.width * 0.09
            anchors.verticalCenter: parent.verticalCenter
            opacity: 0.9
        }

        // -------------------------------------------------------------
        // MAIN CARD
        // -------------------------------------------------------------
        Rectangle {
            id: formCard
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: Math.max(42, parent.height * 0.075)
            width: Math.min(parent.width * 0.36, 520)
            height: Math.min(parent.height - anchors.topMargin - 24, 850)
            color: "#ffffff"
            border.color: "#e4e9f5"
            border.width: 1
            radius: 28

            // The content is deliberately inset so the form feels airy.
            ScrollView {
                id: formScroll
                anchors.fill: parent
                anchors.margins: 26
                clip: true
                contentWidth: availableWidth

                ColumnLayout {
                    id: formColumn
                    width: formScroll.availableWidth
                    spacing: 14

                    // -------------------------------------------------
                    // LOGO / WELCOME
                    // -------------------------------------------------
                    Item {
                        Layout.fillWidth: true
                        height: 118

                        Image {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.top
                            width: 108
                            height: 92
                            source: "qrc:/ui/images/logo.png"
                            fillMode: Image.PreserveAspectFit
                            smooth: true
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 3

                        Label {
                            text: loginRoot.isRegisterMode ? "Create your DYXI account" : "Welcome back!"
                            color: "#25324a"
                            font.pixelSize: 21
                            font.bold: true
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignHCenter
                        }

                        Label {
                            text: loginRoot.isRegisterMode ? "Join your learning journey" : "Sign in to continue your learning journey"
                            color: "#7a879f"
                            font.pixelSize: 11
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignHCenter
                            wrapMode: Text.Wrap
                        }
                    }

                    // -------------------------------------------------
                    // SIGN IN / REGISTER TABS
                    // -------------------------------------------------
                    RowLayout {
                        Layout.fillWidth: true
                        height: 48
                        spacing: 0

                        Button {
                            id: signinTab
                            Layout.fillWidth: true
                            height: 48
                            checkable: true
                            checked: !loginRoot.isRegisterMode

                            background: Rectangle {
                                color: signinTab.checked ? "#f1edff" : "#ffffff"
                                border.color: signinTab.checked ? "#7c4dff" : "#e2e7f1"
                                border.width: 1
                                radius: 12
                            }

                            contentItem: Label {
                                text: "Sign In"
                                color: signinTab.checked ? "#6544dc" : "#7b879d"
                                font.bold: true
                                font.pixelSize: 12
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }

                            onClicked: {
                                loginRoot.isRegisterMode = false;
                                loginRoot.clearError();
                            }
                        }

                        Button {
                            id: registerTab
                            Layout.fillWidth: true
                            height: 48
                            checkable: true
                            checked: loginRoot.isRegisterMode

                            background: Rectangle {
                                color: registerTab.checked ? "#f1edff" : "#ffffff"
                                border.color: registerTab.checked ? "#7c4dff" : "#e2e7f1"
                                border.width: 1
                                radius: 12
                            }

                            contentItem: Label {
                                text: "Register"
                                color: registerTab.checked ? "#6544dc" : "#7b879d"
                                font.bold: true
                                font.pixelSize: 12
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }

                            onClicked: {
                                loginRoot.isRegisterMode = true;
                                loginRoot.clearError();
                            }
                        }
                    }

                    // -------------------------------------------------
                    // ERROR / AUTHENTICATION NOTICE
                    // -------------------------------------------------
                    Rectangle {
                        id: errorCard
                        Layout.fillWidth: true
                        visible: loginRoot.hasError
                        color: "#fff2f3"
                        border.color: "#fecdd3"
                        border.width: 1
                        radius: 14
                        implicitHeight: errorLayout.implicitHeight + 18

                        ColumnLayout {
                            id: errorLayout
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 6

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 7

                                Rectangle {
                                    color: "#ffe4e6"
                                    radius: 6
                                    implicitWidth: badgeLabel.implicitWidth + 12
                                    implicitHeight: 21

                                    Label {
                                        id: badgeLabel
                                        anchors.centerIn: parent
                                        text: loginRoot.errorCategory.toUpperCase()
                                        color: "#be123c"
                                        font.pixelSize: 8
                                        font.bold: true
                                    }
                                }

                                Label {
                                    text: loginRoot.errorCode
                                    color: "#be123c"
                                    font.pixelSize: 9
                                    font.bold: true
                                    Layout.fillWidth: true
                                    elide: Text.ElideRight
                                }

                                Button {
                                    implicitWidth: 22
                                    implicitHeight: 22
                                    background: Rectangle {
                                        color: "transparent"
                                    }
                                    contentItem: Label {
                                        text: "✕"
                                        color: "#be123c"
                                        font.pixelSize: 11
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    onClicked: loginRoot.clearError()
                                }
                            }

                            Label {
                                text: loginRoot.errorTitle
                                color: "#334155"
                                font.pixelSize: 11
                                font.bold: true
                                Layout.fillWidth: true
                                wrapMode: Text.Wrap
                            }

                            Label {
                                text: loginRoot.errorMessage
                                color: "#9f1239"
                                font.pixelSize: 10
                                Layout.fillWidth: true
                                wrapMode: Text.Wrap
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                visible: loginRoot.suggestedAction !== ""
                                color: "#fff7ed"
                                radius: 8
                                implicitHeight: actionLayout.implicitHeight + 8

                                RowLayout {
                                    id: actionLayout
                                    anchors.fill: parent
                                    anchors.margins: 7
                                    spacing: 6

                                    Label {
                                        text: "💡"
                                        font.pixelSize: 10
                                    }
                                    Label {
                                        text: loginRoot.suggestedAction
                                        color: "#9a3412"
                                        font.pixelSize: 10
                                        Layout.fillWidth: true
                                        wrapMode: Text.Wrap
                                    }
                                }
                            }

                            Button {
                                Layout.fillWidth: true
                                height: 24
                                visible: loginRoot.technicalDetails !== ""
                                background: Rectangle {
                                    color: "transparent"
                                }
                                contentItem: Label {
                                    text: loginRoot.showTechDetails ? "Hide technical details" : "Show technical details"
                                    color: "#6366f1"
                                    font.pixelSize: 9
                                    horizontalAlignment: Text.AlignLeft
                                }
                                onClicked: loginRoot.showTechDetails = !loginRoot.showTechDetails
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                visible: loginRoot.showTechDetails && loginRoot.technicalDetails !== ""
                                color: "#f5f3ff"
                                border.color: "#ddd6fe"
                                border.width: 1
                                radius: 8
                                implicitHeight: techLabel.implicitHeight + 12

                                Label {
                                    id: techLabel
                                    anchors.fill: parent
                                    anchors.margins: 7
                                    text: loginRoot.technicalDetails
                                    color: "#4338ca"
                                    font.pixelSize: 9
                                    wrapMode: Text.Wrap
                                }
                            }
                        }
                    }

                    // -------------------------------------------------
                    // SOCIAL LOGIN
                    // -------------------------------------------------
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        Rectangle {
                            Layout.fillWidth: true
                            height: 1
                            color: "#e4e8f0"
                        }
                        Label {
                            text: "or continue with"
                            color: "#8b96aa"
                            font.pixelSize: 9
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle {
                            Layout.fillWidth: true
                            height: 1
                            color: "#e4e8f0"
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        Button {
                            id: googleBtn
                            Layout.fillWidth: true
                            height: 44
                            enabled: !authService.isBusy
                            background: Rectangle {
                                color: googleBtn.pressed ? "#f4f7ff" : "#ffffff"
                                border.color: "#dfe5ef"
                                border.width: 1
                                radius: 12
                            }
                            contentItem: RowLayout {
                                anchors.centerIn: parent
                                spacing: 5
                                Image {
                                    source: "qrc:/ui/images/google.svg"
                                    width: 20
                                    height: 20
                                    fillMode: Image.PreserveAspectFit
                                    smooth: true
                                }
                                Label {
                                    text: loginRoot.isRegisterMode ? "Google" : "Google"
                                    color: "#475569"
                                    font.bold: true
                                    font.pixelSize: 10
                                }
                            }
                            onClicked: {
                                loginRoot.clearError();
                                if (loginRoot.isRegisterMode)
                                    authService.registerWithOAuth("google");
                                else
                                    authService.loginWithGoogle();
                            }
                        }

                        Button {
                            id: appleBtn
                            Layout.fillWidth: true
                            height: 44
                            enabled: !authService.isBusy
                            background: Rectangle {
                                color: appleBtn.pressed ? "#f4f7ff" : "#ffffff"
                                border.color: "#dfe5ef"
                                border.width: 1
                                radius: 12
                            }
                            contentItem: RowLayout {
                                anchors.centerIn: parent
                                spacing: 5
                                Image {
                                    source: "qrc:/ui/images/apple.svg"
                                    width: 20
                                    height: 20
                                    fillMode: Image.PreserveAspectFit
                                    smooth: true
                                }
                                Label {
                                    text: "Apple"
                                    color: "#475569"
                                    font.bold: true
                                    font.pixelSize: 10
                                }
                            }
                            onClicked: {
                                loginRoot.clearError();
                                if (loginRoot.isRegisterMode)
                                    authService.registerWithOAuth("apple");
                                else
                                    authService.loginWithOAuth("apple");
                            }
                        }
                    }

                    // -------------------------------------------------
                    // INPUTS
                    // -------------------------------------------------
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 13

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 5

                            Label {
                                text: "Username / Email Address"
                                color: "#526078"
                                font.pixelSize: 11
                                font.bold: true
                            }

                            TextField {
                                id: usernameField
                                Layout.fillWidth: true
                                height: 52
                                placeholderText: "Username or email"
                                selectByMouse: true
                                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                                color: "#334155"
                                font.pixelSize: 12
                                leftPadding: 16
                                rightPadding: 16

                                background: Rectangle {
                                    color: "#fbfcff"
                                    border.color: usernameField.activeFocus ? "#8b5cf6" : "#dfe5ef"
                                    border.width: usernameField.activeFocus ? 2 : 1
                                    radius: 13
                                }

                                onTextChanged: {
                                    if (loginRoot.hasError && loginRoot.errorCategory === "validation") {
                                        loginRoot.clearError();
                                    }
                                }

                                onAccepted: passwordField.forceActiveFocus()
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 5

                            Label {
                                text: "Secret Passcode"
                                color: "#526078"
                                font.pixelSize: 11
                                font.bold: true
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 7

                                TextField {
                                    id: passwordField
                                    Layout.fillWidth: true
                                    height: 52
                                    placeholderText: "Passcode"
                                    echoMode: loginRoot.showPassword ? TextInput.Normal : TextInput.Password
                                    inputMethodHints: Qt.ImhHiddenText | Qt.ImhSensitiveData
                                    selectByMouse: true
                                    color: "#334155"
                                    font.pixelSize: 12
                                    leftPadding: 16
                                    rightPadding: 16

                                    background: Rectangle {
                                        color: "#fbfcff"
                                        border.color: passwordField.activeFocus ? "#8b5cf6" : "#dfe5ef"
                                        border.width: passwordField.activeFocus ? 2 : 1
                                        radius: 13
                                    }

                                    onTextChanged: {
                                        if (loginRoot.hasError && loginRoot.errorCategory === "validation") {
                                            loginRoot.clearError();
                                        }
                                    }

                                    onAccepted: {
                                        if (loginButton.enabled)
                                            loginButton.clicked();
                                    }
                                }

                                Button {
                                    id: togglePassBtn
                                    width: 52
                                    height: 52
                                    background: Rectangle {
                                        color: togglePassBtn.pressed ? "#eee9ff" : "#fbfcff"
                                        border.color: "#dfe5ef"
                                        border.width: 1
                                        radius: 13
                                    }
                                    contentItem: Label {
                                        text: loginRoot.showPassword ? "🙈" : "👁"
                                        color: "#6d5ce7"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 14
                                    }
                                    onClicked: loginRoot.showPassword = !loginRoot.showPassword
                                }
                            }
                        }
                    }

                    Label {
                        text: authService.statusText
                        color: "#4f8cff"
                        font.pixelSize: 10
                        font.italic: true
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillWidth: true
                        visible: authService.isBusy
                    }

                    // -------------------------------------------------
                    // PRIMARY ACTION
                    // -------------------------------------------------
                    Button {
                        id: loginButton
                        Layout.fillWidth: true
                        height: 52
                        enabled: !authService.isBusy

                        background: Rectangle {
                            color: loginButton.enabled ? (loginButton.pressed ? "#6950e8" : "#7c4dff") : "#cbd5e1"
                            radius: 14
                        }

                        contentItem: RowLayout {
                            anchors.centerIn: parent
                            spacing: 8

                            Label {
                                text: authService.isBusy ? "Verifying..." : (loginRoot.isRegisterMode ? "Register & Log In" : "Sign In")
                                color: "#ffffff"
                                font.bold: true
                                font.pixelSize: 13
                            }

                            Rectangle {
                                width: 9
                                height: 9
                                radius: 4.5
                                color: "#bfe8ff"
                                visible: authService.isBusy

                                SequentialAnimation on opacity {
                                    loops: Animation.Infinite
                                    NumberAnimation {
                                        from: 1.0
                                        to: 0.2
                                        duration: 600
                                    }
                                    NumberAnimation {
                                        from: 0.2
                                        to: 1.0
                                        duration: 600
                                    }
                                }
                            }
                        }

                        onClicked: {
                            var u = usernameField.text.trim();
                            var p = passwordField.text.trim();

                            if (u === "") {
                                loginRoot.setStructuredError("ERR_EMPTY_USERNAME", "validation", "Username Required", "Please enter a valid conventional username or email address.", "Type your workspace identifier in the field above.", "Client-side validation check failed (empty username)");
                                usernameField.forceActiveFocus();
                                return;
                            }
                            if (p === "") {
                                loginRoot.setStructuredError("ERR_EMPTY_PASSWORD", "validation", "Passcode Required", "Please provide your secret authorization passcode.", "Type your password in the secret passcode field.", "Client-side validation check failed (empty password)");
                                passwordField.forceActiveFocus();
                                return;
                            }
                            if (p.length < 6) {
                                loginRoot.setStructuredError("ERR_PASSWORD_TOO_SHORT", "validation", "Passcode Too Short", "Passcode must contain at least 6 characters.", "Provide a passcode of 6 or more characters.", "Client-side validation check failed (password.length < 6)");
                                passwordField.forceActiveFocus();
                                return;
                            }

                            loginRoot.clearError();
                            authService.login(u, p);
                        }
                    }

                    // -------------------------------------------------
                    // SECURITY / AUTHENTICATION NOTICE
                    // -------------------------------------------------
                    Rectangle {
                        Layout.fillWidth: true
                        visible: !loginRoot.hasError
                        color: "#f5f1ff"
                        radius: 12
                        implicitHeight: 54

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 9
                            spacing: 8

                            Label {
                                text: "🛡"
                                font.pixelSize: 16
                            }
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 1
                                Label {
                                    text: "Your learning space is protected"
                                    color: "#4c3ca4"
                                    font.bold: true
                                    font.pixelSize: 10
                                }
                                Label {
                                    text: "Secure sign-in for your personalized DYXI experience."
                                    color: "#756a9e"
                                    font.pixelSize: 8
                                    wrapMode: Text.Wrap
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                        height: 8
                    }
                }
            }
        }
    }
}
