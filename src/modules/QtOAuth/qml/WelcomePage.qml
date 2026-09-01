import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Location: qml/WelcomePage.qml
Item {
    id: welcomeRoot
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#f8fafc"

        ScrollView {
            anchors.fill: parent
            contentWidth: parent.width
            contentHeight: contentCard.implicitHeight + 60
            clip: true

            Item {
                width: parent.width
                height: contentCard.implicitHeight + 60

                Rectangle {
                    id: contentCard
                    anchors.centerIn: parent
                    width: parent.width > 960 ? parent.width * 0.45 : Math.min(parent.width * 0.90, 520)
                    implicitHeight: cardLayout.implicitHeight + 40
                    color: "#ffffff"
                    radius: 16
                    border.color: "#e2e8f0"
                    border.width: 1

                    ColumnLayout {
                        id: cardLayout
                        anchors.fill: parent
                        anchors.margins: 24
                        spacing: 16

                        // Header
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 12

                            Rectangle {
                                width: 44
                                height: 44
                                radius: 22
                                color: "#e0f2fe"
                                border.color: "#bae6fd"
                                Label {
                                    anchors.centerIn: parent
                                    text: "👤"
                                    font.pixelSize: 20
                                }
                            }

                            ColumnLayout {
                                spacing: 2
                                Label {
                                    text: authService.userName.length > 0 ? authService.userName : "Authenticated User"
                                    color: "#0f172a"
                                    font.bold: true
                                    font.pixelSize: 16
                                }
                                Label {
                                    text: authService.userEmail
                                    color: "#64748b"
                                    font.pixelSize: 12
                                }
                            }
                        }

                        // Keychain Storage Info Card
                        Rectangle {
                            Layout.fillWidth: true
                            implicitHeight: 80
                            color: "#f0fdf4"
                            radius: 10
                            border.color: "#bbf7d0"

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 4

                                Label {
                                    text: "✅ Session Persisted in OS Keychain"
                                    color: "#166534"
                                    font.bold: true
                                    font.pixelSize: 12
                                }
                                Label {
                                    text: "Token secured in native keyring (Windows Credential Manager / macOS Keychain / Linux Secret Service)"
                                    color: "#15803d"
                                    font.pixelSize: 10
                                    wrapMode: Text.WordWrap
                                    Layout.fillWidth: true
                                }
                            }
                        }

                        // Provider Info
                        Rectangle {
                            Layout.fillWidth: true
                            implicitHeight: 50
                            color: "#f8fafc"
                            radius: 8
                            border.color: "#e2e8f0"

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 12

                                Label {
                                    text: "Auth Provider:"
                                    color: "#64748b"
                                    font.pixelSize: 11
                                }
                                Label {
                                    text: authService.authProvider.toUpperCase()
                                    color: "#0f172a"
                                    font.bold: true
                                    font.pixelSize: 11
                                }
                            }
                        }

                        // Sign Out Button
                        Button {
                            id: btnSignOut
                            Layout.fillWidth: true
                            height: 40
                            background: Rectangle {
                                color: btnSignOut.down ? "#fee2e2" : "#ffffff"
                                radius: 8
                                border.color: "#fca5a5"
                            }
                            contentItem: Label {
                                text: "Sign Out & Delete from OS Keychain"
                                color: "#b91c1c"
                                font.bold: true
                                font.pixelSize: 12
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: authService.signOut()
                        }
                    }
                }
            }
        }
    }
}
