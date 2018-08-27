unit ANT_types;

interface

uses
{$IFDEF MSWINDOWS}
  Windows;
{$ENDIF}
{$IFDEF LINUX}
  X, Xlib, GLX;
{$ENDIF}

const
    { Key codes  }
     ANT_KEY_LBUTTON = 1;
     ANT_KEY_RBUTTON = 2;
     ANT_KEY_CANCEL = 3;
     ANT_KEY_MBUTTON = 4;
     ANT_KEY_XBUTTON1 = 5;
     ANT_KEY_XBUTTON2 = 6;
     ANT_KEY_BACK = 8;
     ANT_KEY_TAB = 9;
     ANT_KEY_CLEAR = 12;
     ANT_KEY_RETURN = 13;
     ANT_KEY_SHIFT = 16;
     ANT_KEY_CONTROL = 17;
     ANT_KEY_MENU = 18;
     ANT_KEY_PAUSE = 19;
     ANT_KEY_CAPITAL = 20;
     ANT_KEY_KANA = 21;
     ANT_KEY_HANGEUL = 21;
     ANT_KEY_HANGUL = 21;
     ANT_KEY_JUNJA = 23;
     ANT_KEY_FINAL = 24;
     ANT_KEY_HANJA = 25;
     ANT_KEY_KANJI = 25;
     ANT_KEY_ESCAPE = 27;
     ANT_KEY_CONVERT = 28;
     ANT_KEY_NONCONVERT = 29;
     ANT_KEY_ACCEPT = 30;
     ANT_KEY_MODECHANGE = 31;
     ANT_KEY_SPACE = 32;
     ANT_KEY_PRIOR = 33;
     ANT_KEY_NEXT = 34;
     ANT_KEY_END = 35;
     ANT_KEY_HOME = 36;
     ANT_KEY_LEFT = 37;
     ANT_KEY_UP = 38;
     ANT_KEY_RIGHT = 39;
     ANT_KEY_DOWN = 40;
     ANT_KEY_SELECT = 41;
     ANT_KEY_PRINT = 42;
     ANT_KEY_EXECUTE = 43;
     ANT_KEY_SNAPSHOT = 44;
     ANT_KEY_INSERT = 45;
     ANT_KEY_DELETE = 46;
     ANT_KEY_HELP = 47;
     ANT_KEY_0 = 48;
     ANT_KEY_1 = 49;
     ANT_KEY_2 = 50;
     ANT_KEY_3 = 51;
     ANT_KEY_4 = 52;
     ANT_KEY_5 = 53;
     ANT_KEY_6 = 54;
     ANT_KEY_7 = 55;
     ANT_KEY_8 = 56;
     ANT_KEY_9 = 57;
     ANT_KEY_A = 65;
     ANT_KEY_B = 66;
     ANT_KEY_C = 67;
     ANT_KEY_D = 68;
     ANT_KEY_E = 69;
     ANT_KEY_F = 70;
     ANT_KEY_G = 71;
     ANT_KEY_H = 72;
     ANT_KEY_I = 73;
     ANT_KEY_J = 74;
     ANT_KEY_K = 75;
     ANT_KEY_L = 76;
     ANT_KEY_M = 77;
     ANT_KEY_N = 78;
     ANT_KEY_O = 79;
     ANT_KEY_P = 80;
     ANT_KEY_Q = 81;
     ANT_KEY_R = 82;
     ANT_KEY_S = 83;
     ANT_KEY_T = 84;
     ANT_KEY_U = 85;
     ANT_KEY_V = 86;
     ANT_KEY_W = 87;
     ANT_KEY_X = 88;
     ANT_KEY_Y = 89;
     ANT_KEY_Z = 90;
     ANT_KEY_LWIN = 91;
     ANT_KEY_RWIN = 92;
     ANT_KEY_APPS = 93;
     ANT_KEY_SLEEP = 95;
     ANT_KEY_NUMPAD0 = 96;
     ANT_KEY_NUMPAD1 = 97;
     ANT_KEY_NUMPAD2 = 98;
     ANT_KEY_NUMPAD3 = 99;
     ANT_KEY_NUMPAD4 = 100;
     ANT_KEY_NUMPAD5 = 101;
     ANT_KEY_NUMPAD6 = 102;
     ANT_KEY_NUMPAD7 = 103;
     ANT_KEY_NUMPAD8 = 104;
     ANT_KEY_NUMPAD9 = 105;
     ANT_KEY_MULTIPLY = 106;
     ANT_KEY_ADD = 107;
     ANT_KEY_SEPARATOR = 108;
     ANT_KEY_SUBTRACT = 109;
     ANT_KEY_DECIMAL = 110;
     ANT_KEY_DIVIDE = 111;
     ANT_KEY_F1 = 112;
     ANT_KEY_F2 = 113;
     ANT_KEY_F3 = 114;
     ANT_KEY_F4 = 115;
     ANT_KEY_F5 = 116;
     ANT_KEY_F6 = 117;
     ANT_KEY_F7 = 118;
     ANT_KEY_F8 = 119;
     ANT_KEY_F9 = 120;
     ANT_KEY_F10 = 121;
     ANT_KEY_F11 = 122;
     ANT_KEY_F12 = 123;
     ANT_KEY_F13 = 124;
     ANT_KEY_F14 = 125;
     ANT_KEY_F15 = 126;
     ANT_KEY_F16 = 127;
     ANT_KEY_F17 = 128;
     ANT_KEY_F18 = 129;
     ANT_KEY_F19 = 130;
     ANT_KEY_F20 = 131;
     ANT_KEY_F21 = 132;
     ANT_KEY_F22 = 133;
     ANT_KEY_F23 = 134;
     ANT_KEY_F24 = 135;

     ANT_KEY_NUMLOCK = 144;
     ANT_KEY_SCROLL = 145;
     ANT_KEY_OEM_NEC_EQUAL = 146;
     ANT_KEY_OEM_FJ_JISHO = 146;
     ANT_KEY_OEM_FJ_MASSHOU = 147;
     ANT_KEY_OEM_FJ_TOUROKU = 148;
     ANT_KEY_OEM_FJ_LOYA = 149;
     ANT_KEY_OEM_FJ_ROYA = 150;
     ANT_KEY_LSHIFT = 160;
     ANT_KEY_LCONTROL = 162;
     ANT_KEY_LMENU = 164;
     ANT_KEY_RSHIFT = 161;
     ANT_KEY_RCONTROL = 163;
     ANT_KEY_RMENU = 165;
     ANT_KEY_BROWSER_BACK = 166;
     ANT_KEY_BROWSER_FORWARD = 167;
     ANT_KEY_BROWSER_REFRESH = 168;
     ANT_KEY_BROWSER_STOP = 169;
     ANT_KEY_BROWSER_SEARCH = 170;
     ANT_KEY_BROWSER_FAVORITES = 171;
     ANT_KEY_BROWSER_HOME = 172;
     ANT_KEY_VOLUME_MUTE = 173;
     ANT_KEY_VOLUME_DOWN = 174;
     ANT_KEY_VOLUME_UP = 175;
     ANT_KEY_MEDIA_NEXT_TRACK = 176;
     ANT_KEY_MEDIA_PREV_TRACK = 177;
     ANT_KEY_MEDIA_STOP = 178;
     ANT_KEY_MEDIA_PLAY_PAUSE = 179;
     ANT_KEY_LAUNCH_MAIL = 180;
     ANT_KEY_LAUNCH_MEDIA_SELECT = 181;
     ANT_KEY_LAUNCH_APP1 = 182;
     ANT_KEY_LAUNCH_APP2 = 183;
     ANT_KEY_OEM_1 = 186;
     ANT_KEY_OEM_PLUS = 187;
     ANT_KEY_OEM_COMMA = 188;
     ANT_KEY_OEM_MINUS = 189;
     ANT_KEY_OEM_PERIOD = 190;
     ANT_KEY_OEM_2 = 191;
     ANT_KEY_OEM_3 = 192;
     ANT_KEY_OEM_4 = 219;
     ANT_KEY_OEM_5 = 220;
     ANT_KEY_OEM_6 = 221;
     ANT_KEY_OEM_7 = 222;
     ANT_KEY_OEM_8 = 223;
     ANT_KEY_OEM_AX = 225;
     ANT_KEY_OEM_102 = 226;
     ANT_KEY_ICO_HELP = 227;
     ANT_KEY_ICO_00 = 228;

     ANT_KEY_PROCESSKEY = 229;
     ANT_KEY_ICO_CLEAR = 230;
     ANT_KEY_PACKET = 231;
     ANT_KEY_OEM_RESET = 233;
     ANT_KEY_OEM_JUMP = 234;
     ANT_KEY_OEM_PA1 = 235;
     ANT_KEY_OEM_PA2 = 236;
     ANT_KEY_OEM_PA3 = 237;
     ANT_KEY_OEM_WSCTRL = 238;
     ANT_KEY_OEM_CUSEL = 239;
     ANT_KEY_OEM_ATTN = 240;
     ANT_KEY_OEM_FINISH = 241;
     ANT_KEY_OEM_COPY = 242;
     ANT_KEY_OEM_AUTO = 243;
     ANT_KEY_OEM_ENLW = 244;
     ANT_KEY_OEM_BACKTAB = 245;
     ANT_KEY_ATTN = 246;
     ANT_KEY_CRSEL = 247;
     ANT_KEY_EXSEL = 248;
     ANT_KEY_EREOF = 249;
     ANT_KEY_PLAY = 250;
     ANT_KEY_ZOOM = 251;
     ANT_KEY_NONAME = 252;
     ANT_KEY_PA1 = 253;
     ANT_KEY_OEM_CLEAR = 254;

type
  ANT_EventCallback = procedure(msg: pointer);
  ANT_ErrorCallback = procedure(error: integer; const description: string);

  ANTRect = record
    left: longint;
    top: longint;
    right: longint;
    bottom: longint;
  end;

  pANTwindow = ^ANTwindow;

  ANTWindow = record
    next: pANTwindow;        //< Next item in the window list
    prev: pANTwindow;        //< Previous item in the window list

    //Window settings and state

    fscreen: boolean;        //< Flag that indictes if the window is fullscreen or not
    shouldClose: boolean;    //< Flag indicating if the window should close

    //callback functions
    event_callback: ANT_EventCallback;   //< event callback function for this window

{$IFDEF MSWINDOWS}           
    h_Wnd: Windows.HWND;     //< GDI window handle
    h_DC: Windows.HDC;       //< GDI device context
    h_RC: HGLRC;             //< GDI rendering context
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
