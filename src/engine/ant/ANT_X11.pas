
unit ANT_X11;

{$mode objfpc}{$H-}

interface

uses 
X, Xlib, xutil, GLX,
LinkedList, ANT_types;

function X11_CreateWindow(width, height: integer; title: PChar): pWindow;
procedure X11_DestroyWindow(var win: pWindow);
procedure X11_SwapBuffers(win: pWindow);
procedure X11_GetFrameBufferSize(win: pWindow; out width, height: integer);

procedure X11_PollEvents;
//function X11_WindowFromHWND(hWnd: Windows.HWND): pWindow;
//function X11_GetKeyboardShiftState: TShiftState;

implementation

uses 
SysUtils,
ANT_main, ANT_base, ANT_messages;

(*
 * Create an RGB, double-buffered window.
 * Return the window and context handles.
 *)
function X11_CreateWindow(width, height: integer; title: PChar): pWindow;

const 
  WINDOW_POS_X = 0;
  WINDOW_POS_Y = 0;

var 
  attribs: array [0..10] of integer = (GLX_RGBA, GLX_RED_SIZE,
                                       1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DOUBLEBUFFER,
                                       GLX_DEPTH_SIZE, 1, None);
  win: pWindow = nil;
  scrnum: integer;
  attr: TXSetWindowAttributes;
  mask: cardinal;
  root: TWindow;
  visinfo: pXVisualInfo;
  sizehints: TXSizeHints;
begin
  win := callocN(sizeof(window));

  win^.dpy := XOpenDisplay(nil);
  if win^.dpy = nil then
    begin
      antError(ANT_PLATFORM_ERROR, 'could not open display %s', [title]);
      exit(nil);
    end;

  scrnum := DefaultScreen(win^.dpy);
  root := RootWindow(win^.dpy, scrnum);

  visinfo := glXChooseVisual(win^.dpy, scrnum, attribs);
  if visinfo = nil then
    begin
      antError(ANT_PLATFORM_ERROR, 'could not get an RGB, Double-buffered visual', []);
      exit(nil);
    end;

  //window attributes
  attr.background_pixel := 0;
  attr.border_pixel := 0;
  attr.colormap := XCreateColormap(win^.dpy, root, visinfo^.visual, AllocNone);
  attr.event_mask := StructureNotifyMask or ExposureMask or KeyPressMask or
                     KeyReleaseMask or ButtonPress or ButtonReleaseMask or Button1MotionMask or
                     PointerMotionMask;
  //XXX this is a bad way to get a borderless window!
  //   attr.override_redirect := fullscreen;
  attr.override_redirect := 0;
  mask := CWBackPixel or CWBorderPixel or CWColormap or CWEventMask or
          CWOverrideRedirect;

  win^.Xwin := XCreateWindow(win^.dpy, root, WINDOW_POS_X, WINDOW_POS_Y,
               width, height, 0, visinfo^.depth, InputOutput, visinfo^.visual, mask, @attr);

  win^.ctx := glXCreateContext(win^.dpy, visinfo, nil, True);

  //register delete!
  win^.wmDeleteMessage := XInternAtom(win^.dpy, 'WM_DELETE_WINDOW', False);
  XSetWMProtocols(win^.dpy, win^.Xwin, @win^.wmDeleteMessage, 1);
  // Register

  //set hints and properties
  sizehints.x := WINDOW_POS_X;
  sizehints.y := WINDOW_POS_Y;
  sizehints.width := width;
  sizehints.height := height;
  sizehints.flags := USSize or USPosition;
  XSetNormalHints(win^.dpy, win^.Xwin, @sizehints);
  XSetStandardProperties(win^.dpy, win^.Xwin, title, title,
                         None, nil, 0, @sizehints);

  if win^.ctx = nil then
    begin
      antError(ANT_PLATFORM_ERROR, 'glXCreateContext failed', []);
      exit(nil);
    end;

  XFree(visinfo);

  XMapWindow(win^.dpy, win^.Xwin);
  glXMakeCurrent(win^.dpy, win^.Xwin, win^.ctx);

  addtail(@windowlist, win);
  exit(win);
end;

procedure X11_DestroyWindow(var win: pWindow);
begin
  //destroy window and context
  if win^.fscreen then
    begin
    end;

  glXMakeCurrent(win^.dpy, None, nil);
  glXDestroyContext(win^.dpy, win^.ctx);
  XDestroyWindow(win^.dpy, win^.Xwin);
  XCloseDisplay(win^.dpy);
end;

procedure X11_SwapBuffers(win: pWindow);
begin
  glXSwapBuffers(win^.dpy, win^.Xwin);
end;

procedure X11_GetFrameBufferSize(win: pWindow; out width, height: integer);
var
  attribs: XWindowAttributes;
begin
  XGetWindowAttributes(win^.dpy, win^.Xwin, @attribs);

  width := attribs.width;
  height := attribs.height;
end;

procedure X11_PollEvents;
var
  buffer: string[10];
  event: TXEvent;
  win: pWindow;
  params: ANT_MessageParams;
  r: integer;
  done: integer = 0;
begin
  win := windowlist.first;

  while win <> nil do
  begin
  XAllowEvents(win^.dpy, AsyncBoth, CurrentTime);

  if XPending(win^.dpy) > 0 then
    begin
      XNextEvent(win^.dpy, @event);

      case event._type of 
        ClientMessage:
                       begin
                         if event.xclient.data.l[0] = win^.wmDeleteMessage then // quit!
                           done := 1;
                       end;
        Expose:
                begin
                  // Update event! Should do draw here.
                end;
        ConfigureNotify:
                         begin
                           params.rect.width := event.xconfigure.width;
                           params.rect.height := event.xconfigure.height;

                           antPostMessage(win, ANT_MESSAGE_RESIZE, params);
                         end;
        KeyPress,
        KeyRelease:
                    begin
                      r := XLookupString(@event.xkey, @buffer, sizeof(buffer), nil, nil);

                      params.keyboard.keychar := Ord(buffer[0]);
                      //params.keyboard.shiftstate := X11_GetKeyboardShiftState;

                      if event._type = KeyPress then
                        antPostMessage(win, ANT_MESSAGE_KEYPRESS, params);
                      if event._type = KeyRelease then
                        antPostMessage(win, ANT_MESSAGE_KEYRELEASE, params);
                    end;
        ButtonPress,
        ButtonRelease:
                       begin
                         params.mouse.x := event.xbutton.x;
                         params.mouse.y := event.xbutton.y;
                         //params.mouse.shiftstate := X11_GetKeyboardShiftState;
                         params.mouse.timestamp := Now;

                         case event.xbutton.button of 
                           Button1: params.mouse.Buttons := ANT_MOUSE_BUTTON_LEFT;
                           Button2: params.mouse.Buttons := ANT_MOUSE_BUTTON_MIDDLE;
                           Button3: params.mouse.Buttons := ANT_MOUSE_BUTTON_RIGHT;
                         end;

                         case event._type of 
                           ButtonPress: antPostMessage(win, ANT_MESSAGE_MOUSEDOWN, params);
                           ButtonRelease: antPostMessage(win, ANT_MESSAGE_MOUSEUP, params);
                         end;
                       end;
        MotionNotify:
                      begin
                      end;
      end;
    end;

    win := win^.next;
  end;
end;

//function X11_WindowFromHWND(hWnd: Windows.HWND): pWindow;
//var
//  wi: pWindow;
//begin
//  wi := windowlist.first;

//  while wi <> nil do
//    begin
//      if wi^.h_Wnd = hWnd then
//        exit(wi);

//      wi := wi^.next;
//    end;
//end;

//function X11_GetKeyboardShiftState: TShiftState;
//var
//  State: array[byte] of byte;
//begin
//  Windows.GetKeyboardState(State);

//  Result := [];
//  if (State[VK_SHIFT] and 128) <> 0 then
//    Include(Result, ssShift);
//  if (State[VK_MENU] and 128) <> 0 then
//    Include(Result, ssAlt);
//  if (State[VK_CONTROL] and 128) <> 0 then
//    Include(Result, ssCtrl);
//  if (State[VK_LBUTTON] and 128) <> 0 then
//    Include(Result, ssLeft);
//  if (State[VK_RBUTTON] and 128) <> 0 then
//    Include(Result, ssRight);
//  if (State[VK_MBUTTON] and 128) <> 0 then
//    Include(Result, ssMiddle);
//  if (State[VK_CAPITAL] and 1) <> 0 then
//    Include(Result, ssCaps);
//  if (State[VK_NUMLOCK] and 1) <> 0 then
//    Include(Result, ssNum);
//  if (State[VK_SCROLL] and 1) <> 0 then
//    Include(Result, ssScroll);
//end;

end.
