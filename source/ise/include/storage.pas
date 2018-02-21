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

unit storage;

interface

  const
    HDRSIZE = 512;    
    NAMSIZE = 200;    
    RECURDIR = 1;    
    FASTDIR = 2;    

  type
    header = record
        name : array[0..(NAMSIZE)-1] of char;
        size : cardinal;
        chksum : cardinal;
        fill : array[0..((HDRSIZE-NAMSIZE)-(2*(sizeof(cardinal))))-1] of char;
      end;

    pdirentry = ^direntry;
    direntry = record
        _string : pchar;
        _type : longint;
        relname : pchar;
        //s : stat;
        flags : cardinal;
        size : array[0..15] of char;
        mode1 : array[0..3] of char;
        mode2 : array[0..3] of char;
        mode3 : array[0..3] of char;
        owner : array[0..15] of char;
        time : array[0..7] of char;
        date : array[0..15] of char;
        extra : array[0..15] of char;
        poin : pointer;
        nr : longint;
      end;

    pdirlink = ^dirlink;
    dirlink = record
        next : pdirlink;
        prev : pdirlink;
        name : pchar;
      end;

  { ******** storage.c  }

  //procedure adddirstrings;cdecl;

  //procedure builddir(dirname:Pchar; relname:Pchar);cdecl;

  //function compare(entry1:Pdirentry; entry2:Pdirentry):longint;cdecl;

  //function copy(cpy:Pchar; org:Pchar):smallint;cdecl;

  //function diskfree(dir:Pchar):double;cdecl;

  //function exist(name:Pchar):longint;cdecl;

  //function existo(name:Pchar):longint;cdecl;

  //procedure fastdir(on:longint);cdecl;

  //function filesize(file:longint):longint;cdecl;

  //procedure freedir;cdecl;

  //function getdir(dirname:Pchar; filelist:PPdirentry):uint;cdecl;

  //function getwdN(dir:Pchar):^char;cdecl;

  //function recurdir(name:Pchar):longint;cdecl;

implementation

end.
