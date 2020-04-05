import QtQuick 2.0
import Sailfish.Silica 1.0
import battery.chart 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    Chart {
        id: chart
    }

    PageHeader {
        id: header
        //% "Battery stats"
        title: qsTrId("la_battery-stats")
   }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        Text {
            text: chart.getTime();
        }
    }
}
