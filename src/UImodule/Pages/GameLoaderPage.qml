import QtQuick 2.15
import QtQuick.Controls.Universal
import QtQuick.Layouts
import QtQuick.Dialogs
import General

Page {
    id: pageLoader
    property bool isLoaded: false
    property bool isReady: false
    // required property int itemId
    required property string url
    required property string pageTitle
    required property string avatar
    required property string resoucesUrl
    property int typeId: 0
    property var stackView: stackView
    // initiaize user session

    NetworkAccessItemList {
        id: itemConnection
    }

    Loader {
        id: loader
        anchors.fill: parent
        source: "qrc:/ui/UIModule/Components/LoadingPage.qml"
    }

    Component.onCompleted: {
        // console.log(UserSession.userFullName)
        itemConnection.getItemGameTypeApiRequest(url);
    }

    MessageDialog {
        id: errorDialog
        title: "Error"
        buttons: MessageDialog.Ok
    }

    Connections {
        target: itemConnection
        function onItemGameTypeChanged() {
            pageLoader.isLoaded = true;
            loader.setSource("GameListPage.qml", {
                "model": itemConnection.itemGameType,
                "avatar": pageLoader.avatar,
                "pageTitle": pageLoader.pageTitle,
                "resoucesUrl": "modules",
                "moduleName": "modules",
                "typeId": pageLoader.typeId
            });
        }

        function onRequestFailed(stt) {
            // console.log(stt);
            errorDialog.text = stt;
            errorDialog.open();
        }
    }
}
