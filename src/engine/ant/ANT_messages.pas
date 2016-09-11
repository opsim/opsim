unit ANT_messages;

{$mode objfpc}{$H-}

interface

uses 
Classes,
SysUtils,
ANT_types,
LinkedList;

const 
  // messages
  ANT_MESSAGE_PAINT = 1;
  ANT_MESSAGE_ACTIVATE = 2;
  ANT_MESSAGE_DEACTIVATE = 3;
  ANT_MESSAGE_KEYPRESS = 4;
  ANT_MESSAGE_KEYRELEASE = 5;
  ANT_MESSAGE_KEYCHAR = 6;
  ANT_MESSAGE_MOUSEDOWN = 7;
  ANT_MESSAGE_MOUSEUP = 8;
  ANT_MESSAGE_MOUSEMOVE = 9;
  ANT_MESSAGE_DOUBLECLICK = 10;
  ANT_MESSAGE_MOUSEENTER = 11;
  ANT_MESSAGE_MOUSEEXIT = 12;
  ANT_MESSAGE_CLOSE = 13;
  ANT_MESSAGE_SCROLL = 14;
  ANT_MESSAGE_RESIZE = 15;
  ANT_MESSAGE_MOVE = 16;
  ANT_MESSAGE_POPUPCLOSE = 17;
  ANT_MESSAGE_HINTTIMER = 18;
  ANT_MESSAGE_FREEME = 19;
  ANT_MESSAGE_DROPENTER = 20;
  ANT_MESSAGE_DROPEXIT = 21;
  ANT_MESSAGE_HSCROLL = 22;
  ANT_MESSAGE_ABOUT = 23;
  ANT_MESSAGE_USER = 50000;
  ANT_MESSAGE_KILLME = MaxInt;

type 
  ANT_MsgParmMouse = record
    x: integer;                //< X position of the mouse
    y: integer;                //< Y position of the mouder
    buttons: word;             //< button state of the mouse
    shiftstate: TShiftState;   //< shift state of the keyboard
    delta: integer;            //< drag distance of mouse
    timestamp: TDateTime;      //< timestamp when event was generated
  end;

  ANT_MsgParmKeyboard = record
    keychar: longint;          //< ASCII character fro keybord
    shiftstate: TShiftState;   //< shift state of the keyboard 
  end;

  ANT_MsgParmUser = record
    param1: integer;   //< custom message 1
    param2: integer;   //< custom message 4
    param3: integer;   //< custom message 3
  end;

  ANT_MsgParmRect = record
    top: integer;      //< top of rectangle being sent
    left: integer;     //< left of rectangle being sent
    width: integer;    //< width of rectangle being sent
    height: integer;   //< height of rectangle being sent
  end;

  pANT_MessageParams = ^ANT_MessageParams;
  ANT_MessageParams = record
    case integer of                         
      0: (mouse: ANT_MsgParmMouse);         //< mouse event record
      1: (keyboard: ANT_MsgParmKeyboard);   //< keyboard event record
      2: (rect: ANT_MsgParmRect);           //< rectangel event record
      3: (user: ANT_MsgParmUser);           //< used defined event record
  end;

  pANT_MessageRec = ^ANT_MessageRec;

  ANT_MessageRec = record
    next: pANT_MessageRec;       //< next item in the list
    prev: pANT_MessageRec;       //< previous item in the list

    win: pWindow;                //< ANT window that has sent message
    mcode: integer;              //< message code
    params: ANT_MessageParams;   //< event data
  end;

{
   Sends message to the event queue
   @param Window: window where the essage originates from
   @param MsgCode: message code
   @param Params: event data
}
procedure antPostMessage(Window: pWindow; MsgCode: integer; Params: ANT_MessageParams);
{
   Reads the last mesage from the queue
   @return The last message from the queue, otherwise nil.
}
function antReadLastMessage: pANT_MessageRec;
{
   Deletes message from the queue
   @param msg: message to be deleted
}
procedure antDeleteMessage(msg: pANT_MessageRec);

var
  eventfunc: ANT_EventCallback = nil;

implementation

var 
  msglist: ListBase;

procedure antPostMessage(Window: pWindow; MsgCode: integer; Params: ANT_MessageParams);
var
  msg: pANT_MessageRec;
begin
  msg := callocN(sizeof(ANT_MessageRec));

  msg^.mcode := MsgCode;
  move(Params, msg^.params, sizeof(ANT_MessageParams));
  msg^.win := Window;

  addtail(@msglist, msg);
end;

function antReadLastMessage: pANT_MessageRec;
begin
  exit(msglist.last);
end;

procedure antDeleteMessage(msg: pANT_MessageRec);
begin
  remlink(@msglist, msg);
   freeN(msg);
end;

end.
