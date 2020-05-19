import QtQuick 2.0
import Sailfish.Silica 1.0
import powerstat.plotview 1.0
import powerstat.cpuview 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.Portrait

    SilicaFlickable {
        clip: true
        width: Screen.width
        height: Screen.height
        anchors.fill: parent
        contentHeight: plot.height + cpuUsage.height

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
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: parent.height*0.35
            }
        }

        Column {
            id: percantage

            anchors {
                right: plot.left
                rightMargin: Screen.width/100
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
                columns: plot.measurementTime.size
                rows: 1
                columnSpacing: parent.width/plot.measurementTime.lentgh
                Repeater {
                    model: plot.measurementTime
                    delegate: Text {
                        color: Theme.highlightColor
                        text: modelData
                    }
                }
            }
        }

        CpuView {
            id: cpuUsage
            lineWidth: 28
            anchors {
                right: cpuPercentaage.left
                left: applications.right
                top: timing.bottom
            }
            color: Theme.highlightColor
        }

        Column {
            id: applications

            anchors {
                left: percantage.left
                top: timing.bottom
            }

            Grid {
                columns: 1
                rows: cpuUsage.apps.lentgh

                Repeater {
                    model:  cpuUsage.apps
                    delegate: Text {
                        id: appUsage
                        color: Theme.highlightColor
                        text: modelData
                    }
                }
            }
        }

        Column {
            id: cpuPercentaage

            anchors {
               right:  parent.right
               rightMargin: Theme.fontSizeExtraSmall
               top: timing.bottom
            }

            Grid {
                columns: 1
                rows: cpuUsage.percantage.length

                Repeater {
                    model:  cpuUsage.percentage
                    delegate: Text {
                        id: appPercantage
                        color: Theme.highlightColor
                        text: modelData
                    }
                }
            }
        }


        VerticalScrollDecorator {}
    }
}
