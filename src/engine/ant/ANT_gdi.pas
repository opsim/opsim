unit ANT_gdi;

{$mode objfpc}{$H-}

interface

uses
  Classes, Windows,
  GL,
  LinkedList,
  ANT_base, ANT_types;

{
   Creates a GDI window and its associated context.
   @param Width: the width of the window
   @param Height: the height of the window
   @param title: the title of the window
   @return a reference to the created window
}
function gdi_CreateWindow(Width, Height: integer; title: PChar): pWindow;
{
   Destroys a GDI window and its associated context.
   @param win: the reference to the window being destroyed
}
procedure gdi_DestroyWindow(var win: pWindow);
{
   Swaps the front and back buffers of the specified window.
   @param win: the reference to the window
}
procedure gdi_SwapBuffers(win: pWindow);
{
   Retrieves the size of the framebuffer of the specified window.
   @param win: the reference to the window
   @param Width: the width of the window
   @param Height: the height of the window
}
procedure gdi_GetFrameBufferSize(win: pWindow; out width, height: integer);

{
   This is the procedure will poll for any pending events and put them in 
   the eventlist. Next the eventlist is checked and if needed the event 
   callback function is called.
}
procedure gdi_PollEvents;

implementation

uses
  SysUtils,
  ANT_main, ANT_messages;

function gdi_WindowFromHWND(hWnd: Windows.HWND): pWindow; forward;
function gdi_GetKeyboardShiftState: TShiftState; forward;
function WndProc(hWnd: Windows.HWND; message: UINT; wParam: WPARAM; lParam: LPARAM): LRESULT; stdcall; forward;

function gdi_CreateWindow(Width, Height: integer; title: PChar): pWindow;
const
  bits = 16;
var
  Pixelformat: GLuint;
  wc: TWndclass;
  dwExStyle: dword;
  dwStyle: dword;
  pfd: pixelformatdescriptor;
  dmScreenSettings: Devmode;
  WindowRect: TRect;
  win: pWindow = nil;
begin
  win := callocN(sizeof(window));

  WindowRect.Left := 0;
  WindowRect.Top := 0;
  WindowRect.Right := Width;
  WindowRect.Bottom := Height;

  win^.h_Instance := GetModuleHandle(nil);

  with wc do
    begin
      style := CS_HREDRAW or CS_VREDRAW or CS_OWNDC;
      lpfnWndProc := @WndProc;
      cbClsExtra := 0;
      cbWndExtra := 0;
      hInstance := win^.h_Instance;
      hIcon := LoadIcon(0, IDI_WINLOGO);
      hCursor := LoadCursor(0, IDC_ARROW);
      hbrBackground := 0;
      lpszMenuName := nil;
      lpszClassName := 'ANT';
    end;

  if RegisterClass(wc) = 0 then
    begin
      antError(ANT_PLATFORM_ERROR, 'failed to register the window class', []);
      exit(nil);
    end;

  if win^.fscreen then
    begin
      ZeroMemory(@dmScreenSettings, sizeof(dmScreenSettings));
      with dmScreensettings do
        begin
          dmSize := sizeof(dmScreenSettings);
          dmPelsWidth := gWindowWidth;
          dmPelsHeight := gWindowHeight;
          dmBitsPerPel := bits;
          dmFields := DM_BITSPERPEL or DM_PELSWIDTH or DM_PELSHEIGHT;
        end;

      if (ChangeDisplaySettings(dmScreenSettings, CDS_FULLSCREEN)) <> DISP_CHANGE_SUCCESSFUL then
        begin
          antError(ANT_PLATFORM_ERROR, 'fullscreen mode is not supported, switching to windowed mode instead', []);
          win^.fscreen := False;
        end;
    end;

  if win^.fscreen then
    begin
      dwExStyle := WS_EX_APPWINDOW;
      dwStyle := WS_POPUP or WS_CLIPSIBLINGS or WS_CLIPCHILDREN;
      Showcursor(False);
    end
  else
    begin
      dwExStyle := WS_EX_APPWINDOW or WS_EX_WINDOWEDGE;
      dwStyle := WS_OVERLAPPEDWINDOW or WS_CLIPSIBLINGS or WS_CLIPCHILDREN;
    end;

  AdjustWindowRectEx(WindowRect, dwStyle, False, dwExStyle);

  win^.h_Wnd := CreateWindowEx(dwExStyle,
                'ANT',
                Title,
                dwStyle,
                0, 0,
                WindowRect.Right - WindowRect.Left,
                WindowRect.Bottom - WindowRect.Top,
                0,
                0,
                hinstance,
                nil);
  if win^.h_Wnd = 0 then
    begin
      gdi_DestroyWindow(win);
      antError(ANT_PLATFORM_ERROR, 'window creation error', []);
      exit(nil);
    end;

  with pfd do
    begin
      nSize := SizeOf(PIXELFORMATDESCRIPTOR);
      nVersion := 1;
      dwFlags := PFD_DRAW_TO_WINDOW or PFD_SUPPORT_OPENGL or PFD_DOUBLEBUFFER;
      iPixelType := PFD_TYPE_RGBA;
      cColorBits := bits;
      cRedBits := 0;
      cRedShift := 0;
      cGreenBits := 0;
      cBlueBits := 0;
      cBlueShift := 0;
      cAlphaBits := 0;
      cAlphaShift := 0;
      cAccumBits := 0;
      cAccumRedBits := 0;
      cAccumGreenBits := 0;
      cAccumBlueBits := 0;
      cAccumAlphaBits := 0;
      cDepthBits := 16;
      cStencilBits := 0;
      cAuxBuffers := 0;
      iLayerType := PFD_MAIN_PLANE;
      bReserved := 0;
      dwLayerMask := 0;
      dwVisibleMask := 0;
      dwDamageMask := 0;
    end;

  win^.h_DC := GetDC(win^.h_Wnd);
  if win^.h_DC = 0 then
    begin
      gdi_DestroyWindow(win);
      antError(ANT_PLATFORM_ERROR, 'can''t create a GL device context', []);
      exit(nil);
    end;

  PixelFormat := ChoosePixelFormat(win^.h_DC, @pfd);
  if PixelFormat = 0 then
    begin
      gdi_DestroyWindow(win);
      antError(ANT_PLATFORM_ERROR, 'can''t find a suitable PixelFormat', []);
      exit(nil);
    end;

  if not SetPixelFormat(win^.h_DC, PixelFormat, @pfd) then
    begin
      gdi_DestroyWindow(win);
      antError(ANT_PLATFORM_ERROR, 'can''t set PixelFormat', []);
      exit(nil);
    end;

  win^.h_RC := wglCreateContext(win^.h_DC);
  if win^.h_RC = 0 then
    begin
      gdi_DestroyWindow(win);
      antError(ANT_PLATFORM_ERROR, 'can''t create a GL rendering context', []);
      exit(nil);
    end;

  if not wglMakeCurrent(win^.h_DC, win^.h_RC) then
    begin
      gdi_DestroyWindow(win);
      antError(ANT_PLATFORM_ERROR, 'can''t activate the GL rendering context', []);
      exit(nil);
    end;

  ShowWindow(win^.h_Wnd, SW_SHOW);
  SetForegroundWindow(win^.h_Wnd);
  SetFocus(win^.h_Wnd);

  addtail(@windowlist, win);
  exit(win);
end;

procedure gdi_DestroyWindow(var win: pWindow);
begin
  //destroy window and context
  if win^.fscreen then
    begin
      ChangeDisplaySettings(devmode(nil^),0);
      showcursor(true);
    end;

  if win^.h_RC <> 0 then
    begin
      if (not wglMakeCurrent(win^.h_DC,0)) then
        antError(ANT_PLATFORM_ERROR, 'release of DC and RC failed', []);

      if (not wglDeleteContext(win^.h_RC)) then
        begin
          antError(ANT_PLATFORM_ERROR, 'release of Rendering Context failed', []);
          win^.h_RC := 0;
        end;
    end;

  if (win^.h_DC = 1) and (releaseDC(win^.h_Wnd, win^.h_DC) <> 0) then
    begin
      antError(ANT_PLATFORM_ERROR, 'release of Device Context failed', []);
      win^.h_DC := 0;
    end;

  if (win^.h_Wnd <> 0) and (not destroywindow(win^.h_Wnd)) then
    begin
      antError(ANT_PLATFORM_ERROR, 'could not release hWnd', []);
      win^.h_Wnd := 0;
    end;

  if (not UnregisterClass('ANT', win^.h_Instance)) then
    begin
      antError(ANT_PLATFORM_ERROR, 'could not unregister class', []);
    end;
end;

procedure gdi_SwapBuffers(win: pWindow);
begin
  SwapBuffers(win^.h_DC);
end;

procedure gdi_GetFrameBufferSize(win: pWindow; out width, height: integer);
var
  area: RECT;
begin
  GetClientRect(win^.h_Wnd, @area);

  width := area.right;
  height := area.bottom;
end;

procedure gdi_PollEvents;
var
  msg: TMSG;
begin
  // check for messages
  while PeekMessage(msg, 0, 0, 0, PM_REMOVE) do
    begin
      TranslateMessage(@msg);
      DispatchMessage(@msg);
    end;
end;

function gdi_WindowFromHWND(hWnd: Windows.HWND): pWindow;
var
  wi: pWindow;
begin
  wi := windowlist.first;

  while wi <> nil do
    begin
      if wi^.h_Wnd = hWnd then
        exit(wi);

      wi := wi^.next;
    end;
end;

function gdi_GetKeyboardShiftState: TShiftState;
var
  State: array[byte] of byte;
begin
  Windows.GetKeyboardState(State);

  Result := [];
  if (State[VK_SHIFT] and 128) <> 0 then
    Include(Result, ssShift);
  if (State[VK_MENU] and 128) <> 0 then
    Include(Result, ssAlt);
  if (State[VK_CONTROL] and 128) <> 0 then
    Include(Result, ssCtrl);
  if (State[VK_LBUTTON] and 128) <> 0 then
    Include(Result, ssLeft);
  if (State[VK_RBUTTON] and 128) <> 0 then
    Include(Result, ssRight);
  if (State[VK_MBUTTON] and 128) <> 0 then
    Include(Result, ssMiddle);
  if (State[VK_CAPITAL] and 1) <> 0 then
    Include(Result, ssCaps);
  if (State[VK_NUMLOCK] and 1) <> 0 then
    Include(Result, ssNum);
  if (State[VK_SCROLL] and 1) <> 0 then
    Include(Result, ssScroll);
end;

function WndProc(hWnd: Windows.HWND; message: UINT; wParam: WPARAM; lParam: LPARAM): LRESULT; stdcall;
var
  x, y: integer;
  win: pWindow;
  params: ANT_MessageParams;
begin
  x := LOWORD(lParam);
  y := HIWORD(lParam);

  win := gdi_WindowFromHWND(hWnd);

  case message of

    WM_LBUTTONUP,
    WM_LBUTTONDOWN,
    WM_MBUTTONUP,
    WM_MBUTTONDOWN,
    WM_RBUTTONUP,
    WM_RBUTTONDOWN:
                    begin
                      params.mouse.x := x;
                      params.mouse.y := y;
                      params.mouse.shiftstate := gdi_GetKeyboardShiftState;
                      params.mouse.timestamp := Now;

                      case message of
                        WM_LBUTTONUP,
                        WM_LBUTTONDOWN:
                                        params.mouse.buttons := ANT_MOUSE_BUTTON_LEFT;
                        WM_MBUTTONUP,
                        WM_MBUTTONDOWN:
                                        params.mouse.buttons := ANT_MOUSE_BUTTON_MIDDLE;
                        WM_RBUTTONUP,
                        WM_RBUTTONDOWN:
                                        params.mouse.buttons := ANT_MOUSE_BUTTON_RIGHT;
                      end;

                      case message of
                        WM_LBUTTONUP,
                        WM_MBUTTONUP,
                        WM_RBUTTONUP:
                                      antPostMessage(win, ANT_MESSAGE_MOUSEUP, params);
                        WM_LBUTTONDOWN,
                        WM_MBUTTONDOWN,
                        WM_RBUTTONDOWN:
                                        antPostMessage(win, ANT_MESSAGE_MOUSEDOWN, params);
                      end;
                    end;

    WM_MOUSEMOVE:
                  begin
                    params.mouse.x := x;
                    params.mouse.y := y;
                    params.mouse.shiftstate := gdi_GetKeyboardShiftState;
                    params.mouse.timestamp := Now;

                    antPostMessage(win, ANT_MESSAGE_MOUSEMOVE, params);
                  end;

    WM_CREATE:
    ;

    WM_CLOSE:
              PostQuitMessage(0);

    WM_DESTROY:
    ;

    WM_KEYDOWN,
    WM_KEYUP:
              begin
                params.keyboard.keychar := wParam;
                params.keyboard.shiftstate := gdi_GetKeyboardShiftState;

                if message = WM_KEYDOWN then
                  antPostMessage(win, ANT_MESSAGE_KEYPRESS, params);
                if message = WM_KEYUP then
                  antPostMessage(win, ANT_MESSAGE_KEYRELEASE, params);
              end;

    WM_SIZE:
             begin
               params.rect.Width  := smallint(lParam and $FFFF);
               params.rect.Height := smallint((lParam and $FFFF0000) shr 16);

               antPostMessage(win, ANT_MESSAGE_RESIZE, params);
             end;

    //WM_PAINT:
    //begin
    //  // Don't have Windows fighting us while resize!
    //  gDisplay;
    //end;

    WM_ERASEBKGND:
                   exit(1);

    else
      begin
        //writeln(message);

        exit(DefWindowProc(hWnd, message, wParam, lParam));
      end;
  end;

  exit(0);
end;

end.

