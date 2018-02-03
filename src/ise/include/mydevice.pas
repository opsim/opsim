{*
 * $Id:$
 * ***** BEGIN GPL/BL DUAL LICENSE BLOCK *****
 *
 * The contents of this file may be used under the terms of either the GNU
 * General Public License Version 2 or later (the "GPL", see
 * http://www.gnu.org/licenses/gpl.html ), or the Blender License 1.0 or
 * later (the "BL", see http://www.blender.org/BL/ ) which has to be
 * bought from the Blender Foundation to become active, in which case the
 * above mentioned GPL option does not apply.
 *
 * The Original Code is Copyright (C) 2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL/BL DUAL LICENSE BLOCK *****
  }
unit mydevice;

interface

  { OMZETTING VAN X NAAR IRIS GL CODES  }
  {
   * 
   *   mouse / timer / window: tot 0x020
   *   eigen codes: 0x4...
   * 
    }
  { MOUSE : 0x00x  }

  const
    LEFTMOUSE = $001;    
    MIDDLEMOUSE = $002;    
    RIGHTMOUSE = $003;    
    MOUSEX = $004;    
    MOUSEY = $005;    
  { timers  }
    TIMER0 = $006;    
    TIMER1 = $007;    
    TIMER2 = $008;    
    TIMER3 = $009;    
  { SYSTEM : 0x01x  }
  { keyboard  }
    KEYBD = $010;    
  { raw keyboard for keyboard manager  }
    RAWKEYBD = $011;    
  { used by port manager to signal redraws  }
    REDRAW = $012;    
  { input connected or disconnected  }
    INPUTCHANGE = $013;    
  { queue was filled  }
    QFULL = $014;    
  { user wants process in this win to shut up  }
    WINFREEZE = $015;    
  { user wants process in this win to go again  }
    WINTHAW = $016;    
  { window close  }
    WINCLOSE = $017;    
  { signal from user that app is to go away  }
    WINQUIT = $018;    
  { on startup  }
    Q_FIRSTTIME = $019;    

    { standard keyboard  }
{$if defined(BEOS) or defined(WINDOWS)}

  const
    AKEY = 'a';    
    BKEY = 'b';    
    CKEY = 'c';    
    DKEY = 'd';    
    EKEY = 'e';    
    FKEY = 'f';    
    GKEY = 'g';    
    HKEY = 'h';    
    IKEY = 'i';    
    JKEY = 'j';    
    KKEY = 'k';    
    LKEY = 'l';    
    MKEY = 'm';    
    NKEY = 'n';    
    OKEY = 'o';    
    PKEY = 'p';    
    QKEY = 'q';    
    RKEY = 'r';    
    SKEY = 's';    
    TKEY = 't';    
    UKEY = 'u';    
    VKEY = 'v';    
    WKEY = 'w';    
    XKEY = 'x';    
    YKEY = 'y';    
    ZKEY = 'z';    
    ZEROKEY = '0';    
    ONEKEY = '1';    
    TWOKEY = '2';    
    THREEKEY = '3';    
    FOURKEY = '4';    
    FIVEKEY = '5';    
    SIXKEY = '6';    
    SEVENKEY = '7';    
    EIGHTKEY = '8';    
    NINEKEY = '9';    
    CAPSLOCKKEY = 211;    
    LEFTCTRLKEY = 212;    
    LEFTALTKEY = 213;    
    RIGHTALTKEY = 214;    
    RIGHTCTRLKEY = 215;    
    RIGHTSHIFTKEY = 216;    
    LEFTSHIFTKEY = 217;    
    ESCKEY = 218;    
    TABKEY = 219;    
    RETKEY = 220;    
    SPACEKEY = 221;    
    LINEFEEDKEY = 222;    
    BACKSPACEKEY = 223;    
    DELKEY = 224;    
    SEMICOLONKEY = 225;    
    PERIODKEY = 226;    
    COMMAKEY = 227;    
    QUOTEKEY = 228;    
    ACCENTGRAVEKEY = 229;    
    MINUSKEY = 230;    
    VIRGULEKEY = 231;    
    SLASHKEY = 232;    
    BACKSLASHKEY = 233;    
    EQUALKEY = 234;    
    LEFTBRACKETKEY = 235;    
    RIGHTBRACKETKEY = 236;    
    LEFTARROWKEY = 137;    
    DOWNARROWKEY = 138;    
    RIGHTARROWKEY = 139;    
    UPARROWKEY = 140;    
    PAD2 = 150;    
    PAD4 = 151;    
    PAD6 = 152;    
    PAD8 = 153;    
    PAD1 = 154;    
    PAD3 = 155;    
    PAD5 = 156;    
    PAD7 = 157;    
    PAD9 = 158;    
    PADPERIOD = 199;    
    PADVIRGULEKEY = 159;    
    PADASTERKEY = 160;    
    PAD0 = 161;    
    PADMINUS = 162;    
    PADENTER = 163;    
    PADPLUSKEY = 164;    
    F1KEY = 300;    
    F2KEY = 301;    
    F3KEY = 302;    
    F4KEY = 303;    
    F5KEY = 304;    
    F6KEY = 305;    
    F7KEY = 306;    
    F8KEY = 307;    
    F9KEY = 308;    
    F10KEY = 309;    
    F11KEY = 310;    
    F12KEY = 312;    
    PAUSEKEY = 165;    
    INSERTKEY = 166;    
    HOMEKEY = 167;    
    PAGEUPKEY = 168;    
    PAGEDOWNKEY = 169;    
    ENDKEY = 170;    
{$else}
  { Keycodes derived from X  }

  const
    AKEY = XK_a;    
    BKEY = XK_b;    
    CKEY = XK_c;    
    DKEY = XK_d;    
    EKEY = XK_e;    
    FKEY = XK_f;    
    GKEY = XK_g;    
    HKEY = XK_h;    
    IKEY = XK_i;    
    JKEY = XK_j;    
    KKEY = XK_k;    
    LKEY = XK_l;    
    MKEY = XK_m;    
    NKEY = XK_n;    
    OKEY = XK_o;    
    PKEY = XK_p;    
    QKEY = XK_q;    
    RKEY = XK_r;    
    SKEY = XK_s;    
    TKEY = XK_t;    
    UKEY = XK_u;    
    VKEY = XK_v;    
    WKEY = XK_w;    
    XKEY = XK_x;    
    YKEY = XK_y;    
    ZKEY = XK_z;    
    ZEROKEY = XK_0;    
    ONEKEY = XK_1;    
    TWOKEY = XK_2;    
    THREEKEY = XK_3;    
    FOURKEY = XK_4;    
    FIVEKEY = XK_5;    
    SIXKEY = XK_6;    
    SEVENKEY = XK_7;    
    EIGHTKEY = XK_8;    
    NINEKEY = XK_9;    
    CAPSLOCKKEY = XK_Caps_Lock;    
    LEFTCTRLKEY = XK_Control_L;    
    LEFTALTKEY = XK_Alt_L;    
    RIGHTALTKEY = XK_Alt_R;    
    RIGHTCTRLKEY = XK_Control_R;    
    RIGHTSHIFTKEY = XK_Shift_R;    
    LEFTSHIFTKEY = XK_Shift_L;    
    ESCKEY = XK_Escape;    
    TABKEY = XK_Tab;    
    RETKEY = XK_Return;    
    SPACEKEY = XK_space;    
    LINEFEEDKEY = XK_Linefeed;    
    BACKSPACEKEY = XK_BackSpace;    
    DELKEY = XK_Delete;    
    SEMICOLONKEY = XK_semicolon;    
    PERIODKEY = XK_period;    
    COMMAKEY = XK_comma;    
    QUOTEKEY = XK_quoteright;    
    ACCENTGRAVEKEY = XK_quoteleft;    
    MINUSKEY = XK_minus;    
    VIRGULEKEY = XK_slash;    
    SLASHKEY = XK_slash;    
    BACKSLASHKEY = XK_backslash;    
    EQUALKEY = XK_equal;    
    LEFTBRACKETKEY = XK_bracketleft;    
    RIGHTBRACKETKEY = XK_bracketright;    
    LEFTARROWKEY = XK_Left;    
    DOWNARROWKEY = XK_Down;    
    RIGHTARROWKEY = XK_Right;    
    UPARROWKEY = XK_Up;    
  { dit zijn de arrow keys voor SUN  }
    PAD2 = XK_KP_Down;    
    PAD4 = XK_KP_Left;    
    PAD6 = XK_KP_Right;    
    PAD8 = XK_KP_Up;    
{$ifdef __SUN}
    PAD1 = $ffde;    
    PAD3 = $ffe0;    
    PAD5 = $ffdc;    
    PAD7 = $ffd8;    
    PAD9 = $ffda;    
    PADPERIOD = $ffff;    
    PADVIRGULEKEY = $ffd6;    
    PADASTERKEY = $ffd7;    
{$else}

  const
    PAD1 = XK_KP_End;    
    PAD3 = XK_KP_Page_Down;    
    PAD5 = XK_KP_Begin;    
    PAD7 = XK_KP_Home;    
    PAD9 = XK_KP_Page_Up;    
    PADPERIOD = XK_KP_Delete;    
    PADVIRGULEKEY = XK_KP_Divide;    
    PADASTERKEY = XK_KP_Multiply;    
{$endif}

  const
    PAD0 = XK_KP_Insert;    
    PADMINUS = XK_KP_Subtract;    
    PADENTER = XK_KP_Enter;    
    PADPLUSKEY = XK_KP_Add;    
  { the extended keyboard  }
    F1KEY = XK_F1;    
    F2KEY = XK_F2;    
    F3KEY = XK_F3;    
    F4KEY = XK_F4;    
    F5KEY = XK_F5;    
    F6KEY = XK_F6;    
    F7KEY = XK_F7;    
    F8KEY = XK_F8;    
    F9KEY = XK_F9;    
    F10KEY = XK_F10;    
    F11KEY = XK_F11;    
    F12KEY = XK_F12;    
    PAUSEKEY = XK_Pause;    
    INSERTKEY = XK_Insert;    
    HOMEKEY = XK_Home;    
    PAGEUPKEY = XK_Page_Up;    
    PAGEDOWNKEY = XK_Page_Down;    
    ENDKEY = XK_End;    
{$endif}

{ !__MYDEVICE_H__  }

implementation


end.
