//import QtQuick 1.0
import QtQuick 2.5
import "component" as Comp
 
Item
{
    width: 200; height: 200
    // Visual is an item that defines some constants for the application
    // look and feel, e.g. the font sizes & colors etc.
    Comp.Visual
    {
        id: visual
    }
 
    // Example on using ModalDialog component.
    Comp.ModalDialog_t2
    {
        id: dialog_t2
 
        // Shown text can be set by modifying "text" property.
        //text: "Click OK to Accept this dialog. To send it away, click Cancel."
        // Always remember to define the size for the dialog.
        anchors.fill: parent
 
        // Demonstrating how one could keep application wide variables saved
        // in one place and use them conveniently all around the application.
        fontName: "Helvetica"
        fontColor: "#9DE352"
        fontColorButton: "#9DE352"
        fontSize: 16
 
        // Use these if you would like to change the Button look and feel.
        // See Visual.qml how these are defined. Remember to modify also the
        // ModalDialog.qml, since this functionality is disabled currently.
//        buttonBackground: visual.buttonComponent
//        buttonBackgroundPressed: visual.buttonPressedComponent
 
        onAccepted:
        {
            console.log("Dialog accepted signal received!");
 
           // I know could call a C/C++function here or send a signal, but, if this modal dialog is to be used/called in several places of C/C++ application
           // how i could manage such thing?
        }
 
 
        onCancelled:
        {
            console.log("Dialog cancelled signal received.")
        }
    }
 
 
 
    function dialog_type_02()
    {
        dialog_t2.show();
    }
 
 
    // The model:
    ListModel
    {
        id: list_model
 
        Component.onCompleted: dialog_type_02()
    }
}

