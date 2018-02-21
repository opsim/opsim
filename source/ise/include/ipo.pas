(**
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
 *)
(* ipo.h
 *
 * april 95
 *
 *)

unit ipo;

interface

uses
blender, util;

  type
    pIpoKey = ^IpoKey;
    IpoKey = record
        next : ^IpoKey;
        prev : ^IpoKey;
        flag : smallint;
        rt : smallint;
        val : single;
        data : ppBezTriple;
      end;

    pEditIpo = ^EditIpo;
    EditIpo = record
        name : array[0..11] of char;
        icu : ^IpoCurve;
        adrcode : smallint;
        flag : smallint;
        disptype : smallint;
        rt : smallint;
        col : cardinal;
      end;

  const
    IPOBUTY = 17;    
  { disptype  }
    IPO_DISPDEGR = 1;    
    IPO_DISPBITS = 2;    
    IPO_DISPTIME = 3;    
  { ********************  }
    OB_TOTIPO = 23;    
    OB_LOC_X = 1;    
    OB_LOC_Y = 2;    
    OB_LOC_Z = 3;    
    OB_DLOC_X = 4;    
    OB_DLOC_Y = 5;    
    OB_DLOC_Z = 6;    
    OB_ROT_X = 7;    
    OB_ROT_Y = 8;    
    OB_ROT_Z = 9;    
    OB_DROT_X = 10;    
    OB_DROT_Y = 11;    
    OB_DROT_Z = 12;    
    OB_SIZE_X = 13;    
    OB_SIZE_Y = 14;    
    OB_SIZE_Z = 15;    
    OB_DSIZE_X = 16;    
    OB_DSIZE_Y = 17;    
    OB_DSIZE_Z = 18;    
    OB_LAY = 19;    
    OB_TIME = 20;    
    OB_EFF_X = 21;    
    OB_EFF_Y = 22;    
    OB_EFF_Z = 23;    
    OB_COL_R = 21;    
    OB_COL_G = 22;    
    OB_COL_B = 23;    
  { ********************  }
    MA_TOTIPO = 32;    
    MA_COL_R = 1;    
    MA_COL_G = 2;    
    MA_COL_B = 3;    
    MA_SPEC_R = 4;    
    MA_SPEC_G = 5;    
    MA_SPEC_B = 6;    
    MA_MIR_R = 7;    
    MA_MIR_G = 8;    
    MA_MIR_B = 9;    
    MA_REF = 10;    
    MA_ALPHA = 11;    
    MA_EMIT = 12;    
    MA_AMB = 13;    
    MA_SPEC = 14;    
    MA_HARD = 15;    
    MA_SPTR = 16;    
    MA_ANG = 17;    
    MA_MODE = 18;    
    MA_HASIZE = 19;    
    MA_MAP1 = $20;    
    MA_MAP2 = $40;    
    MA_MAP3 = $80;    
    MA_MAP4 = $100;    
    MA_MAP5 = $200;    
    MA_MAP6 = $400;    
    MA_MAP7 = $800;    
    MA_MAP8 = $1000;    
    MAP_OFS_X = 1;    
    MAP_OFS_Y = 2;    
    MAP_OFS_Z = 3;    
    MAP_SIZE_X = 4;    
    MAP_SIZE_Y = 5;    
    MAP_SIZE_Z = 6;    
    MAP_R = 7;    
    MAP_G = 8;    
    MAP_B = 9;    
    MAP_DVAR = 10;    
    MAP_COLF = 11;    
    MAP_NORF = 12;    
    MAP_VARF = 13;    
  { ********************  }
    SEQ_TOTIPO = 1;    
    SEQ_FAC1 = 1;    
  { ********************  }
    CU_TOTIPO = 1;    
    CU_SPEED = 1;    
  { ********************  }
    KEY_TOTIPO = 32;    
    KEY_SPEED = 0;    
    KEY_NR = 1;    
  { ********************  }
    WO_TOTIPO = 29;    
    WO_HOR_R = 1;    
    WO_HOR_G = 2;    
    WO_HOR_B = 3;    
    WO_ZEN_R = 4;    
    WO_ZEN_G = 5;    
    WO_ZEN_B = 6;    
    WO_EXPOS = 7;    
    WO_MISI = 8;    
    WO_MISTDI = 9;    
    WO_MISTSTA = 10;    
    WO_MISTHI = 11;    
    WO_STAR_R = 12;    
    WO_STAR_G = 13;    
    WO_STAR_B = 14;    
    WO_STARDIST = 15;    
    WO_STARSIZE = 16;    
  { ********************  }
    LA_TOTIPO = 23;    
    LA_ENERGY = 1;    
    LA_COL_R = 2;    
    LA_COL_G = 3;    
    LA_COL_B = 4;    
    LA_DIST = 5;    
    LA_SPOTSI = 6;    
    LA_SPOTBL = 7;    
    LA_QUAD1 = 8;    
    LA_QUAD2 = 9;    
    LA_HALOINT = 10;    
  { ********************  }
    CAM_TOTIPO = 3;    
    CAM_LENS = 1;    
    CAM_STA = 2;    
    CAM_END = 3;

  //{ drawipo.c  }

  //function select_proj_ipo(rectf:Prctf; event:longint):pEditIpo;

  //{ editipo.c  }
  //function get_ipo_to_edit(from:PPID):^Ipo;

  //procedure set_speed_editipo(speed:single);

  //function find_ipocurve(ipo:PIpo; adrcode:longint):^IpoCurve;

  //procedure editipo_changed(doredraw:longint);

  //function ipo_rainbow(cur:longint; tot:longint):cardinal;

  //function get_editipo:^EditIpo;

  //function get_ipo(from:PID; make:longint):^Ipo;

  //procedure free_ipokey(lb:pListBase);

  //procedure insertkey(id:PID; adrcode:longint);

  //procedure make_ipokey_spec(lb:pListBase; ipo:PIpo);

  //procedure make_ipokey_transform(ob:PObject; lb:pListBase; sel:longint);

  //procedure nextkey(elems:pListBase; dir:longint);

  //procedure set_ipo_pointers_transob(ik:PIpoKey; tob:PTransOb);

  //{ ipo.c  }
  //function frame_to_float(cfra:longint):single;

  //procedure calc_ipo(_para1:PIpo; _para2:single);

  //function calc_ipo_time(_para1:PIpo; _para2:single):single;

  //function add_ipo(name:Pchar; idcode:longint):^Ipo;

  //function read_ipo_poin(poin:pointer; _type:longint):single;

  //procedure write_ipo_poin(poin:pointer; _type:longint; val:single);

  //function get_ipo_poin(id:PID; icu:PIpoCurve; _type:Plongint):pointer;

  //function copy_ipo(ipo:PIpo):^Ipo;

  //procedure correct_bezpart(v1:Psingle; v2:Psingle; v3:Psingle; v4:Psingle);

  //function calc_ipo_spec(ipo:PIpo; adrcode:longint; ctime:Psingle):longint;

  //procedure make_local_obipo(ipo:PIpo);

  //procedure do_ob_ipo(ob:PObject);

  //procedure do_mat_ipo(ma:PMaterial);

//{$endif}
  { IPO_H  }

implementation

  procedure make_local_obipo(ipo:PIpo);
  begin
    { You must implement this function }
  end;

end.
