
unit ANT_main;

{$mode objfpc}

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
function ANT_Terminate: boolean;

function ANT_WindowShouldClose(win: pANTwindow): boolean;

procedure ANT_SetWindowShouldClose(win: pANTwindow; value: boolean);

{
   Creates a window and its associated context.
   @param Width: the width of the window
   @param Height: the height of the window
   @param title: the title of the window
   @return a reference to the created window
}
function ANT_CreateWindow(posx, posy, sizex, sizey: integer; title: PChar): pANTwindow;

{
   Destroys a window and its associated context.
   @param win: the reference to the window being destroyed
}
procedure ANT_DestroyWindow(win: pANTwindow);

function ANT_MakeCurrent(var win: pANTwindow): boolean;

{
   Swaps the front and back buffers of the specified window.
   @param win: the reference to the window
}
procedure ANT_SwapBuffers(win: pANTwindow);

{
   Retrieves the size of the framebuffer of the specified window.
   @param win: the reference to the window
   @param width: the width of the window
   @param height: the height of the window
}
procedure ANT_GetFrameBufferSize(win: pANTwindow; out width, height: integer);

{
   Sets the error callback procedure
   @param pointer to error callback:
}
procedure ANT_SetErrorCallback(errorCallback: ANT_ErrorCallback);

{
   This is the procedure will poll for any pending events and put them in 
   the eventlist. Next the eventlist is checked and if needed the event 
   callback function is called.
}
procedure ANT_PollEvents;

procedure ANT_SetCursor(cursor: byte);

procedure ANT_GetDisplayCoords(var dr: ANTRect);

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
  MEM_guardedalloc;

function ANT_Init: boolean;
begin
  exit(gdi_Init);
end;

function ANT_Terminate: boolean;
var
  win: pANTwindow;
begin
  //destroy any windows that are not destroyed yet
  win := windowlist.first;

  while win <> nil do
  begin
    ANT_DestroyWindow(win);

    win := win^.next;
  end;

  exit(gdi_Terminate);
end;

function ANT_WindowShouldClose(win: pANTwindow): boolean;
begin
  exit(win^.shouldClose);
end;

procedure ANT_SetWindowShouldClose(win: pANTwindow; value: boolean);
begin
  win^.shouldClose := value;
end;

function ANT_CreateWindow(posx, posy, sizex, sizey: integer; title: PChar): pANTwindow;
var
  win: pANTwindow = nil;
  res: boolean = False;
begin
  win := callocN(sizeof(ANTWindow), 'ANTWindow');

{$IFDEF MSWINDOWS}
  res := gdi_CreateWindow(win ,posx, posy, sizex, sizey, title);
{$ENDIF}
{$IFDEF LINUX}
  res := X11_CreateWindow(win, posx, posy, sizex, sizey, title);
{$ENDIF}

  if res then
    exit(win)
  else
  begin
    freeN(win);
    exit(nil);
  end;
end;

procedure ANT_DestroyWindow(win: pANTwindow);
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

function ANT_MakeCurrent(var win: pANTwindow): boolean;
begin
  exit(gdi_MakeCurrent(win));
end;

procedure ANT_SwapBuffers(win: pANTwindow);
begin
{$IFDEF MSWINDOWS}
  gdi_SwapBuffers(win);
{$ENDIF}
{$IFDEF LINUX}
  X11_SwapBuffers(win);
{$ENDIF}
end;

procedure ANT_GetFrameBufferSize(win: pANTwindow; out width, height: integer);
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

procedure ANT_PollEvents;
var
  event: pANT_MessageRec;
  win: pANTwindow;
begin
{$IFDEF MSWINDOWS}
  gdi_PollEvents;
{$ENDIF}
{$IFDEF LINUX}
  X11_PollEvents;
{$ENDIF}

  event := antReadFirstMessage;

  if event <> nil then
    begin
      win := event^.win;
      if win > nil then
        if win^.event_callback <> nil then
          win^.event_callback(event);

      antDeleteMessage(event);
    end;
end;

procedure ANT_SetCursor(cursor: byte);
begin
  {$note implement this}
end;

procedure ANT_GetDisplayCoords(var dr: ANTRect);
begin
{$IFDEF MSWINDOWS}
  gdi_GetDisplayCoords(dr);
{$ENDIF}
{$IFDEF LINUX}
  x11_GetDisplayCoords(dr);
{$ENDIF}
end;

end.
