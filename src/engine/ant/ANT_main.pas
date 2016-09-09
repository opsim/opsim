
unit ANT_main;

{$mode objfpc}{$H-}

interface

uses 
GL, ANT_messages, ANT_types;

const 
  //mouse buttons.
  ANT_MOUSE_BUTTON_LEFT = 1;
  ANT_MOUSE_BUTTON_MIDDLE = 2;
  ANT_MOUSE_BUTTON_RIGHT = 3;

  //mouse button state.
  ANT_RELEASE = 0;
  ANT_PRESS = 1;
  ANT_REPEAT = 2;

function ANT_Init: boolean;
procedure ANT_Terminate;

function ANT_CreateWindow(Width, Height: integer; title: PChar): pWindow;
procedure ANT_DestroyWindow(win: pWindow);
procedure ANT_SwapBuffers(win: pWindow);

procedure ANT_SetErrorCallback(errorCallback: ANT_ErrorCallback);
procedure ANT_SetEventCallback(eventCallback: ANT_EventCallback);
procedure ANT_PollEvents;

implementation

uses 
Classes,
SysUtils,
ANT_base,
{$IFDEF MSWINDOWS}
ANT_gdi,
{$ENDIF}
{$IFDEF LINUX}
ANT_X11,
{$ENDIF}
LinkedList;

function ANT_Init: boolean;
begin
  exit(True);
end;

procedure ANT_Terminate;
begin

end;

function ANT_CreateWindow(Width, Height: integer; title: PChar): pWindow;
begin
{$IFDEF MSWINDOWS}
  exit(gdi_CreateWindow(Width, Height, title));
{$ENDIF}
{$IFDEF LINUX}
  exit(X11_CreateWindow(Width, Height, title));
{$ENDIF}
end;

procedure ANT_DestroyWindow(win: pWindow);
begin
{$IFDEF MSWINDOWS}
  gdi_DestroyWindow(win);
{$ENDIF}
{$IFDEF LINUX}
  X11_DestroyWindow(win);
{$ENDIF}

  //remove and free window object
  remlink(@windowlist, win);
  freeN(win);
end;

procedure ANT_SwapBuffers(win: pWindow);
begin
{$IFDEF MSWINDOWS}
  gdi_SwapBuffers(win);
{$ENDIF}
{$IFDEF LINUX}
  X11_SwapBuffers(win);
{$ENDIF}
end;

procedure ANT_SetErrorCallback(errorCallback: ANT_ErrorCallback);
begin
  errfunc := errorCallback;
end;

procedure ANT_SetEventCallback(eventCallback: ANT_EventCallback);
begin
  eventfunc := eventCallback;
end;

procedure ANT_PollEvents;
var
  event: pANT_MessageRec;
begin
{$IFDEF MSWINDOWS}
  gdi_PollEvents;
{$ENDIF}
{$IFDEF LINUX}
  X11_PollEvents;
{$ENDIF}

  if eventfunc <> nil then
    begin
      event := antReadLastMessage;

      if event <> nil then
        begin
          eventfunc(event);

          antDeleteMessage(event);
        end;
    end;
end;

end.
