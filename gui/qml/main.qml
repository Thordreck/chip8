import QtQuick 2.13
import QtQuick.Controls 2.13

import "components"
import "components/DebugView"

ApplicationWindow
{
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("chip8")

    menuBar: TopMenu {
    }

    DebugView {
        id: debugView
    }

   Rectangle {
       id: chip8InterpreterScreen
       color: "black"
       width: window.width
       height: 50
       anchors.left: window.left
       anchors.top: parent.top
       anchors.bottom: parent.bottom
   }

}
