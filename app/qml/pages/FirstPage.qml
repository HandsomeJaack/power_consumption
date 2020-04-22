import QtQuick 2.0
import Sailfish.Silica 1.0
import powerstat.plotview 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.Portrait

    PageHeader {
        id: header
        //% "Battery stats"
        title: qsTr("Потребление энергии")
    }

    PlotView {
        id: plot
        color: Theme.highlightColor
        lineCount: 5
        width: parent.width*0.75
        height: parent.width/2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height*0.2
    }

    readonly property var time: plot.diff

    Column {
        id: percantage

        anchors {
            right: plot.left
            rightMargin: parent.width/100
            top: plot.top
            topMargin: -Theme.fontSizeMedium/2
        }

        Grid {
            columns: 1
            rows: plot.lineCount
            rowSpacing: plot.height/plot.lineCount - (Theme.fontSizeMedium + 2)
            Repeater {
                model: plot.lineCount
                Text {
                    id: rep
                    color: Theme.highlightColor
                    text: 100 - index*(100/(plot.lineCount - 1)) + "%"
                }
            }
        }
    }

    Column {
        id: timing

        anchors {
            top: plot.bottom
            horizontalCenter: parent.horizontalCenter
        }

        Grid {
            columns: 3
            rows: 1
            columnSpacing: parent.width/4
            Repeater {
                model: [time[0], time[1], time[2]]
                delegate: Text {
                    color: Theme.highlightColor
                    text: modelData
                }
            }
        }
    }
}
