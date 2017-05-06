#include "qmyapplication.h"
#include <QEvent>
#include <QDebug>
#include <QKeyEvent>

#define MAX_EVENTS 204
static char *events[MAX_EVENTS];

QMyApplication::QMyApplication(int &argc, char **argv) : QApplication(argc, argv)
{
    qDebug() << "QMyApplication";
    setEventList();
}


bool QMyApplication::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << "Key press" << keyEvent->key();
    }

    int et = event->type();
    char *type = (char *)"Unknown";
    if (et < MAX_EVENTS) {
        switch(et) {
        case QEvent::ChildAdded:
        case QEvent::ChildRemoved:
        case QEvent::ChildPolished:
        case QEvent::Polish:
        case QEvent::PolishRequest:
        case QEvent::LayoutRequest:
        case QEvent::MetaCall:
        case QEvent::Timer:
        case QEvent::ShowToParent:
        case QEvent::HideToParent:
        case QEvent::WindowActivate:
        case QEvent::WindowDeactivate:
        case QEvent::Style:
        case QEvent::Paint:
            break;
        case QEvent::Enter:
        case QEvent::Leave:
        case QEvent::MouseMove:
        case QEvent::GraphicsSceneMouseMove:
        case QEvent::GraphicsSceneWheel:
        case QEvent::Wheel:
        case QEvent::Destroy:
        case QEvent::Quit:
        case QEvent::Close:
        case QEvent::Move:
        case QEvent::Resize:
        case QEvent::Show:
        case QEvent::Hide:
        case QEvent::FocusIn:
        case QEvent::FocusOut:
        case QEvent::ApplicationActivate:
        case QEvent::ActivationChange:
        case QEvent::Clipboard:
        case QEvent::NonClientAreaMouseMove:
        case QEvent::NonClientAreaMouseButtonPress:
        case QEvent::NonClientAreaMouseButtonRelease:
        case QEvent::WinIdChange:
        case QEvent::SockAct:
        case QEvent::ParentChange:
        case QEvent::PaletteChange:
        case QEvent::Create:
        case QEvent::AcceptDropsChange:
        case QEvent::GraphicsSceneHoverEnter:
        case QEvent::GraphicsSceneHoverLeave:
        case QEvent::GraphicsSceneHoverMove:
        case QEvent::GraphicsSceneMousePress:
        case QEvent::GraphicsSceneHelp:
        case QEvent::GraphicsSceneMouseRelease:
        case QEvent::ApplicationDeactivate:
        case QEvent::ApplicationPaletteChange:
        case QEvent::MouseTrackingChange:
        case QEvent::ToolTip:
        case 67:
        case 70:
            if (!mFi.isDebugMode())
                break;
        default:
            if (events[et] != NULL)
                type = events[et];
            //qDebug() << "----------------- > eventFilter" << et << type << obj->objectName();
            break;
        }
    }


    return QApplication::eventFilter(obj, event);
}

#ifdef HANDLENOTIFY
bool QMyApplication::notify(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << "Key press" << keyEvent->key();
    }
    //qDebug() << "notify" <<event->type();
    return obj->event(event);
}
#endif

void QMyApplication::setEventList() {
    events[0] =	(char *)"None";
    events[1] =	(char *)"Timer";
    events[2] =	(char *)"MouseButtonPress";
    events[3] =	(char *)"MouseButtonRelease";
    events[4] =	(char *)"MouseButtonDblClick";
    events[5] =	(char *)"MouseMove";
    events[6] =	(char *)"KeyPress";
    events[7] =	(char *)"KeyRelease";
    events[8] =	(char *)"FocusIn";
    events[9] =	(char *)"FocusOut";
    events[10] =	(char *)"Enter";
    events[11] =	(char *)"Leave";
    events[12] =	(char *)"Paint";
    events[13] =	(char *)"Move";
    events[14] =	(char *)"Resize";
    events[15] =	(char *)"Create";
    events[16] =	(char *)"Destroy";
    events[17] =	(char *)"Show";
    events[18] =	(char *)"Hide";
    events[19] =	(char *)"Close";
    events[20] =	(char *)"Quit";
    events[21] =	(char *)"ParentChange";
    events[22] =	(char *)"ThreadChange";
    events[24] =	(char *)"WindowActivate";
    events[25] =	(char *)"WindowDeactivate";
    events[26] =	(char *)"ShowToParent";
    events[27] =	(char *)"HideToParent";
    events[30] =	(char *)"Accel";
    events[31] =	(char *)"Wheel";
    events[32] =	(char *)"AccelAvailable";
    events[33] =	(char *)"WindowTitleChange";
    events[34] =	(char *)"WindowIconChange";
    events[35] =	(char *)"ApplicationWindowIconChange";
    events[36] =	(char *)"ApplicationFontChange";
    events[37] =	(char *)"ApplicationLayoutDirectionChange";
    events[38] =	(char *)"ApplicationPaletteChange";
    events[39] =	(char *)"PaletteChange";
    events[40] =	(char *)"Clipboard";
    events[42] =	(char *)"Speech";
    events[43] =	(char *)"MetaCall";
    events[50] =	(char *)"SockAct";
    events[51] =	(char *)"ShortcutOverride";
    events[52] =	(char *)"DeferredDelete";
    events[60] =	(char *)"DragEnter";
    events[61] =	(char *)"DragMove";
    events[62] =	(char *)"DragLeave";
    events[63] =	(char *)"Drop";
    events[64] =	(char *)"DragResponse";
    events[67] =	(char *)"ChildInsertedRequest";
    events[68] =	(char *)"ChildAdded";
    events[69] =	(char *)"ChildPolished";
    events[70] =	(char *)"ChildInserted";
    events[71] =	(char *)"ChildRemoved";
    events[72] =	(char *)"LayoutHint";
    events[73] =	(char *)"ShowWindowRequest";
    events[74] =	(char *)"PolishRequest";
    events[75] =	(char *)"Polish";
    events[76] =	(char *)"LayoutRequest";
    events[77] =	(char *)"UpdateRequest";
    events[78] =	(char *)"UpdateLater";
    events[79] =	(char *)"EmbeddingControl";
    events[80] =	(char *)"ActivateControl";
    events[81] =	(char *)"DeactivateControl";
    events[82] =	(char *)"ContextMenu";
    events[83] =	(char *)"InputMethod";
    events[86] =	(char *)"AccessibilityPrepare";
    events[87] =	(char *)"TabletMove";
    events[88] =	(char *)"LocaleChange";
    events[89] =	(char *)"LanguageChange";
    events[90] =	(char *)"LayoutDirectionChange";
    events[91] =	(char *)"Style";
    events[92] =	(char *)"TabletPress";
    events[93] =	(char *)"TabletRelease";
    events[94] =	(char *)"OkRequest";
    events[95] =	(char *)"HelpRequest";
    events[96] =	(char *)"IconDrag";
    events[97] =	(char *)"FontChange";
    events[98] =	(char *)"EnabledChange";
    events[99] =	(char *)"ActivationChange";
    events[100] =	(char *)"StyleChange";
    events[101] =	(char *)"IconTextChange";
    events[102] =	(char *)"ModifiedChange";
    events[103] =	(char *)"WindowBlocked";
    events[104] =	(char *)"WindowUnblocked";
    events[105] =	(char *)"WindowStateChange";
    events[109] =	(char *)"MouseTrackingChange";
    events[110] =	(char *)"ToolTip";
    events[111] =	(char *)"WhatsThis";
    events[112] =	(char *)"StatusTip";
    events[113] =	(char *)"ActionChanged";
    events[114] =	(char *)"ActionAdded";
    events[115] =	(char *)"ActionRemoved";
    events[116] =	(char *)"FileOpen";
    events[117] =	(char *)"Shortcut";
    events[118] =	(char *)"WhatsThisClicked";
    events[119] =	(char *)"AccessibilityHelp";
    events[120] =	(char *)"ToolBarChange";
    events[121] =	(char *)"ApplicationActivate";
    events[122] =	(char *)"ApplicationDeactivate";
    events[123] =	(char *)"QueryWhatsThis";
    events[124] =	(char *)"EnterWhatsThisMode";
    events[125] =	(char *)"LeaveWhatsThisMode";
    events[126] =	(char *)"ZOrderChange";
    events[127] =	(char *)"HoverEnter";
    events[128] =	(char *)"HoverLeave";
    events[129] =	(char *)"HoverMove";
    events[130] =	(char *)"AccessibilityDescription";
    events[131] =	(char *)"ParentAboutToChange";
    events[132] =	(char *)"WinEventAct";
    events[150] =	(char *)"EnterEditFocus";
    events[151] =	(char *)"LeaveEditFocus";
    events[152] =	(char *)"AcceptDropsChange";
    events[153] =	(char *)"MenubarUpdated";
    events[154] =	(char *)"ZeroTimerEvent";
    events[155] =	(char *)"GraphicsSceneMouseMove";
    events[156] =	(char *)"GraphicsSceneMousePress";
    events[157] =	(char *)"GraphicsSceneMouseRelease";
    events[158] =	(char *)"GraphicsSceneMouseDoubleClick";
    events[159] =	(char *)"GraphicsSceneContextMenu";
    events[160] =	(char *)"GraphicsSceneHoverEnter";
    events[161] =	(char *)"GraphicsSceneHoverMove";
    events[162] =	(char *)"GraphicsSceneHoverLeave";
    events[163] =	(char *)"GraphicsSceneHelp";
    events[164] =	(char *)"GraphicsSceneDragEnter";
    events[165] =	(char *)"GraphicsSceneDragMove";
    events[166] =	(char *)"GraphicsSceneDragLeave";
    events[167] =	(char *)"GraphicsSceneDrop";
    events[168] =	(char *)"GraphicsSceneWheel";
    events[169] =	(char *)"KeyboardLayoutChange";
    events[170] =	(char *)"DynamicPropertyChange";
    events[171] =	(char *)"TabletEnterProximity";
    events[172] =	(char *)"TabletLeaveProximity";
    events[173] =	(char *)"NonClientAreaMouseMove";
    events[174] =	(char *)"NonClientAreaMouseButtonPress";
    events[175] =	(char *)"NonClientAreaMouseButtonRelease";
    events[176] =	(char *)"NonClientAreaMouseButtonDblClick";
    events[177] =	(char *)"MacSizeChange";
    events[178] =	(char *)"ContentsRectChange";
    events[179] =	(char *)"MacGLWindowChange";
    events[180] =	(char *)"FutureCallOut";
    events[181] =	(char *)"GraphicsSceneResize";
    events[182] =	(char *)"GraphicsSceneMove";
    events[183] =	(char *)"CursorChange";
    events[184] =	(char *)"ToolTipChange";
    events[185] =	(char *)"NetworkReplyUpdated";
    events[186] =	(char *)"GrabMouse";
    events[187] =	(char *)"UngrabMouse";
    events[188] =	(char *)"GrabKeyboard";
    events[189] =	(char *)"UngrabKeyboard";
    events[191] =	(char *)"MacGLClearDrawable";
    events[192] =	(char *)"StateMachineSignal";
    events[193] =	(char *)"StateMachineWrapped";
    events[194] =	(char *)"TouchBegin";
    events[195] =	(char *)"TouchUpdate";
    events[196] =	(char *)"TouchEnd";
    events[197] =	(char *)"NativeGesture";
    events[198] =	(char *)"Gesture";
    events[199] =	(char *)"RequestSoftwareInputPanel";
    events[200] =	(char *)"CloseSoftwareInputPanel";
    events[201] =	(char *)"UpdateSoftKeys";
    events[202] =	(char *)"GestureOverride";
    events[203] =	(char *)"WinIdChange";
}
