import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import com.thales.timers 1.0

Rectangle{
    id: rectangle1
    property int margin: 11

    width:720
    height:720

    anchors.fill: parent

    ColumnLayout{
        id:rootlayout
        anchors.fill: parent
        anchors.margins: margin

        XButton{
            text: "Hello Kelvin"
            name: "X Button 1"
            color: "Green"
            radius: 20
        }

        XButton{
            text: "Hello Kelvin"
            name: "X Button 2"
        }

        MyTimer{
            id:myTime
        }

        GroupBox{
            id:gridbox
            antialiasing: false
            flat: true
            title: "Toggles"
            Layout.fillWidth: true;

           GridLayout{
               id:gridLayout
               rows:2
               flow:GridLayout.TopToBottom
//               anchors.fill: parent

               Text{
                   id: mytext
                   text: "current time: " + myTime.cTime
               }

               Text{
                   id: mytext2
                   text: "current time: " + myTime.cTime
               }

               CheckBox{
                   text: qsTr("Enable Timer")
                   checked: myTime.eTimer
                   onCheckedChanged: myTime.eTimer = checked
               }

               CheckBox{
                   text: qsTr("Enable Timer")
                   checked: myTime.eTimer
                   onCheckedChanged: myTime.eTimer = checked
               }
           }

        }
    }
}
