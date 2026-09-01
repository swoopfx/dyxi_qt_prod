import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Location: qml/Login.qml
Item {
    id: loginRoot
    anchors.fill: parent

    property bool isRegisterMode: false
    property bool showPassword: false

    Rectangle {
        anchors.fill: parent
        color: "#f8fafc"

        ScrollView {
            anchors.fill: parent
            contentWidth: parent.width
            contentHeight: Math.max(parent.height, formCard.height + 40)
            clip: true

            // Centralized 1/3 layout
            Item {
                width: parent.width
                height: Math.max(parent.height, formCard.height + 40)

                Rectangle {
                    id: formCard
                    anchors.centerIn: parent
                    width: parent.width > 960 ? parent.width * 0.333 : Math.min(parent.width * 0.90, 440)
                    implicitHeight: formColumn.implicitHeight + 40
                    color: "#ffffff"
                    radius: 16
                    border.color: "#e2e8f0"
                    border.width: 1

                    ColumnLayout {
                        id: formColumn
                        anchors.fill: parent
                        anchors.margins: 24
                        spacing: 16

                        // Keychain Status Badge
                        Rectangle {
                            Layout.fillWidth: true
                            height: 48
                            color: "#f0f9ff"
                            radius: 10
                            border.color: "#bae6fd"

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 8

                                Label { text: "🔐"; font.pixelSize: 16 }

                                ColumnLayout {
                                    spacing: 1
                                    Label {
                                        text: "Native OS Keychain Storage"
                                        color: "#0369a1"
                                        font.bold: true
                                        font.pixelSize: 11
                                    }
                                    Label {
                                        text: "QtKeychain + Google OAuth PKCE (No Secrets)"
                                        color: "#0284c7"
                                        font.pixelSize: 9.5
                                        font.family: "Monospace"
                                    }
                                }
                            }
                        }

                        // Title
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 3

                            Label {
                                text: loginRoot.isRegisterMode ? "Create Account" : "Enterprise Login"
                                color: "#0f172a"
                                font.pixelSize: 18
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Label {
                                text: "Sign in with your corporate identity"
                                color: "#64748b"
                                font.pixelSize: 11
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }

                        // OAuth Provider Buttons
                        Button {
                            id: btnGoogle
                            Layout.fillWidth: true
                            height: 40
                            background: Rectangle {
                                color: btnGoogle.down ? "#f8fafc" : "#ffffff"
                                radius: 8
                                border.color: "#cbd5e1"
                            }
                            contentItem: RowLayout {
                                spacing: 8
                                anchors.centerIn: parent
                                Label { text: "🌐"; font.pixelSize: 14 }
                                Label {
                                    text: "Continue with Google"
                                    color: "#1e293b"
                                    font.bold: true
                                    font.pixelSize: 12
                                }
                            }
                            onClicked: authService.loginWithOAuth("google")
                        }

                        Button {
                            id: btnApple
                            Layout.fillWidth: true
                            height: 40
                            background: Rectangle {
                                color: btnApple.down ? "#f8fafc" : "#ffffff"
                                radius: 8
                                border.color: "#cbd5e1"
                            }
                            contentItem: RowLayout {
                                spacing: 8
                                anchors.centerIn: parent
                                Label { text: "🍏"; font.pixelSize: 14 }
                                Label {
                                    text: "Continue with Apple"
                                    color: "#1e293b"
                                    font.bold: true
                                    font.pixelSize: 12
                                }
                            }
                            onClicked: authService.loginWithOAuth("apple")
                        }

                        // Divider
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8
                            Rectangle { Layout.fillWidth: true; height: 1; color: "#e2e8f0" }
                            Label { text: "OR USE EMAIL"; color: "#94a3b8"; font.pixelSize: 9.5; font.bold: true }
                            Rectangle { Layout.fillWidth: true; height: 1; color: "#e2e8f0" }
                        }

                        // Email Field
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Label {
                                text: "CORPORATE EMAIL"
                                color: "#475569"
                                font.pixelSize: 10
                                font.bold: true
                                font.family: "Monospace"
                            }

                            TextField {
                                id: inputEmail
                                Layout.fillWidth: true
                                height: 38
                                placeholderText: "user@enterprise.internal"
                                text: "alex.mercer@enterprise.internal"
                                background: Rectangle {
                                    color: "#f8fafc"
                                    radius: 8
                                    border.color: inputEmail.activeFocus ? "#0284c7" : "#cbd5e1"
                                }
                            }
                        }

                        // Password Field
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Label {
                                text: "PASSWORD"
                                color: "#475569"
                                font.pixelSize: 10
                                font.bold: true
                                font.family: "Monospace"
                            }

                            TextField {
                                id: inputPass
                                Layout.fillWidth: true
                                height: 38
                                echoMode: loginRoot.showPassword ? TextInput.Normal : TextInput.Password
                                placeholderText: "••••••••••••"
                                text: "SecurePass123!"
                                background: Rectangle {
                                    color: "#f8fafc"
                                    radius: 8
                                    border.color: inputPass.activeFocus ? "#0284c7" : "#cbd5e1"
                                }
                            }
                        }

                        // Submit Button
                        Button {
                            id: btnSubmit
                            Layout.fillWidth: true
                            height: 42
                            enabled: !authService.isBusy
                            background: Rectangle {
                                color: btnSubmit.enabled ? "#0284c7" : "#94a3b8"
                                radius: 8
                            }
                            contentItem: Label {
                                text: authService.isBusy ? "Authenticating..." : (loginRoot.isRegisterMode ? "Register Account" : "Sign In")
                                color: "#ffffff"
                                font.bold: true
                                font.pixelSize: 12
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                authService.verifyCredentials(inputEmail.text, inputPass.text)
                            }
                        }
                    }
                }
            }
        }
    }
}
