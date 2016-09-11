
unit ANT_main;

{$mode objfpc}{$H-}

interface

uses 
GL, ANT_messages, ANT_types;

const 
  //mouse buttons.
  ANT_MOUSE_BUTTON_LEFT      = $00000001;
  ANT_MOUSE_BUTTON_MIDDLE    = $00000002;
  ANT_MOUSE_BUTTON_RIGHT     = $00000003;

  //mouse button state.
  ANT_RELEASE                = $00000000;
  ANT_PRESS                  = $00000001;
  ANT_REPEAT                 = $00000002;

  //standard cursor shapes
  ANT_ARROW_CURSOR           = $00000001;
  ANT_IBEAM_CURSOR           = $00000002;
  ANT_CROSSHAIR_CURSOR       = $00000003;
  ANT_HAND_CURSOR            = $00000004;
  ANT_HRESIZE_CURSOR         = $00000005;
  ANT_VRESIZE_CURSOR         = $00000006;


{
   Initializes the ANT library
   @return True is successfull otherwise False
}
function ANT_Init: boolean;
{
   Terminates the ANT library
}
procedure ANT_Terminate;

{
   Creates a window and its associated context.
   @param Width: the width of the window
   @param Height: the height of the window
   @param title: the title of the window
   @return a reference to the created window
}
function ANT_CreateWindow(width, height: integer; title: PChar): pWindow;
{
   Destroys a window and its associated context.
   @param win: the reference to the window being destroyed
}
procedure ANT_DestroyWindow(win: pWindow);
{
   Swaps the front and back buffers of the specified window.
   @param win: the reference to the window
}
procedure ANT_SwapBuffers(win: pWindow);
{
   Retrieves the size of the framebuffer of the specified window.
   @param win: the reference to the window
   @param Width: the width of the window
   @param Height: the height of the window
}
procedure ANT_GetFrameBufferSize(win: pWindow; out width, height: integer);

{
   Sets the error callback procedure
   @param pointer to error callback:
}
procedure ANT_SetErrorCallback(errorCallback: ANT_ErrorCallback);
{
   Sets the event callback procedure
   @param pointer to event callback:
}
procedure ANT_SetEventCallback(eventCallback: ANT_EventCallback);
{
   This is the procedure will poll for any pending events and put them in 
   the eventlist. Next the eventlist is checked and if needed the event 
   callback function is called.
}
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
var
  win: pWindow;
begin
  //destroy any windows that are not destroyed yet
  win := windowlist.first;

  while win <> nil do
  begin
    ANT_DestroyWindow(win);

    win := win^.next;
  end;
end;

function ANT_CreateWindow(width, height: integer; title: PChar): pWindow;
begin
{$IFDEF MSWINDOWS}
  exit(gdi_CreateWindow(width, height, title));
{$ENDIF}
{$IFDEF LINUX}
  exit(X11_CreateWindow(width, height, title));
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

procedure ANT_GetFrameBufferSize(win: pWindow; out width, height: integer);
begin
{$IFDEF MSWINDOWS}
  gdi_GetFrameBufferSize(win, width, height);
{$ENDIF}
{$IFDEF LINUX}
  X11_GetFrameBufferSize(win, width, height);
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
