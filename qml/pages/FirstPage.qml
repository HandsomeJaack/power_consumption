import QtQuick 2.0
import Sailfish.Silica 1.0
import powerstat.plotview 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    PageHeader {
        id: header
        //% "Battery stats"
        title: qsTr("Энергопотребление")
    }

    PlotView {
        id: plot
        width: parent.width*0.75
        height: parent.width/2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height*0.2
    }

    Item {
        onStateChanged: console.log(plot.lineCount)
    }

    property int lineCount: plot.lineCount
    readonly property var time: plot.diff

    Column {
        id: percantage
        anchors.right: plot.left
        anchors.top: plot.top
        anchors.topMargin: -parent.height*0.02

        Grid {
            columns: 1
            rows: lineCount
            rowSpacing: plot.height/10
            Repeater {
                model: lineCount
                Text {
                    color: "white"
                    text: 100 - index*20
                }
            }
        }
    }

    Column {
        anchors.top: percantage.bottom
        anchors.left: percantage.right
        Grid {
            columns: 3
            rows: 1
            columnSpacing: parent.width/5
            Repeater {
                model: [time[0], time[1], time[2]]
                delegate: Text {
                    color: "white"
                    text: modelData
                }
            }
        }
    }
}
