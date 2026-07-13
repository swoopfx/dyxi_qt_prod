import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtOAuthClient

Item {
    id: loginRoot
    anchors.fill: parent

    // Property to receive the AuthService backend
    property var authService: null

    // Slide-in Animation upon load
    Component.onCompleted: {
        loginEntranceAnim.start();
        if (authService) {
            authService.rawLog("✨ Loaded separate Login.qml component successfully.", "info");
        }
    }

    SequentialAnimation {
        id: loginEntranceAnim
        NumberAnimation {
            target: loginContent
            property: "opacity"
            from: 0
            to: 1
            duration: 350
            easing.type: Easing.OutQuad
        }
        NumberAnimation {
            target: loginContent
            property: "scale"
            from: 0.97
            to: 1.0
            duration: 250
            easing.type: Easing.OutBack
        }
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: -1
        clip: true

        ColumnLayout {
            id: loginContent
            spacing: 24
            width: Math.min(loginRoot.width - 40, 500)
            anchors.centerIn: parent

            property real scale: 1.0
            transform: Scale {
                origin.x: loginContent.width / 2
                origin.y: loginContent.height / 2
                xScale: loginContent.scale
                yScale: loginContent.scale
            }

            // Security Shield Icon and Intro text
            ColumnLayout {
                spacing: 12
                Layout.alignment: Qt.AlignHCenter

                Rectangle {
                    width: 80
                    height: 80
                    color: "#0f172a"
                    border.color: "#1e293b"
                    border.width: 2
                    radius: 20
                    Layout.alignment: Qt.AlignHCenter

                    Text {
                        anchors.centerIn: parent
                        text: "🛡️"
                        font.pixelSize: 36
                    }
                }

                Label {
                    text: qsTr("Character Connection Hub")
                    font.pixelSize: 22
                    font.bold: true
                    color: "#f8fafc"
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: qsTr("Sign in via Google or Apple. Tokens are stored natively in OS Keyrings using QtKeychain.")
                    font.pixelSize: 11
                    color: "#94a3b8"
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                }
            }

            // Authentication Status Bar
            Rectangle {
                Layout.fillWidth: true
                height: 52
                color: authService.isBusy ? "#172554" : "#0f172a"
                border.color: authService.isBusy ? "#2563eb" : "#1e293b"
                border.width: 1
                radius: 12

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 14
                    spacing: 8

                    BusyIndicator {
                        running: authService.isBusy
                        implicitWidth: 20
                        implicitHeight: 20
                    }

                    Label {
                        text: authService.statusText
                        color: authService.isBusy ? "#60a5fa" : "#f1f5f9"
                        font.pixelSize: 12
                        font.bold: true
                        Layout.fillWidth: true
                    }
                }
            }

            // Social Connections Menu Group
            ColumnLayout {
                spacing: 12
                Layout.fillWidth: true

                // Google Action Handler
                Button {
                    id: googleBtn
                    Layout.fillWidth: true
                    height: 52
                    enabled: !authService.isBusy

                    background: Rectangle {
                        color: googleBtn.pressed ? "#1e293b" : "#0f172a"
                        border.color: "#334155"
                        border.width: 1
                        radius: 14
                    }

                    contentItem: RowLayout {
                        spacing: 12
                        layoutDirection: Qt.LeftToRight
                        anchors.centerIn: parent

                        Text {
                            text: "🔑"
                            font.pixelSize: 18
                        }

                        Label {
                            text: "Connect Google Account"
                            font.bold: true
                            color: "#f8fafc"
                            font.pixelSize: 13
                        }
                    }

                    onClicked: authService.startGoogleLogin()
                }

                // Apple Connect Handler
                Button {
                    id: appleBtn
                    Layout.fillWidth: true
                    height: 52
                    enabled: !authService.isBusy

                    background: Rectangle {
                        color: appleBtn.pressed ? "#1e293b" : "#0f172a"
                        border.color: "#334155"
                        border.width: 1
                        radius: 14
                    }

                    contentItem: RowLayout {
                        spacing: 12
                        anchors.centerIn: parent

                        Text {
                            text: "🍏"
                            font.pixelSize: 18
                        }

                        Label {
                            text: "Connect Apple Identity"
                            font.bold: true
                            color: "#f8fafc"
                            font.pixelSize: 13
                        }
                    }

                    onClicked: authService.startAppleLogin()
                }

                Button {
                    visible: authService.isBusy
                    Layout.fillWidth: true
                    height: 44

                    background: Rectangle {
                        color: "#991b1b"
                        radius: 10
                    }

                    contentItem: Label {
                        text: qsTr("🛑 Cancel Authorization Flow")
                        color: "#fca5a5"
                        font.bold: true
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: authService.cancelLogin()
                }
            }
        }
    }
}
