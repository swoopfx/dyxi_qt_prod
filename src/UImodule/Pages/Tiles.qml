import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    // ================================================================
    // PROPERTIES
    // ================================================================

    property int selectedChildIndex: -1

    // Network state
    property bool childrenLoading: true
    property bool childrenLoaded: false
    property string childrenError: ""

    // ================================================================
    // SIGNALS
    // ================================================================

    signal childSelected(int childId, string childName)
    signal addChildRequested
    signal manageChildRequested(int childId)
    signal viewProgressRequested(int childId)

    signal childRegistrationSubmitted(string fullName, string dateOfBirth, string gender)

    // ================================================================
    // CHILD MODEL
    // ================================================================

    ListModel {
        id: childrenModel
    }

    // ================================================================
    // DATE HELPERS
    // ================================================================

    readonly property int currentYear: new Date().getFullYear()

    readonly property int youngestBirthYear: currentYear - 1

    readonly property int birthYearCount: 15

    function monthNumber(monthIndex) {
        return monthIndex + 1;
    }

    function formattedDate() {
        if (dayCombo.currentIndex < 0 || monthCombo.currentIndex < 0 || yearCombo.currentIndex < 0) {
            return "";
        }

        return dayCombo.currentText + " " + monthCombo.currentText + " " + yearCombo.currentText;
    }

    function backendDate() {
        if (dayCombo.currentIndex < 0 || monthCombo.currentIndex < 0 || yearCombo.currentIndex < 0) {
            return "";
        }

        var day = dayCombo.currentText;

        var month = String(monthCombo.currentIndex + 1).padStart(2, "0");

        var year = yearCombo.currentText;

        return year + "-" + month + "-" + day;
    }

    function resetChildForm() {
        fullNameField.clear();

        dayCombo.currentIndex = -1;
        monthCombo.currentIndex = -1;
        yearCombo.currentIndex = -1;

        genderCombo.currentIndex = -1;

        formError.text = "";
    }

    // ================================================================
    // LOAD CHILDREN
    // ================================================================

    function loadChildren() {
        childrenLoading = true;
        childrenLoaded = false;
        childrenError = "";

        childrenModel.clear();

        /*
         * Keep the Add Child tile in the GridView immediately.
         */

        childrenModel.append({
            isAddChild: true,
            childId: -1,
            childName: "",
            age: 0,
            grade: "",
            avatarColor: "",
            avatarText: "",
            progress: 0
        });

        /*
         * C++:
         *
         * networkaccess.loadChildren()
         */

        networkaccess.loadChildren();
    }

    // ================================================================
    // SERVER RESPONSE
    // ================================================================

    function showChildren(children) {
        childrenModel.clear();

        // ------------------------------------------------------------
        // SERVER CHILDREN
        // ------------------------------------------------------------

        for (var i = 0; i < children.length; ++i) {
            var child = children[i];

            var name = child.childName || child.name || "Unknown";

            childrenModel.append({
                isAddChild: false,
                childId: child.childId !== undefined ? child.childId : 0,
                childName: name,
                age: child.age !== undefined ? child.age : 0,
                grade: child.grade !== undefined ? child.grade : "N/A",
                avatarColor: child.avatarColor !== undefined ? child.avatarColor : "#E8E0FF",
                avatarText: child.avatarText !== undefined ? child.avatarText : name.charAt(0).toUpperCase(),
                progress: child.progress !== undefined ? child.progress : 0
            });
        }

        // ------------------------------------------------------------
        // ALWAYS ADD ADD-CHILD TILE LAST
        // ------------------------------------------------------------

        childrenModel.append({
            isAddChild: true,
            childId: -1,
            childName: "",
            age: 0,
            grade: "",
            avatarColor: "",
            avatarText: "",
            progress: 0
        });

        childrenLoading = false;
        childrenLoaded = true;
    }

    // ================================================================
    // NETWORK FAILURE
    // ================================================================

    function childrenLoadFailed(message) {
        childrenLoading = false;
        childrenLoaded = false;

        childrenError = message && message.length > 0 ? message : "Unable to load children's profiles.";

        /*
         * Keep Add Child available even after failure.
         */

        childrenModel.clear();

        childrenModel.append({
            isAddChild: true,
            childId: -1,
            childName: "",
            age: 0,
            grade: "",
            avatarColor: "",
            avatarText: "",
            progress: 0
        });
    }

    // ================================================================
    // C++ NETWORK CONNECTION
    // ================================================================

    Connections {

        target: networkaccess

        function onChildrenLoaded(children) {
            root.showChildren(children);
        }

        function onChildrenLoadFailed(message) {
            root.childrenLoadFailed(message);
        }
    }

    // ================================================================
    // INITIALIZATION
    // ================================================================

    Component.onCompleted: {
        root.loadChildren();
    }

    // ================================================================
    // GRID DELEGATE
    // ================================================================

    Component {
        id: delegateComponent

        Item {
            id: delegateItem

            width: childrenGridView.cellWidth

            height: childrenGridView.cellHeight

            // ========================================================
            // ADD CHILD TILE
            // ========================================================

            Rectangle {
                id: addChildTile

                visible: isAddChild === true

                anchors.fill: parent

                anchors.margins: 6

                radius: 17

                color: "#FCFAFF"

                border.color: "#D9D0FF"

                border.width: 1

                MouseArea {

                    anchors.fill: parent

                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        root.addChildRequested();

                        root.resetChildForm();

                        addChildDialog.open();
                    }
                }

                ColumnLayout {

                    anchors.centerIn: parent

                    spacing: 7

                    Rectangle {

                        Layout.alignment: Qt.AlignHCenter

                        Layout.preferredWidth: 46

                        Layout.preferredHeight: 46

                        radius: 23

                        color: "#EEE AFF".replace(" ", "")

                        Label {

                            anchors.centerIn: parent

                            text: "+"

                            color: "#7658DC"

                            font.pixelSize: 25

                            font.bold: true
                        }
                    }

                    Label {

                        text: "Add Child"

                        color: "#634AC9"

                        font.pixelSize: 12

                        font.bold: true

                        Layout.alignment: Qt.AlignHCenter
                    }

                    Label {

                        text: "Register another\nchild profile"

                        color: "#929BB0"

                        font.pixelSize: 9

                        horizontalAlignment: Text.AlignHCenter

                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }

            // ========================================================
            // LOADING SKELETON
            // ========================================================

            Rectangle {
                id: loadingCard

                visible: root.childrenLoading && isAddChild !== true

                anchors.fill: parent

                anchors.margins: 6

                radius: 17

                color: "#FFFFFF"

                border.color: "#E7EAF2"

                border.width: 1

                ColumnLayout {

                    anchors.fill: parent

                    anchors.margins: 13

                    spacing: 10

                    // Avatar skeleton

                    Rectangle {

                        Layout.preferredWidth: 56
                        Layout.preferredHeight: 56

                        radius: 28

                        color: "#EEF0F6"

                        SequentialAnimation on opacity {

                            loops: Animation.Infinite

                            NumberAnimation {
                                to: 0.45
                                duration: 700
                            }

                            NumberAnimation {
                                to: 1.0
                                duration: 700
                            }
                        }
                    }

                    // Name skeleton

                    Rectangle {

                        Layout.fillWidth: true

                        height: 14

                        radius: 7

                        color: "#EEF0F6"
                    }

                    // Details skeleton

                    Rectangle {

                        Layout.preferredWidth: 110

                        height: 10

                        radius: 5

                        color: "#F1F2F6"
                    }

                    // Status skeleton

                    Rectangle {

                        width: 52

                        height: 18

                        radius: 9

                        color: "#EEF0F6"
                    }

                    Item {
                        Layout.fillHeight: true
                    }

                    // Progress skeleton

                    Rectangle {

                        Layout.fillWidth: true

                        height: 7

                        radius: 3.5

                        color: "#ECEEF4"
                    }

                    Rectangle {

                        Layout.fillWidth: true

                        height: 34

                        radius: 9

                        color: "#F1F2F7"
                    }
                }
            }

            // ========================================================
            // CHILD CARD
            // ========================================================

            Rectangle {
                id: childTile

                visible: !root.childrenLoading && isAddChild !== true

                anchors.fill: parent

                anchors.margins: 6

                radius: 17

                color: "#FFFFFF"

                border.color: root.selectedChildIndex === index ? "#8A6CFF" : "#E4E8F2"

                border.width: root.selectedChildIndex === index ? 2 : 1

                MouseArea {

                    anchors.fill: parent

                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        root.selectedChildIndex = index;

                        root.childSelected(childId, childName);
                    }
                }

                ColumnLayout {

                    anchors.fill: parent

                    anchors.margins: 13

                    spacing: 6

                    // =================================================
                    // AVATAR
                    // =================================================

                    RowLayout {

                        Layout.fillWidth: true

                        Rectangle {

                            Layout.preferredWidth: 56
                            Layout.preferredHeight: 56

                            radius: 28

                            color: avatarColor

                            Label {

                                anchors.centerIn: parent

                                text: avatarText

                                color: "#4F4770"

                                font.pixelSize: 22

                                font.bold: true
                            }

                            Rectangle {

                                width: 12
                                height: 12

                                radius: 6

                                color: "#48C985"

                                border.color: "#FFFFFF"

                                border.width: 2

                                anchors.right: parent.right

                                anchors.bottom: parent.bottom
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        Button {

                            Layout.preferredWidth: 28

                            Layout.preferredHeight: 28

                            background: Rectangle {

                                radius: 8

                                color: "#F6F4FF"
                            }

                            contentItem: Label {

                                text: "⋮"

                                color: "#7660D9"

                                font.pixelSize: 17

                                horizontalAlignment: Text.AlignHCenter

                                verticalAlignment: Text.AlignVCenter
                            }

                            onClicked: {
                                root.manageChildRequested(childId);
                            }
                        }
                    }

                    // =================================================
                    // NAME
                    // =================================================

                    Label {

                        text: childName

                        color: "#202D50"

                        font.pixelSize: 13

                        font.bold: true

                        Layout.fillWidth: true

                        elide: Text.ElideRight
                    }

                    // =================================================
                    // AGE / GRADE
                    // =================================================

                    Label {

                        text: "Age " + age + "  •  " + grade

                        color: "#8994AA"

                        font.pixelSize: 9
                    }

                    // =================================================
                    // STATUS
                    // =================================================

                    Rectangle {

                        Layout.preferredWidth: 52

                        Layout.preferredHeight: 18

                        radius: 9

                        color: "#E9F9EE"

                        Label {

                            anchors.centerIn: parent

                            text: "● Active"

                            color: "#28A66C"

                            font.pixelSize: 8

                            font.bold: true
                        }
                    }

                    // =================================================
                    // PROGRESS
                    // =================================================

                    RowLayout {

                        Layout.fillWidth: true

                        Label {

                            text: "Progress"

                            color: "#69758D"

                            font.pixelSize: 9

                            Layout.fillWidth: true
                        }

                        Label {

                            text: progress + "%"

                            color: "#674FD4"

                            font.pixelSize: 10

                            font.bold: true
                        }
                    }

                    Rectangle {

                        Layout.fillWidth: true

                        height: 7

                        radius: 3.5

                        color: "#ECEAF3"

                        Rectangle {

                            width: parent.width * Math.max(0, Math.min(100, progress)) / 100

                            height: parent.height

                            radius: 3.5

                            color: "#7C4DFF"
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                    }

                    // =================================================
                    // DASHBOARD
                    // =================================================

                    Button {

                        Layout.fillWidth: true

                        Layout.preferredHeight: 34

                        background: Rectangle {

                            radius: 9

                            color: "#F4F1FF"

                            border.color: "#DED6FF"

                            border.width: 1
                        }

                        contentItem: Label {

                            text: "View Dashboard"

                            color: "#684BD0"

                            font.pixelSize: 10

                            font.bold: true

                            horizontalAlignment: Text.AlignHCenter

                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            root.selectedChildIndex = index;

                            root.viewProgressRequested(childId);
                        }
                    }
                }
            }
        }
    }

    // ================================================================
    // MAIN BACKGROUND
    // ================================================================

    Rectangle {

        anchors.fill: parent

        color: "#F8F9FE"

        // ============================================================
        // DECORATION
        // ============================================================

        Rectangle {

            width: 260
            height: 150

            radius: 75

            color: "#EEE9FF"

            opacity: 0.35

            anchors.left: parent.left

            anchors.bottom: parent.bottom

            anchors.leftMargin: -120

            anchors.bottomMargin: -90
        }

        Rectangle {

            width: 220
            height: 140

            radius: 70

            color: "#E8F5FF"

            opacity: 0.30

            anchors.right: parent.right

            anchors.bottom: parent.bottom

            anchors.rightMargin: -100

            anchors.bottomMargin: -80
        }

        // ============================================================
        // PAGE
        // ============================================================

        ScrollView {
            id: page

            anchors.fill: parent

            anchors.leftMargin: 28

            anchors.rightMargin: 28

            anchors.topMargin: 6

            anchors.bottomMargin: 10

            clip: true

            contentWidth: availableWidth

            ColumnLayout {

                width: page.availableWidth

                spacing: 12

                // ====================================================
                // HEADER
                // ====================================================

                RowLayout {

                    Layout.fillWidth: true

                    Layout.preferredHeight: 58

                    spacing: 14

                    Image {

                        Layout.preferredWidth: 78

                        Layout.preferredHeight: 52

                        source: "qrc:/ui/images/dyxi_logo_42.png"

                        fillMode: Image.PreserveAspectFit

                        smooth: true
                    }

                    ColumnLayout {

                        Layout.fillWidth: true

                        spacing: 1

                        Label {

                            text: "My Ward"

                            color: "#202D50"

                            font.pixelSize: 24

                            font.bold: true
                        }

                        Label {

                            text: "Manage your children's learning journey"

                            color: "#7B87A3"

                            font.pixelSize: 10
                        }
                    }

                    Button {
                        id: addButton

                        Layout.preferredWidth: 128

                        Layout.preferredHeight: 40

                        background: Rectangle {

                            radius: 11

                            color: addButton.pressed ? "#6848E8" : "#7C4DFF"
                        }

                        contentItem: RowLayout {

                            spacing: 5

                            Label {

                                text: "+"

                                color: "#FFFFFF"

                                font.pixelSize: 19

                                font.bold: true
                            }

                            Label {

                                text: "Add Child"

                                color: "#FFFFFF"

                                font.pixelSize: 11

                                font.bold: true
                            }
                        }

                        onClicked: {
                            root.addChildRequested();

                            root.resetChildForm();

                            addChildDialog.open();
                        }
                    }
                }

                // ====================================================
                // SUMMARY
                // ====================================================

                Rectangle {

                    Layout.fillWidth: true

                    Layout.preferredHeight: 50

                    radius: 13

                    color: "#FFFFFF"

                    border.color: "#E5E9F3"

                    border.width: 1

                    RowLayout {

                        anchors.fill: parent

                        anchors.leftMargin: 13

                        anchors.rightMargin: 13

                        spacing: 9

                        Rectangle {

                            Layout.preferredWidth: 32

                            Layout.preferredHeight: 32

                            radius: 16

                            color: "#EEE AFF".replace(" ", "")

                            Label {

                                anchors.centerIn: parent

                                text: "♥"

                                color: "#7658DC"

                                font.pixelSize: 15
                            }
                        }

                        Label {

                            text: root.childrenLoading ? "Loading registered children..." : childrenModel.count - 1 + " registered " + (childrenModel.count - 1 === 1 ? "child" : "children")

                            color: "#34415D"

                            font.pixelSize: 11

                            font.bold: true

                            Layout.fillWidth: true
                        }

                        Label {

                            text: root.childrenLoading ? "Please wait" : "Select a profile to continue"

                            color: "#929BB0"

                            font.pixelSize: 9
                        }
                    }
                }

                // ====================================================
                // CHILDREN GRID
                // ====================================================

                Item {
                    id: childrenArea

                    Layout.fillWidth: true

                    Layout.preferredHeight: Math.max(250, childrenGridView.contentHeight)

                    // =================================================
                    // GRID
                    // =================================================

                    GridView {
                        id: childrenGridView

                        anchors.left: parent.left

                        anchors.right: parent.right

                        anchors.top: parent.top

                        anchors.bottom: parent.bottom

                        clip: true

                        interactive: false

                        model: childrenModel

                        property int columnCount: width >= 1180 ? 5 : width >= 880 ? 4 : width >= 620 ? 3 : 1

                        cellWidth: width / columnCount

                        cellHeight: 244

                        delegate: delegateComponent
                    }

                    // =================================================
                    // ERROR MESSAGE
                    // =================================================

                    Rectangle {
                        id: errorOverlay

                        anchors.fill: parent

                        visible: !root.childrenLoading && !root.childrenLoaded && root.childrenError.length > 0

                        color: "#FFFFFF"

                        radius: 17

                        border.color: "#F0D8DE"

                        border.width: 1

                        /*
                         * Keep the Add Child tile clickable.
                         * The error panel is only shown in the
                         * unused area if there is an error.
                         */

                        Column {

                            anchors.centerIn: parent

                            spacing: 10

                            Label {

                                text: "Unable to load profiles"

                                color: "#D94B61"

                                font.pixelSize: 12

                                font.bold: true

                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Label {

                                text: root.childrenError

                                color: "#8994AA"

                                font.pixelSize: 9

                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Button {

                                width: 110

                                height: 34

                                anchors.horizontalCenter: parent.horizontalCenter

                                background: Rectangle {

                                    radius: 9

                                    color: "#7C4DFF"
                                }

                                contentItem: Label {

                                    text: "Try Again"

                                    color: "#FFFFFF"

                                    font.pixelSize: 10

                                    font.bold: true

                                    horizontalAlignment: Text.AlignHCenter

                                    verticalAlignment: Text.AlignVCenter
                                }

                                onClicked: {
                                    root.loadChildren();
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // ====================================================================
    // ADD CHILD DIALOG
    // ====================================================================

    Dialog {
        id: addChildDialog

        modal: true

        anchors.centerIn: parent

        width: Math.min(parent.width * 0.72, 580)

        height: Math.min(parent.height * 0.86, 560)

        padding: 0

        closePolicy: Popup.CloseOnEscape

        background: Rectangle {

            radius: 22

            color: "#FFFFFF"

            border.color: "#E2E6F1"

            border.width: 1
        }

        contentItem: ColumnLayout {

            anchors.fill: parent

            anchors.margins: 26

            spacing: 13

            // ====================================================
            // MODAL HEADER
            // ====================================================

            RowLayout {

                Layout.fillWidth: true

                ColumnLayout {

                    Layout.fillWidth: true

                    spacing: 2

                    Label {

                        text: "Add Child"

                        color: "#202D50"

                        font.pixelSize: 22

                        font.bold: true
                    }

                    Label {

                        text: "Create a profile for your child"

                        color: "#8994AA"

                        font.pixelSize: 10
                    }
                }

                Button {

                    Layout.preferredWidth: 34

                    Layout.preferredHeight: 34

                    background: Rectangle {

                        radius: 10

                        color: "#F4F2FA"
                    }

                    contentItem: Label {

                        text: "×"

                        color: "#6F7890"

                        font.pixelSize: 22

                        horizontalAlignment: Text.AlignHCenter

                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: addChildDialog.close()
                }
            }

            // ====================================================
            // FULL NAME
            // ====================================================

            ColumnLayout {

                Layout.fillWidth: true

                spacing: 5

                Label {

                    text: "Child's Full Name"

                    color: "#45516A"

                    font.pixelSize: 11

                    font.bold: true
                }

                TextField {
                    id: fullNameField

                    Layout.fillWidth: true

                    Layout.preferredHeight: 46

                    placeholderText: "Enter child's full name"

                    font.pixelSize: 12

                    leftPadding: 14

                    rightPadding: 14

                    background: Rectangle {

                        radius: 11

                        color: "#F9FAFD"

                        border.color: fullNameField.activeFocus ? "#8060E8" : "#DDE2EC"

                        border.width: fullNameField.activeFocus ? 2 : 1
                    }
                }
            }

            // ====================================================
            // DATE OF BIRTH
            // ====================================================

            ColumnLayout {

                Layout.fillWidth: true

                spacing: 5

                Label {

                    text: "Date of Birth"

                    color: "#45516A"

                    font.pixelSize: 11

                    font.bold: true
                }

                RowLayout {

                    Layout.fillWidth: true

                    spacing: 8

                    // =================================================
                    // DAY
                    // =================================================

                    ComboBox {
                        id: dayCombo

                        Layout.fillWidth: true

                        Layout.preferredHeight: 46

                        model: ["01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"]

                        currentIndex: -1

                        displayText: currentIndex < 0 ? "Day" : currentText

                        font.pixelSize: 11

                        background: Rectangle {

                            radius: 11

                            color: "#F9FAFD"

                            border.color: dayCombo.activeFocus ? "#8060E8" : "#DDE2EC"

                            border.width: dayCombo.activeFocus ? 2 : 1
                        }
                    }

                    // =================================================
                    // MONTH
                    // =================================================

                    ComboBox {
                        id: monthCombo

                        Layout.fillWidth: true

                        Layout.preferredHeight: 46

                        model: ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]

                        currentIndex: -1

                        displayText: currentIndex < 0 ? "Month" : currentText

                        font.pixelSize: 11

                        background: Rectangle {

                            radius: 11

                            color: "#F9FAFD"

                            border.color: monthCombo.activeFocus ? "#8060E8" : "#DDE2EC"

                            border.width: monthCombo.activeFocus ? 2 : 1
                        }
                    }

                    // =================================================
                    // YEAR
                    // =================================================

                    ComboBox {
                        id: yearCombo

                        Layout.fillWidth: true

                        Layout.preferredHeight: 46

                        model: ListModel {
                            id: birthYearModel
                        }

                        currentIndex: -1

                        displayText: currentIndex < 0 ? "Year" : currentText

                        font.pixelSize: 11

                        background: Rectangle {

                            radius: 11

                            color: "#F9FAFD"

                            border.color: yearCombo.activeFocus ? "#8060E8" : "#DDE2EC"

                            border.width: yearCombo.activeFocus ? 2 : 1
                        }

                        Component.onCompleted: {
                            birthYearModel.clear();

                            for (var i = 0; i < birthYearCount; i++) {
                                birthYearModel.append({
                                    year: String(youngestBirthYear - i)
                                });
                            }
                        }

                        delegate: ItemDelegate {

                            width: yearCombo.width

                            height: 42

                            text: model.year

                            highlighted: yearCombo.highlightedIndex === index

                            contentItem: Text {

                                text: model.year

                                color: "#34415D"

                                font.pixelSize: 11

                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        textRole: "year"
                    }
                }

                // =================================================
                // DATE PREVIEW
                // =================================================

                Rectangle {

                    Layout.fillWidth: true

                    Layout.preferredHeight: 36

                    radius: 9

                    color: "#F5F2FF"

                    visible: dayCombo.currentIndex >= 0 && monthCombo.currentIndex >= 0 && yearCombo.currentIndex >= 0

                    Label {

                        anchors.centerIn: parent

                        text: "Selected date: " + formattedDate()

                        color: "#684BD0"

                        font.pixelSize: 10

                        font.bold: true
                    }
                }
            }

            // ====================================================
            // GENDER
            // ====================================================

            ColumnLayout {

                Layout.fillWidth: true

                spacing: 5

                Label {

                    text: "Gender"

                    color: "#45516A"

                    font.pixelSize: 11

                    font.bold: true
                }

                ComboBox {
                    id: genderCombo

                    Layout.fillWidth: true

                    Layout.preferredHeight: 46

                    model: ["Female", "Male", "Other"]

                    currentIndex: -1

                    displayText: currentIndex < 0 ? "Select gender" : currentText

                    font.pixelSize: 11

                    background: Rectangle {

                        radius: 11

                        color: "#F9FAFD"

                        border.color: genderCombo.activeFocus ? "#8060E8" : "#DDE2EC"

                        border.width: genderCombo.activeFocus ? 2 : 1
                    }
                }
            }

            // ====================================================
            // ERROR
            // ====================================================

            Label {
                id: formError

                Layout.fillWidth: true

                visible: text.length > 0

                text: ""

                color: "#D94B61"

                font.pixelSize: 10

                wrapMode: Text.WordWrap
            }

            Item {
                Layout.fillHeight: true
            }

            // ====================================================
            // BUTTONS
            // ====================================================

            RowLayout {

                Layout.fillWidth: true

                spacing: 10

                // =================================================
                // CANCEL
                // =================================================

                Button {

                    Layout.fillWidth: true

                    Layout.preferredHeight: 44

                    background: Rectangle {

                        radius: 11

                        color: "#F2F1F7"
                    }

                    contentItem: Label {

                        text: "Cancel"

                        color: "#667087"

                        font.pixelSize: 11

                        font.bold: true

                        horizontalAlignment: Text.AlignHCenter

                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        resetChildForm();

                        addChildDialog.close();
                    }
                }

                // =================================================
                // REGISTER
                // =================================================

                Button {
                    id: submitChildButton

                    Layout.fillWidth: true

                    Layout.preferredHeight: 44

                    background: Rectangle {

                        radius: 11

                        color: submitChildButton.pressed ? "#6848E8" : "#7C4DFF"
                    }

                    contentItem: Label {

                        text: "Register Child"

                        color: "#FFFFFF"

                        font.pixelSize: 11

                        font.bold: true

                        horizontalAlignment: Text.AlignHCenter

                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {

                        // =========================================
                        // VALUES
                        // =========================================

                        var fullName = fullNameField.text.trim();

                        var dateOfBirth = backendDate();

                        var gender = genderCombo.currentIndex >= 0 ? genderCombo.currentText : "";

                        // =========================================
                        // VALIDATION
                        // =========================================

                        if (fullName.length === 0) {
                            formError.text = "Please enter the child's full name.";

                            fullNameField.forceActiveFocus();

                            return;
                        }

                        if (dayCombo.currentIndex < 0 || monthCombo.currentIndex < 0 || yearCombo.currentIndex < 0) {
                            formError.text = "Please select the child's date of birth.";

                            return;
                        }

                        if (gender.length === 0) {
                            formError.text = "Please select the child's gender.";

                            genderCombo.forceActiveFocus();

                            return;
                        }

                        // =========================================
                        // SEND TO C++
                        // =========================================

                        root.childRegistrationSubmitted(fullName, dateOfBirth, gender);

                        /*
                             * Do not add a fake child locally.
                             *
                             * Once C++ completes registration,
                             * reloadChildren() can retrieve the
                             * authoritative server data.
                             */

                        resetChildForm();

                        addChildDialog.close();

                        root.loadChildren();
                    }
                }
            }
        }
    }
}
