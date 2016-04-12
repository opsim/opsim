{
Ver:
  http://forum.lazarus.freepascal.org/index.php/topic,21961.msg129100.html#msg129100
}
//*************************************************************************
// * GLFW 3.0 - www.glfw.org
// * A library for OpenGL, window and input
// *------------------------------------------------------------------------
// * Copyright (c) 2002-2006 Marcus Geelnard
// * Copyright (c) 2006-2010 Camilla Berglund <elmindreda@elmindreda.org>
// *
// * This software is provided 'as-is', without any express or implied
// * warranty. In no event will the authors be held liable for any damages
// * arising from the use of this software.
// *
// * Permission is granted to anyone to use this software for any purpose,
// * including commercial applications, and to alter it and redistribute it
// * freely, subject to the following restrictions:
// *
// * 1. The origin of this software must not be misrepresented; you must not
// *    claim that you wrote the original software. If you use this software
// *    in a product, an acknowledgment in the product documentation would
// *    be appreciated but is not required.
// *
// * 2. Altered source versions must be plainly marked as such, and must not
// *    be misrepresented as being the original software.
// *
// * 3. This notice may not be removed or altered from any source
// *    distribution.
// *
// *************************************************************************

//*************************************************************************
// * Pascal header translate by: Jorge Turiel (Aka BlueIcaro)
// * with the help of the lazarus community (http://www.lazarus.freepascal.org/index.php)
// * Date: 20130912

unit glfw3;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, GL;

type
  GLFW_INT = integer;
  GLFWwindow = integer;
  pGLFWwindow = ^GLFWwindow;
  GLFWmonitor = integer;
  pGLFWmonitor = ^GLFWmonitor;
  pFloat = ^longint;
//Estructuras
//========================================================================
//Video Mode
//========================================================================


 Type
  GLFWvidmode = record
    Width: integer;
    Height: integer;
    redBits: integer;
    greenBits: integer;
    blueBits: integer;
    refreshRate: integer;
  end;

 pGLFWvidmode = ^GLFWvidmode;


//========================================================================
//Gamma
//========================================================================
type
  PGLFWgammaramp = ^GLFWgammaramp;

  GLFWgammaramp = record
    red: smallint;
    green: smallint;
    blue: smallint;
    size: dword;
  end;

type
  CharArray = array of char;
  pCharArray = CharArray;

//========================================================================
//Callbacks  must be cdecl
//========================================================================
type
  pGLFWKeyFun = ^GLFWKeyFun;
  GLFWKeyFun = procedure(p: pGLFWWindow; i2, i3, i4, i5: longint); cdecl;

  pGLFWerrorfun = ^GLFWerrorfun;
  GLFWerrorfun = procedure(Error: GLFW_INT; Description: PChar); cdecl;

  pGLFWmonitorfun = ^GLFWmonitorfun;
  GLFWmonitorfun = procedure(Monitor: GLFWmonitor); cdecl;

  pGLFWwindowposfun = ^GLFWwindowposfun;
  GLFWwindowposfun = procedure(window: pGLFWwindow; PosX, PosY: integer); cdecl;

  pGLFWwindowsizefun = ^GLFWwindowsizefun;
  GLFWwindowsizefun = procedure(window: pGLFWwindow; Width, Height: integer); cdecl;

  pGLFWwindowclosefun = ^GLFWwindowclosefun;
  GLFWwindowclosefun = procedure(window: pGLFWwindow); cdecl;

  pGLFWwindowrefreshfun = ^GLFWwindowrefreshfun;
  GLFWwindowrefreshfun = procedure(window: pGLFWwindow); cdecl;

  pGLFWwindowfocusfun = ^GLFWwindowfocusfun;
  GLFWwindowfocusfun = procedure(window: pGLFWwindow); cdecl;

  pGLFWwindowiconifyfun = ^GLFWwindowiconifyfun;
  GLFWwindowiconifyfun = procedure(window: pGLFWwindow); cdecl;

  pGLFWframebuffersizefun = ^GLFWframebuffersizefun;
  GLFWframebuffersizefun = procedure(window: pGLFWwindow;
    witdth, Height: integer); cdecl;

  pGLFWcharfun = ^GLFWcharfun;
  GLFWcharfun = procedure(window: pGLFWwindow); cdecl;

  pGLFWmousebuttonfun = ^GLFWmousebuttonfun;
  GLFWmousebuttonfun = procedure(window: pGLFWwindow;
    button, action, mods: integer); cdecl;

  pGLFWcursorposfun = ^GLFWcursorposfun;
  GLFWcursorposfun = procedure(window: pGLFWwindow; xpos, ypos: integer); cdecl;

  pGLFWcursorenterfun = ^GLFWcursorenterfun;
  GLFWcursorenterfun = procedure(window: pGLFWwindow); cdecl;

  pGLFWscrollfun = ^GLFWscrollfun;
  GLFWscrollfun = procedure(window: pGLFWwindow; xoffset, yoffset: integer); cdecl;

  pGLFWglproc = ^GLFWglproc;
  GLFWglproc = procedure;cdecl;

const
  GLFW_DLL = 'GLFW3.DLL';


  //========================================================================
  // GLFW version
  //========================================================================

  GLFW_VERSION_MAJOR = 3;

  GLFW_VERSION_MINOR = 0;

  GLFW_VERSION_REVISION = 3;



  GLFW_RELEASE = 0;

  GLFW_PRESS = 1;

  GLFW_REPEAT = 2;



  GLFW_KEY_UNKNOWN = -(1);

  GLFW_KEY_SPACE = 32;
  GLFW_KEY_APOSTROPHE = 39;
  GLFW_KEY_COMMA = 44;
  GLFW_KEY_MINUS = 45;
  GLFW_KEY_PERIOD = 46;
  GLFW_KEY_SLASH = 47;
  GLFW_KEY_0 = 48;
  GLFW_KEY_1 = 49;
  GLFW_KEY_2 = 50;
  GLFW_KEY_3 = 51;
  GLFW_KEY_4 = 52;
  GLFW_KEY_5 = 53;
  GLFW_KEY_6 = 54;
  GLFW_KEY_7 = 55;
  GLFW_KEY_8 = 56;
  GLFW_KEY_9 = 57;
  GLFW_KEY_SEMICOLON = 59;
  GLFW_KEY_EQUAL = 61;
  GLFW_KEY_A = 65;
  GLFW_KEY_B = 66;
  GLFW_KEY_C = 67;
  GLFW_KEY_D = 68;
  GLFW_KEY_E = 69;
  GLFW_KEY_F = 70;
  GLFW_KEY_G = 71;
  GLFW_KEY_H = 72;
  GLFW_KEY_I = 73;
  GLFW_KEY_J = 74;
  GLFW_KEY_K = 75;
  GLFW_KEY_L = 76;
  GLFW_KEY_M = 77;
  GLFW_KEY_N = 78;
  GLFW_KEY_O = 79;
  GLFW_KEY_P = 80;
  GLFW_KEY_Q = 81;
  GLFW_KEY_R = 82;
  GLFW_KEY_S = 83;
  GLFW_KEY_T = 84;
  GLFW_KEY_U = 85;
  GLFW_KEY_V = 86;
  GLFW_KEY_W = 87;
  GLFW_KEY_X = 88;
  GLFW_KEY_Y = 89;
  GLFW_KEY_Z = 90;
  GLFW_KEY_LEFT_BRACKET = 91;
  GLFW_KEY_BACKSLASH = 92;
  GLFW_KEY_RIGHT_BRACKET = 93;
  GLFW_KEY_GRAVE_ACCENT = 96;
  GLFW_KEY_WORLD_1 = 161;
  GLFW_KEY_WORLD_2 = 162;

  GLFW_KEY_ESCAPE = 256;
  GLFW_KEY_ENTER = 257;
  GLFW_KEY_TAB = 258;
  GLFW_KEY_BACKSPACE = 259;
  GLFW_KEY_INSERT = 260;
  GLFW_KEY_DELETE = 261;
  GLFW_KEY_RIGHT = 262;
  GLFW_KEY_LEFT = 263;
  GLFW_KEY_DOWN = 264;
  GLFW_KEY_UP = 265;
  GLFW_KEY_PAGE_UP = 266;
  GLFW_KEY_PAGE_DOWN = 267;
  GLFW_KEY_HOME = 268;
  GLFW_KEY_END = 269;
  GLFW_KEY_CAPS_LOCK = 280;
  GLFW_KEY_SCROLL_LOCK = 281;
  GLFW_KEY_NUM_LOCK = 282;
  GLFW_KEY_PRINT_SCREEN = 283;
  GLFW_KEY_PAUSE = 284;
  GLFW_KEY_F1 = 290;
  GLFW_KEY_F2 = 291;
  GLFW_KEY_F3 = 292;
  GLFW_KEY_F4 = 293;
  GLFW_KEY_F5 = 294;
  GLFW_KEY_F6 = 295;
  GLFW_KEY_F7 = 296;
  GLFW_KEY_F8 = 297;
  GLFW_KEY_F9 = 298;
  GLFW_KEY_F10 = 299;
  GLFW_KEY_F11 = 300;
  GLFW_KEY_F12 = 301;
  GLFW_KEY_F13 = 302;
  GLFW_KEY_F14 = 303;
  GLFW_KEY_F15 = 304;
  GLFW_KEY_F16 = 305;
  GLFW_KEY_F17 = 306;
  GLFW_KEY_F18 = 307;
  GLFW_KEY_F19 = 308;
  GLFW_KEY_F20 = 309;
  GLFW_KEY_F21 = 310;
  GLFW_KEY_F22 = 311;
  GLFW_KEY_F23 = 312;
  GLFW_KEY_F24 = 313;
  GLFW_KEY_F25 = 314;
  GLFW_KEY_KP_0 = 320;
  GLFW_KEY_KP_1 = 321;
  GLFW_KEY_KP_2 = 322;
  GLFW_KEY_KP_3 = 323;
  GLFW_KEY_KP_4 = 324;
  GLFW_KEY_KP_5 = 325;
  GLFW_KEY_KP_6 = 326;
  GLFW_KEY_KP_7 = 327;
  GLFW_KEY_KP_8 = 328;
  GLFW_KEY_KP_9 = 329;
  GLFW_KEY_KP_DECIMAL = 330;
  GLFW_KEY_KP_DIVIDE = 331;
  GLFW_KEY_KP_MULTIPLY = 332;
  GLFW_KEY_KP_SUBTRACT = 333;
  GLFW_KEY_KP_ADD = 334;
  GLFW_KEY_KP_ENTER = 335;
  GLFW_KEY_KP_EQUAL = 336;
  GLFW_KEY_LEFT_SHIFT = 340;
  GLFW_KEY_LEFT_CONTROL = 341;
  GLFW_KEY_LEFT_ALT = 342;
  GLFW_KEY_LEFT_SUPER = 343;
  GLFW_KEY_RIGHT_SHIFT = 344;
  GLFW_KEY_RIGHT_CONTROL = 345;
  GLFW_KEY_RIGHT_ALT = 346;
  GLFW_KEY_RIGHT_SUPER = 347;
  GLFW_KEY_MENU = 348;
  GLFW_KEY_LAST = GLFW_KEY_MENU;



  GLFW_MOD_SHIFT = $0001;

  GLFW_MOD_CONTROL = $0002;

  GLFW_MOD_ALT = $0004;

  GLFW_MOD_SUPER = $0008;


  GLFW_MOUSE_BUTTON_1 = 0;
  GLFW_MOUSE_BUTTON_2 = 1;
  GLFW_MOUSE_BUTTON_3 = 2;
  GLFW_MOUSE_BUTTON_4 = 3;
  GLFW_MOUSE_BUTTON_5 = 4;
  GLFW_MOUSE_BUTTON_6 = 5;
  GLFW_MOUSE_BUTTON_7 = 6;
  GLFW_MOUSE_BUTTON_8 = 7;
  GLFW_MOUSE_BUTTON_LAST = GLFW_MOUSE_BUTTON_8;
  GLFW_MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_1;
  GLFW_MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_2;
  GLFW_MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_3;


  GLFW_JOYSTICK_1 = 0;
  GLFW_JOYSTICK_2 = 1;
  GLFW_JOYSTICK_3 = 2;
  GLFW_JOYSTICK_4 = 3;
  GLFW_JOYSTICK_5 = 4;
  GLFW_JOYSTICK_6 = 5;
  GLFW_JOYSTICK_7 = 6;
  GLFW_JOYSTICK_8 = 7;
  GLFW_JOYSTICK_9 = 8;
  GLFW_JOYSTICK_10 = 9;
  GLFW_JOYSTICK_11 = 10;
  GLFW_JOYSTICK_12 = 11;
  GLFW_JOYSTICK_13 = 12;
  GLFW_JOYSTICK_14 = 13;
  GLFW_JOYSTICK_15 = 14;
  GLFW_JOYSTICK_16 = 15;
  GLFW_JOYSTICK_LAST = GLFW_JOYSTICK_16;



  GLFW_NOT_INITIALIZED = $00010001;

  GLFW_NO_CURRENT_CONTEXT = $00010002;

  GLFW_INVALID_ENUM = $00010003;

  GLFW_INVALID_VALUE = $00010004;

  GLFW_OUT_OF_MEMORY = $00010005;

  GLFW_API_UNAVAILABLE = $00010006;

  GLFW_VERSION_UNAVAILABLE = $00010007;

  GLFW_PLATFORM_ERROR = $00010008;

  GLFW_FORMAT_UNAVAILABLE = $00010009;

  GLFW_FOCUSED = $00020001;
  GLFW_ICONIFIED = $00020002;
  GLFW_RESIZABLE = $00020003;
  GLFW_VISIBLE = $00020004;
  GLFW_DECORATED = $00020005;
  GLFW_RED_BITS = $00021001;
  GLFW_GREEN_BITS = $00021002;
  GLFW_BLUE_BITS = $00021003;
  GLFW_ALPHA_BITS = $00021004;
  GLFW_DEPTH_BITS = $00021005;
  GLFW_STENCIL_BITS = $00021006;
  GLFW_ACCUM_RED_BITS = $00021007;
  GLFW_ACCUM_GREEN_BITS = $00021008;
  GLFW_ACCUM_BLUE_BITS = $00021009;
  GLFW_ACCUM_ALPHA_BITS = $0002100A;
  GLFW_AUX_BUFFERS = $0002100B;
  GLFW_STEREO = $0002100C;
  GLFW_SAMPLES = $0002100D;
  GLFW_SRGB_CAPABLE = $0002100E;
  GLFW_REFRESH_RATE = $0002100F;
  GLFW_CLIENT_API = $00022001;
  GLFW_CONTEXT_VERSION_MAJOR = $00022002;
  GLFW_CONTEXT_VERSION_MINOR = $00022003;
  GLFW_CONTEXT_REVISION = $00022004;
  GLFW_CONTEXT_ROBUSTNESS = $00022005;
  GLFW_OPENGL_FORWARD_COMPAT = $00022006;
  GLFW_OPENGL_DEBUG_CONTEXT = $00022007;
  GLFW_OPENGL_PROFILE = $00022008;
  GLFW_OPENGL_API = $00030001;
  GLFW_OPENGL_ES_API = $00030002;
  GLFW_NO_ROBUSTNESS = 0;
  GLFW_NO_RESET_NOTIFICATION = $00031001;
  GLFW_LOSE_CONTEXT_ON_RESET = $00031002;
  GLFW_OPENGL_ANY_PROFILE = 0;
  GLFW_OPENGL_CORE_PROFILE = $00032001;
  GLFW_OPENGL_COMPAT_PROFILE = $00032002;
  GLFW_CURSOR = $00033001;
  GLFW_STICKY_KEYS = $00033002;
  GLFW_STICKY_MOUSE_BUTTONS = $00033003;
  GLFW_CURSOR_NORMAL = $00034001;
  GLFW_CURSOR_HIDDEN = $00034002;
  GLFW_CURSOR_DISABLED = $00034003;
  GLFW_CONNECTED = $00040001;
  GLFW_DISCONNECTED = $00040002;
//All call must be  cdecl
function glfwInit: integer cdecl; external GLFW_DLL;
procedure glfwTerminate; cdecl; external GLFW_DLL;
procedure glfwGetVersion(out major, minor, rev: integer);
  cdecl; external GLFW_DLL;
function glfwGetVersionString: PChar; cdecl; external GLFW_DLL;
//========================================================================
//Monitor
//========================================================================
function glfwGetMonitors(var Monitors: GLFW_INT): GLFWmonitor; cdecl; external GLFW_DLL;
function glfwGetPrimaryMonitor: pGLFWmonitor; cdecl; external GLFW_DLL;
procedure glfwGetMonitorPos(monitor: pGLFWmonitor; var xpos: GLFW_INT;
  var ypos: GLFW_INT); cdecl; external GLFW_DLL;
procedure glfwGetMonitorPhysicalSize(Monitor: pGLFWmonitor; var Width: GLFW_INT;
  var Height: GLFW_INT); cdecl; external GLFW_DLL;
function glfwGetMonitorName(Monitor: pGLFWmonitor): PChar; cdecl; external GLFW_DLL;
function glfwSetMonitorCallback(cbfun: GLFWmonitorfun): pGLFWmonitorfun;
  cdecl; external GLFW_DLL; //Untest
//========================================================================
//Video Mode
//========================================================================
function glfwGetVideoModes(monitor: pGLFWmonitor; var Count: longint): PGLFWvidmode;
  cdecl; external GLFW_DLL;
procedure glfwSetGamma(monitor: pGLFWmonitor; gamma: single); cdecl; external GLFW_DLL;
procedure glfwSetGammaRamp(monitor: pGLFWmonitor; ramp: pGLFWgammaramp);
  cdecl; external GLFW_DLL;
//========================================================================
//Windows (not the SO)
//========================================================================
procedure glfwDefaultWindowHints; cdecl; external GLFW_DLL;
procedure glfwWindowHint(target: integer; hint: integer); cdecl; external GLFW_DLL;
function glfwCreateWindow(Width, Height: integer; title: PChar;
  monitor: pGLFWmonitor; share: pGLFWwindow): pGLFWwindow; cdecl; external GLFW_DLL;
procedure glfwDestroyWindow(Window: pGLFWwindow); cdecl; external GLFW_DLL;
function glfwWindowShouldClose(Window: pGLFWwindow): integer; cdecl; external GLFW_DLL;
procedure glfwSetWindowShouldClose(window: pGLFWwindow; Action: GLFW_INT);
  cdecl; external GLFW_DLL;
procedure glfwSetWindowTitle(window: pGLFWwindow; title: PChar); cdecl;
  external GLFW_DLL;
procedure glfwGetWindowPos(window: pGLFWWindow; var xpos, ypos: integer);
  cdecl; external GLFW_DLL;
procedure glfwSetWindowPos(window: pGLFWwindow; xpos, ypos: integer);
  cdecl; external GLFW_DLL;
procedure glfwGetWindowSize(window: pGLFWwindow; var Width, Height: integer); cdecl;
  external GLFW_DLL;
procedure glfwSetWindowSize(window: pGLFWwindow; var Width, Height: integer);
  cdecl; external GLFW_DLL;
procedure glfwGetFramebufferSize(window: pGLFWwindow; var Width, Height: integer); cdecl;
  external GLFW_DLL;
procedure glfwIconifyWindow(window: pGLFWwindow); cdecl; external GLFW_DLL;
procedure glfwRestoreWindow(window: pGLFWwindow); cdecl; external GLFW_DLL;
procedure glfwShowWindow(window: pGLFWwindow); cdecl; external GLFW_DLL;
procedure glfwHideWindow(window: pGLFWwindow); cdecl; external GLFW_DLL;
function glfwGetWindowMonitor(window: pGLFWwindow): pGLFWmonitor;
  cdecl; external GLFW_DLL;
procedure glfwGetWindowAttrib(window: pGLFWwindow; attrib: integer);
  cdecl; external GLFW_DLL;
{ TODO : Es correcto usar pointer como tipo de dato? }
procedure glfwSetWindowUserPointer(window: pGLFWwindow; p: pointer);
  cdecl; external GLFW_DLL;  //Pointer??
function glfwGetWindowUserPointer(window: GLFWwindow): Pointer; cdecl; external GLFW_DLL;
function glfwSetWindowPosCallback(window: pGLFWwindow;
  cbfun: GLFWwindowposfun): pGLFWwindowposfun; cdecl; external GLFW_DLL;
function glfwSetWindowSizeCallback(window: pGLFWwindow;
  cbfun: GLFWwindowsizefun): pGLFWwindowsizefun; cdecl; external GLFW_DLL;
function glfwSetWindowCloseCallback(window: pGLFWwindow;
  ccbfun: GLFWwindowclosefun): pGLFWwindowclosefun; cdecl; external GLFW_DLL;
function glfwSetWindowRefreshCallback(window: pGLFWwindow;
  cbfun: GLFWwindowrefreshfun): pGLFWwindowrefreshfun; cdecl; external GLFW_DLL;
function glfwSetWindowFocusCallback(window: pGLFWwindow;
  cbfun: GLFWwindowfocusfun): pGLFWwindowfocusfun; cdecl; external GLFW_DLL;
function glfwSetWindowIconifyCallback(window: pGLFWwindow;
  cbfun: GLFWwindowiconifyfun): pGLFWwindowiconifyfun; cdecl; external GLFW_DLL;
function glfwSetFramebufferSizeCallback(window: pGLFWwindow;
  cbfun: GLFWframebuffersizefun): pGLFWframebuffersizefun; cdecl; external GLFW_DLL;
procedure glfwPollEvents; cdecl; external GLFW_DLL;
procedure glfwWaitEvents; cdecl; external GLFW_DLL;
//========================================================================
//Input
//========================================================================
function glfwGetInputMode(window: pGLFWwindow; mode: integer): integer;
  cdecl; external GLFW_DLL;
procedure glfwSetInputMode(window: pGLFWwindow; mode, Value: integer); cdecl;
  external GLFW_DLL;
function glfwGetKey(window: pGLFWwindow; key: integer): integer;
  cdecl; external GLFW_DLL;
function glfwGetMouseButton(window: pGLFWwindow; button: integer): integer;
  cdecl; external GLFW_DLL;
function glfwGetCursorPos(window: pGLFWwindow; var xpos, ypos: double): integer;
  cdecl; external GLFW_DLL;
procedure glfwSetCursorPos(window: pGLFWwindow; xpos, ypos: double);
  cdecl; external GLFW_DLL;
function glfwSetKeyCallback(window: pGLFWwindow; cbfun: GLFWkeyfun): pGLFWKeyFun;
  cdecl; external GLFW_DLL;
function glfwSetCharCallback(window: pGLFWwindow;
  cdfun: GLFWcharfun): pGLFWcharfun; cdecl;
  external GLFW_DLL;
function glfwSetMouseButtonCallback(window: pGLFWwindow;
  cbfun: GLFWmousebuttonfun): pGLFWmousebuttonfun; cdecl; external GLFW_DLL;
function glfwSetCursorPosCallback(window: pGLFWwindow;
  cbfun: GLFWcursorposfun): pGLFWcursorposfun; cdecl; external GLFW_DLL;
function glfwSetCursorEnterCallback(window: pGLFWwindow;
  cbfun: GLFWcursorenterfun): pGLFWcursorenterfun; cdecl; external GLFW_DLL;
function glfwSetScrollCallback(window: pGLFWwindow;
  cbfun: GLFWscrollfun): pGLFWscrollfun; cdecl; external GLFW_DLL;
function glfwJoystickPresent(joy: integer): integer; cdecl; external GLFW_DLL;
function glfwGetJoystickAxes(joy: integer; var Count: integer): pfloat;
  cdecl; external GLFW_DLL;
{ TODO : ¿Como gestionar los punteros a arrays? }
function glfwGetJoystickButtons(joy: integer; varcount: integer): pCharArray;
  cdecl; external GLFW_DLL;
function glfwGetJoystickName(joy: integer): PChar; cdecl; external GLFW_DLL;


//========================================================================
// clipboard
//========================================================================
procedure glfwSetClipboardString(window: pGLFWwindow; char: string);
  cdecl; external GLFW_DLL;
function glfwGetClipboardString(window: pGLFWwindow): PChar; cdecl; external GLFW_DLL;

//========================================================================
//Time
//========================================================================
function glfwGetTime: double; cdecl; external GLFW_DLL;
procedure glfwSetTime(time: double); cdecl; external GLFW_DLL;

//========================================================================
//Context
//========================================================================
procedure glfwSwapBuffers(window: pGLFWwindow); cdecl; external GLFW_DLL;
procedure glfwMakeContextCurrent(window: pGLFWwindow); cdecl; external GLFW_DLL;
function glfwGetCurrentContext: pGLFWwindow; cdecl; external GLFW_DLL;
procedure glfwSwapInterval(interval: integer); cdecl; external GLFW_DLL;
function glfwExtensionSupported(extension: PChar): integer; cdecl; external GLFW_DLL;
function glfwGetProcAddress(procname: PChar): pGLFWglproc; cdecl; external GLFW_DLL;

//========================================================================
// Error
//========================================================================

function glfwSetErrorCallback(cbfun: GLFWerrorfun): pGLFWerrorfun;
  cdecl; external GLFW_DLL;


implementation

end.
