unit ANT_types;

{$mode objfpc}

interface

uses
{$IFDEF MSWINDOWS}
  Windows;
{$ENDIF}
{$IFDEF LINUX}
  X, Xlib, GLX;
{$ENDIF}

type
  ANT_EventCallback = procedure(msg: pointer);
  ANT_ErrorCallback = procedure(error: integer; const description: string);

  pWindow = ^Window;

  Window = record
    next, prev: pWindow;
    fscreen: boolean;

{$IFDEF MSWINDOWS}
    wc: TWndclass;
    h_Wnd: Windows.HWND;
    h_DC: Windows.HDC;
    h_RC: HGLRC;
    h_Instance: HINST;
{$ENDIF}
{$IFDEF LINUX}
   dpy: pDisplay;
   Xwin: TWindow;
   ctx: GLXContext;
   wmDeleteMessage: TAtom;
{$ENDIF}
  end;

implementation

end.
