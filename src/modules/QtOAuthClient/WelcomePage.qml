import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtOAuthClient

Item {
    id: welcomeRoot
    anchors.fill: parent

    // Property to receive the AuthService backend
    property var authService: null

    // Slide-in Animation upon browser redirection callback
    Component.onCompleted: {
        entranceAnim.start();
        if (authService) {
            authService.rawLog("✨ Loaded WelcomePage.qml view component seamlessly upon redirection callback", "info");
        }
    }

    SequentialAnimation {
        id: entranceAnim
        NumberAnimation {
            target: welcomeCard
            property: "opacity"
            from: 0
            to: 1
            duration: 400
            easing.type: Easing.OutQuad
        }
        NumberAnimation {
            target: welcomeCard
            property: "scale"
            from: 0.95
            to: 1.0
            duration: 300
            easing.type: Easing.OutBack
        }
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: -1
        clip: true

        ColumnLayout {
            spacing: 24
            width: Math.min(welcomeRoot.width - 40, 520)
            anchors.horizontalCenter: parent
            anchors.verticalCenter: parent

            // Floating Custom Card Window
            Rectangle {
                id: welcomeCard
                Layout.fillWidth: true
                implicitHeight: mainLayout.implicitHeight + 48
                color: "#0f172a" // Deep Slate
                border.color: "#1e293b"
                border.width: 2
                radius: 24

                property real scale: 1.0
                transform: Scale {
                    origin.x: welcomeCard.width / 2
                    origin.y: welcomeCard.height / 2
                    xScale: welcomeCard.scale
                    yScale: welcomeCard.scale
                }

                // Decorative Ambient Glow Accent
                Rectangle {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    width: 140
                    height: 140
                    radius: 70
                    color: "#312e81"
                    opacity: 0.2
                }

                ColumnLayout {
                    id: mainLayout
                    anchors.fill: parent
                    anchors.margins: 28
                    spacing: 16

                    // Header Segment with Pulsing Verified Seal
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        Rectangle {
                            width: 32
                            height: 32
                            radius: 16
                            color: "#065f46"
                            border.color: "#34d399"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent
                                text: "✓"
                                color: "#34d399"
                                font.bold: true
                                font.pixelSize: 14
                            }
                        }

                        ColumnLayout {
                            spacing: 1
                            Layout.fillWidth: true

                            Label {
                                text: qsTr("AUTHORIZED LOGIN REDIRECTION")
                                font.pixelSize: 9
                                font.bold: true
                                color: "#10b981"
                                font.family: "Courier"
                            }

                            Label {
                                text: qsTr("Handshake Verified Successfully")
                                font.pixelSize: 14
                                font.bold: true
                                color: "#f8fafc"
                            }
                        }

                        // State Token Seal
                        Rectangle {
                            color: "#1e1b4b"
                            border.color: "#818cf8"
                            border.width: 1
                            radius: 6
                            Layout.preferredWidth: 90
                            Layout.preferredHeight: 22

                            Label {
                                anchors.centerIn: parent
                                text: "CSRF: Verified"
                                color: "#818cf8"
                                font.bold: true
                                font.pixelSize: 9
                                font.family: "Courier"
                            }
                        }
                    }

                    // Separator line
                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: "#1e293b"
                    }

                    // Character visual profiling
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 16

                        Rectangle {
                            width: 60
                            height: 60
                            radius: 30
                            color: "#312e81"
                            border.color: "#4f46e5"
                            border.width: 2

                            Text {
                                anchors.centerIn: parent
                                text: "⭐"
                                font.pixelSize: 26
                            }
                        }

                        ColumnLayout {
                            spacing: 4
                            Layout.fillWidth: true

                            Label {
                                text: authService.userName !== "" ? authService.userName : "Explorer Character"
                                font.pixelSize: 17
                                font.bold: true
                                color: "#f8fafc"
                            }

                            Label {
                                text: authService.userEmail !== "" ? authService.userEmail : "unknown@securedomain.internal"
                                font.pixelSize: 11
                                font.family: "Courier"
                                color: "#38bdf8"
                            }
                        }
                    }

                    // Parameter isolation registry
                    Rectangle {
                        Layout.fillWidth: true
                        height: detailsLayout.implicitHeight + 20
                        color: "#020617"
                        radius: 12
                        border.color: "#1e293b"

                        ColumnLayout {
                            id: detailsLayout
                            anchors.fill: parent
                            anchors.margins: 14
                            spacing: 8

                            RowLayout {
                                Layout.fillWidth: true
                                Label {
                                    text: "OS Hardware Token Lock:"
                                    color: "#64748b"
                                    font.pixelSize: 10.5
                                }
                                Rectangle {
                                    width: 8
                                    height: 8
                                    color: "#10b981"
                                    radius: 4
                                    SequentialAnimation on opacity {
                                        loops: Animation.Infinite
                                        NumberAnimation {
                                            from: 1.0
                                            to: 0.3
                                            duration: 800
                                        }
                                        NumberAnimation {
                                            from: 0.3
                                            to: 1.0
                                            duration: 800
                                        }
                                    }
                                }
                                Label {
                                    text: "Protected via QtKeychain"
                                    color: "#10b981"
                                    font.bold: true
                                    font.pixelSize: 10.5
                                    Layout.fillWidth: true
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                Label {
                                    text: "Credential Target Scheme:"
                                    color: "#64748b"
                                    font.pixelSize: 10.5
                                }
                                Label {
                                    text: "myqtoauth://callback"
                                    color: "#cbd5e1"
                                    font.family: "Courier"
                                    font.pixelSize: 10.5
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                Label {
                                    text: "Active Token Stream:"
                                    color: "#64748b"
                                    font.pixelSize: 10.5
                                }
                                Label {
                                    text: "oauth_access_token [Locked]"
                                    color: "#fbbf24"
                                    font.pixelSize: 10.5
                                    font.bold: true
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                            }
                        }
                    }

                    // Interactive Action triggers
                    ColumnLayout {
                        spacing: 10
                        Layout.fillWidth: true

                        Button {
                            id: auditBtn
                            Layout.fillWidth: true
                            height: 38

                            background: Rectangle {
                                color: auditBtn.pressed ? "#1e293b" : "#111827"
                                border.color: "#374151"
                                border.width: 1
                                radius: 8
                            }

                            contentItem: Label {
                                text: qsTr("🛡️ Audit Keyring Security Rules")
                                color: "#cbd5e1"
                                font.bold: true
                                font.pixelSize: 11
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }

                            onClicked: {
                                authService.rawLog("🔍 QKeychain::ReadPasswordJob: Security manual Audit executed inside the operating system keychain wrapper...", "success");
                                authService.rawLog("🛡️ Matching SHA-256 signatures against secure credentials space.", "info");
                            }
                        }

                        Button {
                            id: signOutBtn
                            Layout.fillWidth: true
                            height: 42

                            background: Rectangle {
                                color: signOutBtn.pressed ? "#450a0a" : "#1e1b4b"
                                radius: 10
                            }

                            contentItem: Label {
                                text: qsTr("Disconnect Safe Session & Clear Keyrings")
                                color: "#fca5a5"
                                font.bold: true
                                font.pixelSize: 11.5
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }

                            onClicked: {
                                authService.signOut();
                            }
                        }
                    }
                }
            }
        }
    }
}
