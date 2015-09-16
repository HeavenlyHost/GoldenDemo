import QtQuick 2.0
import com.thales.cppxbutton 1.0;

Rectangle {
    id: button
    width: 145; height: 60
    color: "blue"
    smooth: true; radius: 9
    property alias text: label.text
    property alias name: myButton.sName

    property string normalcolor: "#719FCB"
    property string clickedcolor: "green"

    property string grad3: normalcolor

    border {color: "#B9C5D0"; width: 1}

    gradient: Gradient {
        GradientStop {color: "#CFF7FF"; position: 0.0}
        GradientStop {color: "#99C0E5"; position: 0.57}
        GradientStop {color: grad3; position: 0.9}
    }

    CppXButton{
        id: myButton
        sName: "XButton"
    }

    Text {
        id: label
        anchors.centerIn: parent
        text: "Click Me!!!"
        font.pointSize: 12
        color: "blue"
    }

    MouseArea {
        anchors.fill: parent
        onReleased: {
            button.grad3 = normalcolor
            console.log(text + " clicked," + myButton.sName);
        }
        onPressed: {
            button.grad3 = clickedcolor
            console.log(text + " clicked," + myButton.sName);
        }
    }
}

