import QtQuick 2.15
import QtQuick.Controls

Page {

    id: entryPoint

       // Route passed from the previous page
       property string routeName: ""
       property int typeId: 0
       property int id: 0
    property var stackView

    signal navigate(string page)

       // String -> QML page mapping
       readonly property var pageMap: ({
           // "home": "HomePage.qml",
           // "settings": "SettingsPage.qml",
           // "profile": "ProfilePage.qml",
           // "about": {"url":"AboutPage.qml", 'img":"link to image"},
           "ShapeMatching1":"qrc:/module/shape_recog/ShapeRecog/Recognition_Shape_Explorer_Level1.qml",
             "MathDice2":"qrc:/modules/phonetics/kidphonics/KidPhonics/KidPhonicsMain.qml",
           "cognimazeS1":"qrc:/modules/adhd/cognimaze/CogniMaze1/CogniMazeMain.qml",
          // "wordregnition1":"qrc:/modules/phonetics/wordrecog2/Phonetics/WordRecognitionGame2/WordRecog2Main.qml",
            //"Numicon1":"qrc:/modules/dyscalculia/numicon1/Dyscalculia/Numicon1/NumiconMain.qml",
             "RobotTalk1":"qrc:/module/writing/letterexplorer/LetterExplorer/LetterExplorerMain.qml",
             "ShapeMatchingEasy":"qrc:/modules/recognition/shapematchin/ShapeMatchin/ShapeMatchingMain.qml",
          //  "NeuroPlay_Jigsaw":"qrc:/modules/adhd/jigsaw/Jigsaw/JigsawMain.qml",
            "textTracing1":"/module/writing/Writing/WritingMainPlayGround.qml",
            "cognimazej1":"qrc:/modules/recognition/cognimaze/Cognimaze/HardPage.qml"

   })



    Component.onCompleted: {
        // navigate(pageMap[routeName])
         console.log("stackView =", stackView)
         Qt.callLater(redirect)
        console.log("EntryPoint")
    }

    function redirect() {
        var target = pageMap[routeName]

        if (!target) {
            console.warn("Unknown route:", routeName)
            target = "NotFoundPage.qml"
        }

        // Push destination while preserving existing stack
        // stackView.view.push(target)
        stackView.replace(target, {
            id: id,
            typeId: typeId,
                              // img:target.img
        })

        // Optional: remove this redirect page from the stack
        // StackView.view.pop()
    }



}
