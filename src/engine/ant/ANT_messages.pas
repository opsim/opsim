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
    x: integer;
    y: integer;
    buttons: word;
    shiftstate: TShiftState;
    delta: integer;
    timestamp: TDateTime;
    state: word;
  end;

  ANT_MsgParmKeyboard = record
    keychar: longint;
    shiftstate: TShiftState;
  end;

  ANT_MsgParmUser = record
    param1: integer;
    param2: integer;
    param3: integer;
  end;

  ANT_MsgParmRect = record
    top: integer;
    left: integer;
    width: integer;
    height: integer;
  end;

  pANT_MessageParams = ^ANT_MessageParams;
  ANT_MessageParams = record
    case integer of 
      0: (mouse: ANT_MsgParmMouse);
      1: (keyboard: ANT_MsgParmKeyboard);
      2: (rect: ANT_MsgParmRect);
      3: (user: ANT_MsgParmUser);
  end;

  pANT_MessageRec = ^ANT_MessageRec;

  ANT_MessageRec = record
    next, prev: pANT_MessageRec;

    win: pWindow;
    mcode: integer;
    params: ANT_MessageParams;
  end;

procedure antPostMessage(Window: pWindow; MsgCode: integer; Params: ANT_MessageParams);
function antReadLastMessage: pANT_MessageRec;
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
