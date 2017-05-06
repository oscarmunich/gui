import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: repkeyboard
    visible: true
    width: 480
    height: 250

    Item {
        height: 250
        anchors.bottom: parent.bottom
        Column {
            Row {
                property int keycount: 10
                RepButton {
                    text: '0'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 0
                }
                RepButton {
                    text: '1'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 1
                }
                RepButton {
                    text: '2'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 2
                }
                RepButton {
                    text: '3'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 3
                }
                RepButton {
                    text: '4'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 4
                }
                RepButton {
                    text: '5'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 5
                }
                RepButton {
                    text: '6'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 6
                }
                RepButton {
                    text: '7'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 7
                }
                RepButton {
                    text: '8'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 8
                }
                RepButton {
                    text: '9'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 9
                }
            }
            Row {
                property int keycount: 10
                RepButton {
                    text: 'A'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 10
                }
                RepButton {
                    text: 'B'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 11
                }
                RepButton {
                    text: 'C'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 12
                }
                RepButton {
                    text: 'D'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 13
                }
                RepButton {
                    text: 'E'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 14
                }
                RepButton {
                    text: 'F'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 15
                }
                RepButton {
                    text: 'G'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 16
                }
                RepButton {
                    text: 'H'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 17
                }
                RepButton {
                    text: 'I'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 18
                }
                RepButton {
                    text: 'J'
                    bwidth: repkeyboard.width / parent.keycount
                    envar: cvaluebase.isEnabled
                    index: 19
                }
            }
            Row {
                property int keycount: 11
                RepButton {
                    text: 'K'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 20
                }
                RepButton {
                    text: 'L'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 21
                }
                RepButton {
                    text: 'M'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 22
                }
                RepButton {
                    text: 'N'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 23
                }
                RepButton {
                    text: 'O'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 24
                }
                RepButton {
                    text: 'P'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 25
                }
                RepButton {
                    text: 'Q'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 26
                }
                RepButton {
                    text: 'R'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 27
                }
                RepButton {
                    text: 'S'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 28
                }
                RepButton {
                    text: 'T'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 29
                }
                RepButton {
                    text: ''
                    isenabled: true
                    bwidth: repkeyboard.width / parent.keycount
                    specialImage: backspaceImage
                }
            }
            Row {
                property int keycount: 11
                RepButton {
                    text: 'U'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 30
                }
                RepButton {
                    text: 'V'
                    envar: cvaluebase.isEnabled
                    bwidth: repkeyboard.width / parent.keycount
                    index: 31
                }
                RepButton {
                    text: 'W'
                    envar: cvaluebase.isEnabled2
                    bwidth: repkeyboard.width / parent.keycount
                    index: 0
                }
                RepButton {
                    text: 'X'
                    envar: cvaluebase.isEnabled2
                    bwidth: repkeyboard.width / parent.keycount
                    index: 1
                }
                RepButton {
                    text: 'Y'
                    envar: cvaluebase.isEnabled2
                    bwidth: repkeyboard.width / parent.keycount
                    index: 2
                }
                RepButton {
                    text: 'Z'
                    envar: cvaluebase.isEnabled2
                    bwidth: repkeyboard.width / parent.keycount
                    index: 3
                }
                RepButton {
                    text: '/'
                    envar: cvaluebase.isEnabled2
                    bwidth: repkeyboard.width / parent.keycount
                    index: 4
                }
                RepButton {
                    text: '-'
                    envar: cvaluebase.isEnabled2
                    bwidth: repkeyboard.width / parent.keycount
                    index: 5
                }
                RepButton {
                    text: ' '
                    envar: cvaluebase.isEnabled2
                    bwidth: repkeyboard.width / parent.keycount
                    index: 6
                }
                RepButton {
                    text: '.'
                    envar: cvaluebase.isEnabled2
                    bwidth: repkeyboard.width / parent.keycount
                    index: 7
                }
                RepButton {
                    text: '_'
                    envar: cvaluebase.isEnabled2
                    bwidth: repkeyboard.width / parent.keycount
                    index: 8
                }
            }
        }
    }
}
