import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Universal
import QtQuick.Dialogs
import General

Page {
    id: dashboard
    // anchors.fill: parent

    property int board_width: 0
    property int board_height: 0
    property string activeUserName: ""
    property string activeUserUuid: ""
    property string activeUserId: ""

    NeworkAccessManager {
        id: service
    }

    Image {
        id: bg_image
        source: "qrc:/ui/images/Artboard.png"
        fillMode: Image.PreserveAspectCrop
        mipmap: true
        smooth: true
        anchors.fill: parent
    }

    ListModel {
        id: userModel

        ListElement {
            username: "J1"
            ageBracket: "1-4 years"
            desc: "No Dyslexia, no ADHD "
        }
        ListElement {
            username: "s1"
            ageBracket: "5-8 years"
            desc: "No Dyslexia, no ADHD "
        }
        ListElement {
            username: "a1"
            ageBracket: "9-11"
            desc: "No Dyslexia, no ADHD "
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // Left Column - Table
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredWidth: dashboard.width * 0.4
            Layout.fillHeight: true

            border.color: "#c0c0c0"
            radius: 5

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10

                Label {
                    text: "Login Credentials"
                    font.bold: true
                    font.pixelSize: 18
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 40
                    color: "#dddddd"

                    RowLayout {
                        anchors.fill: parent

                        Label {
                            text: "Username"
                            Layout.fillWidth: true
                            font.bold: true
                        }

                        Label {
                            text: "Age Bracket"
                            Layout.fillWidth: true
                            font.bold: true
                        }
                        // Label {
                        //     text: "Description"
                        //     Layout.fillWidth: true
                        //     font.bold: true
                        // }
                    }
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: userModel

                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 40
                        border.color: "#e0e0e0"

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 5

                            Label {
                                text: username
                                Layout.fillWidth: true
                            }

                            Label {
                                text: ageBracket
                                Layout.fillWidth: true
                            }
                            // Label {
                            //     text: desc
                            //     Layout.fillWidth: true
                            // }
                        }
                    }
                }
            }
        }

        // Right Column - Form
        Rectangle {
            Layout.preferredWidth: dashboard.width * 0.6
            Layout.fillHeight: true
            color: "transparent"
            // border.color: "#c0c0c0"
            radius: 5

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20
                width: parent.width * 0.6

                Label {
                    text: "Enter Student ID"
                    font.pixelSize: 28
                    color: "black"
                    horizontalAlignment: Text.AlignHCenter
                    Layout.alignment: Qt.AlignHCenter
                }

                TextField {
                    id: studentinput
                    placeholderText: "Student ID"
                    font.pixelSize: 38
                    Layout.fillWidth: true
                    height: 100
                    focus: true
                    text: "s1"

                    background: Rectangle {
                        radius: 10
                        border.color: "#ffffff"
                        border.width: 2
                        color: "#fffffc"
                    }
                }

                Button {
                    text: "Submit"
                    // buttonText: "Submit"
                    Layout.fillWidth: true
                    font.pixelSize: 38
                    height: 100
                    visible: !service.isLoadingData
                    background: Rectangle {
                        radius: 10
                        color: "#4CAF50"
                    }

                    onClicked: {
                        // console.log("Submit button clicked")
                        // service.helloworld();
                        service.getProfileApiRequest("https://mvp.dyxi.site/application/get-student-details?studentId=" + studentinput.text);
                    }
                }
            }
        }
    }

    Rectangle {
        id: opaqueIndicatorBg
        anchors.centerIn: parent
        color: "#E0E0E0"

        BusyIndicator {
            id: indi
            anchors.centerIn: parent
            width: 100
            height: 100
            running: service.isLoadingData
            visible: running
            // contentItem: Item{
            //     opacity: indi.running ? 1 : 0
            // }

            // background: {
            //     color: "grey"
            //     opacity: 0.5
            // }
        }
    }

    MessageDialog {
        id: errorDialog
        title: "Error"
        buttons: MessageDialog.Ok
    }

    Component.onCompleted: {
        studentinput.forceActiveFocus();
        // console.log(core.name);
    }

    Connections {
        target: service
        // onChangePage:{
        //     // if(pageName == "SelectGamePage"){ // define the page that needs  to go to
        //     //     StackView.push("SelectGamePage.qml");
        //     // }
        // }
        function onChangePage(pageName) {
            // console.log("Select Page Emitted")
            // if(pageName === "SelectGamePage"){ // define the page that needs  to go to
            stackView.push(pageName + ".qml", { //
                "activeUserName": dashboard.activeUserName
            // "stackView": stackView // Pass the stackView to the categoryPage

            });
            // }
        }

        function onRequestFailed(stt) {
            // console.log(stt);
            errorDialog.text = stt;
            errorDialog.open();
        }

        function onProfileDataMapChanged() {
            let dataMap = service.profileDataMap;
            dashboard.activeUserName = dataMap["studentName"];
        }
    }
}
