unit ANT_gdi;

{$mode objfpc}

interface

uses
  Classes, Windows,
  GL,
  MEM_guardedalloc,
  ANT_base, ANT_types;

function gdi_Init: boolean;
function gdi_Terminate: boolean;

{
   Creates a GDI window and its associated context.
   @param Width: the width of the window
   @param Height: the height of the window
   @param title: the title of the window
   @return a reference to the created window
}
function gdi_CreateWindow(win: pANTwindow; posx, posy, sizex, sizey: integer; title: PChar): boolean;
{
   Destroys a GDI window and its associated context.
   @param win: the reference to the window being destroyed
}
procedure gdi_DestroyWindow(var win: pANTwindow);

function gdi_MakeCurrent(win: pANTwindow): boolean;

{
   Swaps the front and back buffers of the specified window.
   @param win: the reference to the window
}
procedure gdi_SwapBuffers(win: pANTwindow);
{
   Retrieves the size of the framebuffer of the specified window.
   @param win: the reference to the window
   @param Width: the width of the window
   @param Height: the height of the window
}
procedure gdi_GetFrameBufferSize(win: pANTwindow; out width, height: integer);

{
   This is the procedure will poll for any pending events and put them in 
   the eventlist. Next the eventlist is checked and if needed the event 
   callback function is called.
}
procedure gdi_PollEvents;

procedure gdi_GetDisplayCoords(var dr: ANTRect);

implementation

uses
  SysUtils,
  ANT_main, ANT_messages;

var
  wc: TWndclass;
  h_Instance: HINST;

function gdi_WindowFromHWND(hWnd: Windows.HWND): pANTwindow; forward;
function gdi_GetKeyboardShiftState: TShiftState; forward;
function WndProc(hWnd: Windows.HWND; message: UINT; wParam: WPARAM; lParam: LPARAM): LRESULT; stdcall; forward;

function gdi_Init: boolean;
begin
  h_Instance := GetModuleHandle(nil);

  with wc do
    begin
      style := CS_HREDRAW or CS_VREDRAW or CS_OWNDC;
      lpfnWndProc := @WndProc;
      cbClsExtra := 0;
      cbWndExtra := 0;
      hInstance := h_Instance;
      hIcon := LoadIcon(0, IDI_WINLOGO);
      hCursor := LoadCursor(0, IDC_ARROW);
      hbrBackground := 0;
      lpszMenuName := nil;
      lpszClassName := 'ANT class';
    end;

  if RegisterClass(wc) = 0 then
    begin
      case GetLastError of
        ERROR_CLASS_ALREADY_EXISTS:
          antError(ANT_PLATFORM_ERROR, 'ANT error: class already exists', []);
        ERROR_CLASS_DOES_NOT_EXIST:
          antError(ANT_PLATFORM_ERROR, 'ANT error: class does not exists', []);
        ERROR_CLASS_HAS_WINDOWS:
          antError(ANT_PLATFORM_ERROR, 'ANT error: class has windows', []);
      else
        antError(ANT_PLATFORM_ERROR, 'ANT error: unknown class error', []);
      end;

      exit(False);
    end;

  exit(True);
end;

function gdi_Terminate: boolean;
begin
  if (not UnregisterClass('ANT class', h_Instance)) then
    begin
      antError(ANT_PLATFORM_ERROR, 'ANT error: could not unregister class', []);
      exit(False);
    end;

  exit(True);
end;

function gdi_CreateWindow(win: pANTwindow; posx, posy, sizex, sizey: integer; title: PChar): boolean;
const
  bits = 16;
var
  Pixelformat: GLuint;
  dwExStyle: dword;
  dwStyle: dword;
  pfd: pixelformatdescriptor;
  dmScreenSettings: Devmode;
  WindowRect: TRect;
begin
  WindowRect.Left := posx;
  WindowRect.Top := posy;
  WindowRect.Right := sizex;
  WindowRect.Bottom := sizey;

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
          antError(ANT_PLATFORM_ERROR, 'ANT error: fullscreen mode is not supported, switching to windowed mode instead', []);
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
                'ANT class',
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
      antError(ANT_PLATFORM_ERROR, 'ANT error: window creation error', []);
      exit(False);
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
      antError(ANT_PLATFORM_ERROR, 'ANT error: can''t create a GL device context', []);
      exit(False);
    end;

  PixelFormat := ChoosePixelFormat(win^.h_DC, @pfd);
  if PixelFormat = 0 then
    begin
      gdi_DestroyWindow(win);
      antError(ANT_PLATFORM_ERROR, 'ANT error: can''t find a suitable PixelFormat', []);
      exit(False);
    end;

  if not SetPixelFormat(win^.h_DC, PixelFormat, @pfd) then
    begin
      gdi_DestroyWindow(win);
      antError(ANT_PLATFORM_ERROR, 'ANT error: can''t set PixelFormat', []);
      exit(False);
    end;

  win^.h_RC := wglCreateContext(win^.h_DC);
  if win^.h_RC = 0 then
    begin
      gdi_DestroyWindow(win);
      antError(ANT_PLATFORM_ERROR, 'ANT error: can''t create a GL rendering context', []);
      exit(False);
    end;

  if not wglMakeCurrent(win^.h_DC, win^.h_RC) then
    begin
      gdi_DestroyWindow(win);
      antError(ANT_PLATFORM_ERROR, 'ANT error: can''t activate the GL rendering context', []);
      exit(False);
    end;

  ShowWindow(win^.h_Wnd, SW_SHOW);
  SetForegroundWindow(win^.h_Wnd);
  SetFocus(win^.h_Wnd);

  addtail(@windowlist, win);
  exit(True);
end;

procedure gdi_DestroyWindow(var win: pANTwindow);
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
        antError(ANT_PLATFORM_ERROR, 'ANT error: release of DC and RC failed', []);

      if (not wglDeleteContext(win^.h_RC)) then
        begin
          antError(ANT_PLATFORM_ERROR, 'ANT error: release of Rendering Context failed', []);
          win^.h_RC := 0;
        end;
    end;

  if (win^.h_DC = 1) and (releaseDC(win^.h_Wnd, win^.h_DC) <> 0) then
    begin
      antError(ANT_PLATFORM_ERROR, 'ANT error: release of Device Context failed', []);
      win^.h_DC := 0;
    end;

  if (win^.h_Wnd <> 0) and (not destroywindow(win^.h_Wnd)) then
    begin
      antError(ANT_PLATFORM_ERROR, 'ANT error: could not release hWnd', []);
      win^.h_Wnd := 0;
    end;
end;

function gdi_MakeCurrent(win: pANTwindow): boolean;
begin
  exit(wglMakeCurrent(win^.h_DC, win^.h_RC));
end;

procedure gdi_SwapBuffers(win: pANTwindow);
begin
  SwapBuffers(win^.h_DC);
end;

procedure gdi_GetFrameBufferSize(win: pANTwindow; out width, height: integer);
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

procedure gdi_GetDisplayCoords(var dr: ANTRect);
begin
  if not SystemParametersInfo(SPI_GETWORKAREA, 0, @dr, 0) then
  begin
    dr.right := 0;
    dr.left := 0;
    dr.bottom := 0;
    dr.top := 0;
  end;
end;

function gdi_WindowFromHWND(hWnd: Windows.HWND): pANTwindow;
var
  wi: pANTwindow;
begin
  wi := windowlist.first;

  while wi <> nil do
    begin
      if wi^.h_Wnd = hWnd then
        exit(wi);

      wi := wi^.next;
    end;

  exit(nil);
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
  win: pANTwindow;
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
               params.rect.top := 0;
               params.rect.left := 0;
               params.rect.width  := smallint(lParam and $FFFF);
               params.rect.height := smallint((lParam and $FFFF0000) shr 16);

               antPostMessage(win, ANT_MESSAGE_RESIZE, params);
             end;
    //WM_PAINT:
    //          antPostMessage(win, ANT_MESSAGE_PAINT, params);
    else
      begin
        //writeln(message);

        if message = WM_PAINT then
          antPostMessage(win, ANT_MESSAGE_PAINT, params);

        exit(DefWindowProc(hWnd, message, wParam, lParam));
      end;
  end;

  exit(0);
end;

end.

