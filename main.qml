import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2
import SaveUpdater 1.0

Window {
    visible: true
    width: 640
    height: 150
    title: "MHST2 SaveFile Updater"

    Text {
        id: labelFile
        x: 10
        y: 10
        text: "File:"
        font.pixelSize: 30
    }

    Rectangle {
        id: rectFile
        anchors.left: labelFile.right
        anchors.leftMargin: 5
        anchors.top: labelFile.top
        anchors.topMargin: 1
        anchors.right: parent.right
        anchors.rightMargin: 100
        anchors.bottom: labelFile.bottom

        border.width: 1
        border.color: "#8080ff"
        visible: true

        TextField {
            id: textFile
            font.pixelSize: 14
            leftPadding: 10

            anchors.fill: parent
        }
    }

    Text {
        id: labelID
        anchors.left: labelFile.left
        anchors.top: labelFile.bottom
        anchors.topMargin: 10
        text: "ID:"
        font.pixelSize: 30
    }

    Rectangle {
        id: rectID
        anchors.left: labelID.right
        anchors.leftMargin: 5
        anchors.top: labelID.top
        anchors.topMargin: 1
        width: 70
        height: rectFile.height

        border.width: 1
        border.color: "#8080ff"
        visible: true

        TextField {
            id: textID
            font.pixelSize: 14
            leftPadding: 10

            anchors.fill: parent
        }
    }

    Text {
        id: labelCount
        anchors.left: rectID.right
        anchors.top: labelID.top
        anchors.leftMargin: 10
        text: "Count:"
        font.pixelSize: 30
    }

    Rectangle {
        id: rectCount
        anchors.left: labelCount.right
        anchors.leftMargin: 5
        anchors.top: labelCount.top
        anchors.topMargin: 1
        width: 70
        height: rectFile.height

        border.width: 1
        border.color: "#8080ff"
        visible: true

        TextField {
            id: textCount
            font.pixelSize: 14
            leftPadding: 10

            anchors.fill: parent
        }
    }

    Button {
        id: buttonBrowse
        anchors.left: rectFile.right
        anchors.leftMargin: 5
        anchors.top: rectFile.top
        anchors.topMargin: 1
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: rectFile.bottom
        text: "Browse"
        font.pointSize: 14
        spacing: 3

        onClicked: {
            dlgBrowse.open()
        }
    }

    Button {
        id: buttonGo
        anchors.left: buttonBrowse.left
        anchors.top: buttonBrowse.bottom
        anchors.topMargin: 10
        anchors.right: buttonBrowse.right
        text: "Go"
        font.pointSize: 14
        spacing: 3
        onClicked: {
            saveUpdater.save_update(textFile.text.toString())
            ppMsgText.text = "Done"
            popupMessage.open()
        }
    }

    Button {
        id: buttonAdj
        anchors.left: rectCount.right
        anchors.top: labelCount.top
        anchors.leftMargin: 10
        text: "Adjust"
        font.pointSize: 14
        spacing: 3
        onClicked: {
            saveUpdater.save_adjust(textFile.text.toString(), textID.text.toString(), textCount.text.toString())
            ppMsgText.text = "Done"
            popupMessage.open()
        }
    }

    FileDialog {
        id: dlgBrowse
        title: "choose MHST2 save file"
        onAccepted: {
            var s
            if (Qt.platform.os == "linux") {
                s = dlgBrowse.fileUrl.toString().substring(7)
            }
            else if (Qt.platform.os == "windows") {
                s = dlgBrowse.fileUrl.toString().substring(8)
            }

            textFile.text = decodeURIComponent(s)
        }

    }

    SaveUpdater {
        id: saveUpdater

        onSigErrorMessage: {
            console.log(msg)
            ppMsgText.text = msg
            popupMessage.open()
        }
    }

    Popup {
        id: popupMessage
        width: 200
        height: 150
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        contentItem: Text {
            id: ppMsgText
        }
    }
}
