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

unit objfnt;

interface

  type
    pchardesc = ^chardesc;
    chardesc = record
        movex : smallint;         { advance  }
        movey : smallint;
        llx : smallint;           { bounding box  }
        lly : smallint;
        urx : smallint;
        ury : smallint;
        data : psmallint;         { char data  }
        datalen : longint;
      end;

  pobjfnt = ^objfnt_;
    objfnt_ = record
        freeaddr : pobjfnt; { if freeaddr != 0, objfnt is one chunck  }
        _type : smallint;
        charmin : smallint;
        charmax : smallint;
        my_nchars : smallint;
        scale : smallint;
        my_chars : pchardesc;
      end;

  const
    OFMAGIC = $93339333;

    TM_TYPE = 1;    
    PO_TYPE = 2;    
    SP_TYPE = 3;

  { ops for tmesh characters  }

    TM_BGNTMESH = 1;    
    TM_SWAPTMESH = 2;    
    TM_ENDBGNTMESH = 3;    
    TM_RETENDTMESH = 4;    
    TM_RET = 5;

  { ops for poly characters  }

    PO_BGNLOOP = 1;    
    PO_ENDBGNLOOP = 2;    
    PO_RETENDLOOP = 3;    
    PO_RET = 4;

  { ops for spline  characters  }

    SP_MOVETO = 1;    
    SP_LINETO = 2;    
    SP_CURVETO = 3;    
    SP_CLOSEPATH = 4;    
    SP_RETCLOSEPATH = 5;    
    SP_RET = 6;

    MIN_ASCII = ' ';    
    MAX_ASCII = '~';    
    NASCII = 256-32;

    NOBBOX = 30000;    

  type
    pschar = record
        name : pchar;
        code : longint;
        prog : longint;
      end;

  //  var
  //    charlist : array[0..(NASCII)-1] of pschar;cvar;external;
  //
  //function fontname:^objfnt;
  //
  //function readobjfnt:^objfnt;
  //
  //function newobjfnt:^objfnt;
  //
  //function fontstringwidth:single;
  //
  //function getcharprog:^smallint;
  //
  //function getchardesc:^chardesc;
  //
  //function asciiname:^char;

implementation

  //function fontname:Pobjfnt;
  //begin
  //  { You must implement this function }
  //end;
  //function readobjfnt:Pobjfnt;
  //begin
  //  { You must implement this function }
  //end;
  //function newobjfnt:Pobjfnt;
  //begin
  //  { You must implement this function }
  //end;
  //function fontstringwidth:single;
  //begin
  //  { You must implement this function }
  //end;
  //function getcharprog:Psmallint;
  //begin
  //  { You must implement this function }
  //end;
  //function getchardesc:Pchardesc;
  //begin
  //  { You must implement this function }
  //end;
  //function asciiname:Pchar;
  //begin
  //  { You must implement this function }
  //end;

end.
