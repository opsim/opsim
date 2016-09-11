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
    next: pWindow;           //< Next item in the window list
    prev: pWindow;           //< Previous item in the window list
    fscreen: boolean;        //< Flag that indictes if the window is fullscreen or not

{$IFDEF MSWINDOWS}           
    wc: TWndclass;           //< GDI window class
    h_Wnd: Windows.HWND;     //< GDI window handle
    h_DC: Windows.HDC;       //< GDI device context
    h_RC: HGLRC;             //< GDI rendering context
    h_Instance: HINST;       //< GDI application instance
{$ENDIF}                     
{$IFDEF LINUX}               
   dpy: pDisplay;            //< X11 display
   Xwin: TWindow;            //< X11 window
   ctx: GLXContext;          //< X11 GLX context
   wmDeleteMessage: TAtom;   //< X11 delete mesage callback
{$ENDIF}                     
  end;

implementation

end.
