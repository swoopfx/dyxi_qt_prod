import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: loginRoot
    anchors.fill: parent

    signal loginRequested(string username, string password)

    property bool isRegisterMode: false

    Rectangle {
        anchors.fill: parent
        color: "#0f172a" // Slate-900 background

        ColumnLayout {
            anchors.centerIn: parent
            width: parent.width * 0.85
            spacing: 16

            // Title Header
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                
                Label {
                    text: loginRoot.isRegisterMode ? "🌟 Register Workspace" : "🛡️ Secure Workspace"
                    color: "#f8fafc"
                    font.pixelSize: 22
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: loginRoot.isRegisterMode ? "Configure a new secure workspace identity" : "Enter your secure master credentials"
                    color: "#94a3b8"
                    font.pixelSize: 12
                    Layout.alignment: Qt.AlignHCenter
                }
            }

            // Segmented Mode Tab Bar
            RowLayout {
                Layout.fillWidth: true
                spacing: 4

                Button {
                    id: signinTab
                    Layout.fillWidth: true
                    height: 28
                    checkable: true
                    checked: !loginRoot.isRegisterMode
                    background: Rectangle {
                        color: signinTab.checked ? "#4f46e5" : "#020617"
                        border.color: "#334155"
                        border.width: 1
                        radius: 6
                    }
                    contentItem: Label {
                        text: "Sign In"
                        color: "#ffffff"
                        font.bold: true
                        font.pixelSize: 11
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        loginRoot.isRegisterMode = false
                    }
                }

                Button {
                    id: registerTab
                    Layout.fillWidth: true
                    height: 28
                    checkable: true
                    checked: loginRoot.isRegisterMode
                    background: Rectangle {
                        color: registerTab.checked ? "#4f46e5" : "#020617"
                        border.color: "#334155"
                        border.width: 1
                        radius: 6
                    }
                    contentItem: Label {
                        text: "Register"
                        color: "#ffffff"
                        font.bold: true
                        font.pixelSize: 11
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        loginRoot.isRegisterMode = true
                    }
                }
            }

            // Input Fields
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 12

                // Username field
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Username / Email Address"
                        color: "#cbd5e1"
                        font.pixelSize: 11
                        font.bold: true
                    }

                    TextField {
                        id: usernameField
                        Layout.fillWidth: true
                        placeholderText: "e.g. explorer@securedomain.internal"
                        selectByMouse: true
                        color: "#f8fafc"
                        background: Rectangle {
                            color: "#020617"
                            border.color: usernameField.activeFocus ? "#4f46e5" : "#334155"
                            border.width: 1
                            radius: 8
                        }
                    }
                }

                // Password field
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Secret Passcode"
                        color: "#cbd5e1"
                        font.pixelSize: 11
                        font.bold: true
                    }

                    TextField {
                        id: passwordField
                        Layout.fillWidth: true
                        placeholderText: "••••••••••••"
                        echoMode: TextInput.Password
                        selectByMouse: true
                        color: "#f8fafc"
                        background: Rectangle {
                            color: "#020617"
                            border.color: passwordField.activeFocus ? "#4f46e5" : "#334155"
                            border.width: 1
                            radius: 8
                        }
                    }
                }
            }

            // Status message
            Label {
                text: authService.statusText
                color: authService.isBusy ? "#38bdf8" : "#ef4444"
                font.pixelSize: 11
                font.italic: true
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
                visible: authService.statusText !== "Ready" && authService.statusText !== ""
            }

            // Submit Button
            Button {
                id: loginButton
                Layout.fillWidth: true
                height: 42
                enabled: !authService.isBusy && usernameField.text !== "" && passwordField.text !== ""

                background: Rectangle {
                    color: loginButton.enabled ? (loginButton.pressed ? "#4338ca" : "#4f46e5") : "#1e293b"
                    radius: 8
                }

                contentItem: Item {
                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 8
                        
                        Label {
                            text: authService.isBusy ? "Verifying..." : (loginRoot.isRegisterMode ? "Register & Log In" : "Launch Safe Workspace")
                            color: loginButton.enabled ? "#ffffff" : "#64748b"
                            font.bold: true
                            font.pixelSize: 13
                        }
                        
                        Rectangle {
                            width: 10
                            height: 10
                            radius: 5
                            color: "#38bdf8"
                            visible: authService.isBusy
                            SequentialAnimation on opacity {
                                loops: Animation.Infinite
                                NumberAnimation { from: 1.0; to: 0.2; duration: 600 }
                                NumberAnimation { from: 0.2; to: 1.0; duration: 600 }
                            }
                        }
                    }
                }

                onClicked: {
                    authService.login(usernameField.text, passwordField.text)
                }
            }

            // Separator and OAuth Buttons
            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                
                Rectangle { Layout.fillWidth: true; height: 1; color: "#1e293b" }
                Label {
                    text: loginRoot.isRegisterMode ? "OR OAUTH REGISTER" : "OR OAUTH SIGN-IN"
                    color: "#64748b"
                    font.pixelSize: 8
                    font.bold: true
                    font.family: "Courier"
                }
                Rectangle { Layout.fillWidth: true; height: 1; color: "#1e293b" }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                Button {
                    id: googleBtn
                    Layout.fillWidth: true
                    height: 36
                    enabled: !authService.isBusy

                    background: Rectangle {
                        color: googleBtn.enabled ? (googleBtn.pressed ? "#0f172a" : "#020617") : "#1e293b"
                        border.color: "#334155"
                        border.width: 1
                        radius: 8
                    }

                    contentItem: RowLayout {
                        anchors.centerIn: parent
                        spacing: 6
                        Label { text: "🌐"; font.pixelSize: 12 }
                        Label {
                            text: loginRoot.isRegisterMode ? "Google Reg" : "Google"
                            color: "#cbd5e1"
                            font.bold: true
                            font.pixelSize: 11
                        }
                    }

                    onClicked: {
                        if (loginRoot.isRegisterMode) {
                            authService.registerWithOAuth("google")
                        } else {
                            authService.loginWithOAuth("google")
                        }
                    }
                }

                Button {
                    id: appleBtn
                    Layout.fillWidth: true
                    height: 36
                    enabled: !authService.isBusy

                    background: Rectangle {
                        color: appleBtn.enabled ? (appleBtn.pressed ? "#0f172a" : "#020617") : "#1e293b"
                        border.color: "#334155"
                        border.width: 1
                        radius: 8
                    }

                    contentItem: RowLayout {
                        anchors.centerIn: parent
                        spacing: 6
                        Label { text: "🍎"; font.pixelSize: 12 }
                        Label {
                            text: loginRoot.isRegisterMode ? "Apple Reg" : "Apple ID"
                            color: "#cbd5e1"
                            font.bold: true
                            font.pixelSize: 11
                        }
                    }

                    onClicked: {
                        if (loginRoot.isRegisterMode) {
                            authService.registerWithOAuth("apple")
                        } else {
                            authService.loginWithOAuth("apple")
                        }
                    }
                }
            }
        }
    }
}
