import QtQuick 2.15
import QtQuick.Controls.Universal
import QtQuick.Layouts
import QtQuick.Dialogs
// import mvpDyxi
import  General
// import "Buttons"

Page {
    id:dashboard
    // anchors.fill: parent

    property int board_width: 0
    property int board_height: 0
    property string activeUserName:""
    property string activeUserUuid: ""
    property string activeUserId:""



    NeworkAccessManager{
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

    // Rectangle{
    //     anchors.fill:  parent *0.15
    //     color: "#ccc"
    // }



    ColumnLayout{
        anchors.centerIn: parent
        spacing: 20
        width: parent.width *0.6

        Label{
            text: "Enter Student ID"
            font.pixelSize: 28
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }

        TextField{
            id: studentinput
            placeholderText: "Student ID"
            font.pixelSize: 38
            Layout.fillWidth: true
            height: 100
            focus:  true


            background: Rectangle{
                radius: 10
                border.color: "#ffffff"
                border.width: 2
                color: "#fffffc"

            }
        }



        Button{
            text: "Submit"
            // buttonText: "Submit"
            Layout.fillWidth: true
            font.pixelSize: 38
            height: 100
            visible: !service.isLoadingData
            background: Rectangle{
                radius: 10
                color: "#4CAF50"
            }

            onClicked:{
                // console.log("Submit button clicked")
                // service.helloworld();
                service.getProfileApiRequest("https://mvp.dyxi.site/application/get-student-details?studentId="+studentinput.text);
            }
        }
    }

    Rectangle{
        anchors.centerIn: parent
        id: opaqueIndicatorBg
        color: "#E0E0E0"




        BusyIndicator{
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

    MessageDialog{
        id:errorDialog
        title:"Error"
        buttons: MessageDialog.Ok
    }


    Component.onCompleted: {
        studentinput.forceActiveFocus();
        // console.log(core.name);
    }

    Connections{
        target: service
        // onChangePage:{
        //     // if(pageName == "SelectGamePage"){ // define the page that needs  to go to
        //     //     StackView.push("SelectGamePage.qml");
        //     // }
        // }
        function onChangePage(pageName){
            // console.log("Select Page Emitted")
            // if(pageName === "SelectGamePage"){ // define the page that needs  to go to
                stackView.push(pageName+".qml", { //
                                   "activeUserName" : dashboard.activeUserName
                                   // "stackView": stackView // Pass the stackView to the categoryPage

                               });
            // }
        }

        function onRequestFailed(stt){
            // console.log(stt);
            errorDialog.text = stt;
            errorDialog.open();
        }

        function onProfileDataMapChanged(){
            let dataMap = service.profileDataMap;
            dashboard.activeUserName = dataMap["studentName"]

        }



    }



}
