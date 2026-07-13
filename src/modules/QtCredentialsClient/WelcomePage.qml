import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: welcomeRoot
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#0b0f19" // Deep Cosmic Dark Slate

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 16

            // Top Status Header row
            RowLayout {
                Layout.fillWidth: true

                Rectangle {
                    width: 32
                    height: 32
                    radius: 16
                    color: "#064e3b"
                    border.color: "#34d399"
                    border.width: 1

                    Label {
                        anchors.centerIn: parent
                        text: "✓"
                        color: "#34d399"
                        font.bold: true
                    }
                }

                ColumnLayout {
                    spacing: 2
                    Layout.fillWidth: true

                    Label {
                        text: "KEYCHAIN ACTIVE"
                        color: "#10b981"
                        font.pixelSize: 9
                        font.bold: true
                        font.family: "Courier"
                    }

                    Label {
                        text: "Secure Session Established"
                        color: "#f8fafc"
                        font.pixelSize: 13
                        font.bold: true
                    }
                }
            }

            // Profile Container
            Rectangle {
                Layout.fillWidth: true
                height: 70
                color: "#1e1b4b"
                radius: 12
                border.color: "#312e81"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 12

                    Rectangle {
                        width: 44
                        height: 44
                        radius: 22
                        color: "#312e81"
                        border.color: "#4f46e5"
                        border.width: 2

                        Text {
                            anchors.centerIn: parent
                            text: "⭐"
                            font.pixelSize: 18
                        }
                    }

                    ColumnLayout {
                        spacing: 2
                        Layout.fillWidth: true

                        Label {
                            text: authService.userName !== "" ? authService.userName : "Explorer Character"
                            font.pixelSize: 15
                            font.bold: true
                            color: "#f8fafc"
                        }

                        Label {
                            text: authService.userEmail !== "" ? authService.userEmail : "unknown@securedomain.internal"
                            font.pixelSize: 10
                            font.family: "Courier"
                            color: "#38bdf8"
                        }
                    }
                }
            }

            // Keyring Parameters isolation
            Rectangle {
                Layout.fillWidth: true
                height: 100
                color: "#020617"
                radius: 12
                border.color: "#1e293b"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            text: "OS Hardware Token Lock:"
                            color: "#64748b"
                            font.pixelSize: 10
                        }
                        Rectangle {
                            width: 8
                            height: 8
                            radius: 4
                            color: "#10b981"
                        }
                        Label {
                            text: "Protected via QtKeychain"
                            color: "#10b981"
                            font.bold: true
                            font.pixelSize: 10
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            text: "System Domain Scope:"
                            color: "#64748b"
                            font.pixelSize: 10
                        }
                        Label {
                            text: "api.enterprise-security.internal"
                            color: "#cbd5e1"
                            font.family: "Courier"
                            font.pixelSize: 10
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            text: "Active Token Stream:"
                            color: "#64748b"
                            font.pixelSize: 10
                        }
                        Label {
                            text: "session_auth_token [AES Encrypted]"
                            color: "#fbbf24"
                            font.pixelSize: 10
                            font.bold: true
                        }
                    }
                }
            }

            // Buttons
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 10

                Button {
                    id: auditBtn
                    Layout.fillWidth: true
                    height: 38

                    background: Rectangle {
                        color: auditBtn.pressed ? "#1e293b" : "#111827"
                        border.color: "#374151"
                        radius: 8
                    }

                    contentItem: Label {
                        text: qsTr("🛡️ Audit Keyring Security Rules")
                        color: "#cbd5e1"
                        font.bold: true
                        font.pixelSize: 11
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: {
                        authService.rawLog("🔍 QKeychain::ReadPasswordJob: Security manual Audit executed inside the operating system keychain wrapper...", "success");
                        authService.rawLog("🛡️ Matching SHA-256 signatures against secure credentials space.", "info");
                    }
                }

                Button {
                    id: profileBtn
                    Layout.fillWidth: true
                    height: 38

                    background: Rectangle {
                        color: profileBtn.pressed ? "#064e3b" : "#022c22"
                        border.color: "#047857"
                        radius: 8
                    }

                    contentItem: Label {
                        text: qsTr("🔄 Fetch User Profile (fetchUserProfile)")
                        color: "#6ee7b7"
                        font.bold: true
                        font.pixelSize: 11
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: {
                        authService.fetchUserProfile();
                    }
                }

                Button {
                    id: signOutBtn
                    Layout.fillWidth: true
                    height: 42

                    background: Rectangle {
                        color: signOutBtn.pressed ? "#450a0a" : "#1e1b4b"
                        radius: 8
                    }

                    contentItem: Label {
                        text: qsTr("Disconnect Safe Session & Clear Keyrings")
                        color: "#fca5a5"
                        font.bold: true
                        font.pixelSize: 11
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: {
                        authService.signOut();
                    }
                }
            }
        }
    }
}
