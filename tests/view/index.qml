import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    width: 640
    height: 480

    Button {
        id: centerButton
        text: "Test"
        anchors.centerIn: parent
        property var counter: 0
        onClicked: {
            Stimmt.sendMessage("test", counter++);
        }
    }
}