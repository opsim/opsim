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
{ file.h juli 94  }
{
 *
 *
 *
  }

unit file_;
interface

//{$ifndef FILE_H}
//{$define FILE_H}  
//  { flags voor wegschrijven/lezen scene  }
//{$include <fcntl.h>}
//{$include "blender.h"}
//{$include "storage.h"}
//{$ifndef WINDOWS}
//{$ifdef __FreeBSD__}
//{$include <sys/param.h>}
//{$include <sys/stat.h>}
//{$include <sys/mount.h>}
//{$else}
//{$if defined(linux)}
//{$include <sys/vfs.h>}
//{$else}
//{$ifndef __BeOS}
//{$include <sys/statfs.h>  }
//{$endif}
//{$endif}
//{$endif}
//{$endif}

  { INTEGER CODES  }

  //{ was #define dname def_expr }
  //function FORM : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function DDG1 : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function DDG2 : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function DDG3 : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function DDG4 : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function GOUR : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function BLEN : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function DER_ : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function V100 : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function DATA : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function GLOB : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function IMAG : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function DNA1 : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function TEST : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function REND : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function USER : longint; { return type might be wrong }
  //
  //{ was #define dname def_expr }
  //function ENDB : longint; { return type might be wrong }

  {# }
  {# }

  type
    SDNA = record
        data : pchar;
        datalen : longint;
        nr_names : longint;
        names : ppchar;
        nr_types : longint;
        pointerlen : longint;
        types : ppchar;
        typelens : psmallint;
        nr_structs : longint;
        structs : ^psmallint;
      end;

  {# }
  {# }

    BHead = record
        code : longint;
        len : longint;
        old : pointer;
        SDNAnr : longint;
        nr : longint;
      end;
  {# }
  {# }

    BHead4 = record
        code : longint;
        len : longint;
        old : longint;
        SDNAnr : longint;
        nr : longint;
      end;
  {# }
  {# }
{$ifdef WINDOWS}
{$else}
{$endif}

  type
    BHead8 = record
        code : longint;
        len : longint;
  {$ifdef WINDOWS}
        old : DWORD;
 {$else}
       old : int64;
 {$endif}
       SDNAnr : longint;
        nr : longint;
      end;
  { structs alleen voor files: (moeten WEL in SDNA)  }

    FileGlobal = record
        curscreen : pointer;
        displaymode : smallint;
        winpos : smallint;
        pad : longint;
        psx : array[0..79] of char;
      end;
  { genfile.c  }

  //procedure make_structDNA;cdecl;
  //
  //function findstruct_name(sdna:PSDNA; str:Pchar):psmallint;cdecl;
  //
  //function findstruct_nr(sdna:PSDNA; str:Pchar):longint;cdecl;
  //
  //function struct_compare(stype:Pchar; one:pointer; two:pointer):longint;cdecl;
  //
  //
  //  var
  //    cur_sdna : SDNA;cvar;external;
  //
  //function text_to_curve(ob:PObject; mode:longint):^chartrans;cdecl;
  //
  //procedure freestructDNA(sdna:PSDNA);cdecl;
  //
  //procedure freestructDNA_all;cdecl;
  //
  //procedure init_structDNA(sdna:PSDNA);cdecl;
  //
  //procedure reconstruct(oldSDNAnr:longint; blocks:longint; data:Ppointer);cdecl;
  //
  //procedure switch_endian_struct(oldSDNAnr:longint; data:Pchar);cdecl;
  //
  //{ readfile.c  }
  //function gethome:^char;cdecl;
  //
  //procedure add_data_adr(old:pointer; _new:pointer);cdecl;
  //
  //procedure add_lib_adr(old:pointer; _new:pointer);cdecl;
  //
  //procedure append_id_part(filedata:Pchar; main:PMain; id:PID);cdecl;
  //
  //procedure append_named_part(sfile:PSpaceFile; main:PMain; name:Pchar; idcode:longint);cdecl;
  //
  //procedure change_libadr(old:pointer; _new:pointer);cdecl;
  //
  //function convertstringcode(str:Pchar):longint;cdecl;
  //
  //procedure do_versions(main:PMain);cdecl;
  //
  //function find_bhead(old:pointer; filedata:Pchar):^BHead;cdecl;
  //
  //procedure give_base_to_objects(sce:PScene; lb:PListBase);cdecl;
  //
  //procedure inst_file(filename:Pchar; data:Pchar; size:longint);cdecl;
  //
  //function is_yet_read(main:PMain; bhead:PBHead):^ID;cdecl;
  //
  //procedure library_append(sfile:PSpaceFile);cdecl;
  //
  //{ append aan G
  //extern    void link_global(FileGlobal *fg);
  //extern    void link_list(ListBase *lb);		/* alleen direkte data  }
  //procedure link_recurs_seq(lb:PListBase);cdecl;
  //
  //procedure makestringcode(str:Pchar);cdecl;
  //
  //function mallocNN(len:longint; str:Pchar):pointer;cdecl;
  //
  //function newadr(adr:pointer):pointer;cdecl;
  //
  //{ alleen direkte datablokken  }
  //function newlibadr(lib:pointer; adr:pointer):pointer;cdecl;
  //
  //{ alleen Lib datablokken  }
  //function newlibadr_us(lib:pointer; adr:pointer):pointer;cdecl;
  //
  //{ hoogt usernummer op  }
  //function newlibadr_us_type(_type:smallint; adr:pointer):pointer;cdecl;
  //
  //{ alleen Lib datablokken  }
  //procedure newstructname(bh:PBHead);cdecl;
  //
  //function openblenderfile(name:Pchar; filelen:Plongint):^char;cdecl;
  //
  //procedure read_autosavefile;cdecl;
  //
  //procedure read_file(dir:Pchar);cdecl;
  //
  //function read_file_dna(filedata:Pchar; filelen:longint):longint;cdecl;
  //
  //function read_homefile:longint;cdecl;
  //
  //function read_libblock(main:PMain; bhead:PBHead; flag:longint):longint;cdecl;
  //
  //procedure read_libraries;cdecl;
  //
  //function read_libstruct(bh:PBHead):pointer;cdecl;
  //
  //procedure read_struct(bh:PBHead);cdecl;
  //
  //procedure splitdirstring(di:Pchar; fi:Pchar);cdecl;
  //
  //procedure switch_endian_bheads(filedata:Pchar; filelen:longint);cdecl;
  //
  //procedure switch_endian_structs(bhead:PBHead);cdecl;
  //
  //function testextensie(str:Pchar; ext:Pchar):longint;cdecl;
  //
  //procedure vcol_to_fcol(me:PMesh);cdecl;
  //
  //{ ******* writefile.c  }
  //procedure add_cam_info(mode:smallint);cdecl;
  //
  //{ alleen current camera  }
  //procedure add_network_info(mode:smallint);cdecl;
  //
  //{ alleen met CFRA  }
  //function add_scal_info(ob:PObject; mode:smallint):longint;cdecl;
  //
  //{ return OK  }
  //procedure delete_autosave;cdecl;
  //
  //procedure do_history(name:Pchar);cdecl;
  //
  //procedure bgnwrite(file:longint);cdecl;
  //
  //procedure endwrite;cdecl;
  //
  //procedure flushwrite;cdecl;
  //
  //procedure initrender_infofiles;cdecl;
  //
  //procedure make_mdec_movies;cdecl;
  //
  //procedure mdec_to_pupstring(str:PPchar);cdecl;
  //
  //procedure readBlog;cdecl;
  //
  //procedure remove_mdec_chunks(id:longint);cdecl;
  //
  //procedure test_Mdec_movie(name:Pchar);cdecl;
  //
  //procedure writeBlog;cdecl;
  //
  //procedure write_file(dir:Pchar);cdecl;
  //
  //function write_homefile:longint;cdecl;
  //
  //procedure writedata(filecode:longint; len:longint; adr:pointer);cdecl;
  //
  //{ geen struct  }
  //procedure writestruct(filecode:longint; structname:Pchar; nr:longint; adr:pointer);cdecl;
  //
//{$endif}
  { FILE_H  }

implementation

  //{ was #define dname def_expr }
  //function FORM : longint; { return type might be wrong }
  //  begin
  //    FORM:=MAKE_ID('F','O','R','M');
  //  end;
  //
  //{ was #define dname def_expr }
  //function DDG1 : longint; { return type might be wrong }
  //  begin
  //    DDG1:=MAKE_ID('3','D','G','1');
  //  end;
  //
  //{ was #define dname def_expr }
  //function DDG2 : longint; { return type might be wrong }
  //  begin
  //    DDG2:=MAKE_ID('3','D','G','2');
  //  end;
  //
  //{ was #define dname def_expr }
  //function DDG3 : longint; { return type might be wrong }
  //  begin
  //    DDG3:=MAKE_ID('3','D','G','3');
  //  end;
  //
  //{ was #define dname def_expr }
  //function DDG4 : longint; { return type might be wrong }
  //  begin
  //    DDG4:=MAKE_ID('3','D','G','4');
  //  end;
  //
  //{ was #define dname def_expr }
  //function GOUR : longint; { return type might be wrong }
  //  begin
  //    GOUR:=MAKE_ID('G','O','U','R');
  //  end;
  //
  //{ was #define dname def_expr }
  //function BLEN : longint; { return type might be wrong }
  //  begin
  //    BLEN:=MAKE_ID('B','L','E','N');
  //  end;
  //
  //{ was #define dname def_expr }
  //function DER_ : longint; { return type might be wrong }
  //  begin
  //    DER_:=MAKE_ID('D','E','R','_');
  //  end;
  //
  //{ was #define dname def_expr }
  //function V100 : longint; { return type might be wrong }
  //  begin
  //    V100:=MAKE_ID('V','1','0','0');
  //  end;
  //
  //{ was #define dname def_expr }
  //function DATA : longint; { return type might be wrong }
  //  begin
  //    DATA:=MAKE_ID('D','A','T','A');
  //  end;
  //
  //{ was #define dname def_expr }
  //function GLOB : longint; { return type might be wrong }
  //  begin
  //    GLOB:=MAKE_ID('G','L','O','B');
  //  end;
  //
  //{ was #define dname def_expr }
  //function IMAG : longint; { return type might be wrong }
  //  begin
  //    IMAG:=MAKE_ID('I','M','A','G');
  //  end;
  //
  //{ was #define dname def_expr }
  //function DNA1 : longint; { return type might be wrong }
  //  begin
  //    DNA1:=MAKE_ID('D','N','A','1');
  //  end;
  //
  //{ was #define dname def_expr }
  //function TEST : longint; { return type might be wrong }
  //  begin
  //    TEST:=MAKE_ID('T','E','S','T');
  //  end;
  //
  //{ was #define dname def_expr }
  //function REND : longint; { return type might be wrong }
  //  begin
  //    REND:=MAKE_ID('R','E','N','D');
  //  end;
  //
  //{ was #define dname def_expr }
  //function USER : longint; { return type might be wrong }
  //  begin
  //    USER:=MAKE_ID('U','S','E','R');
  //  end;
  //
  //{ was #define dname def_expr }
  //function ENDB : longint; { return type might be wrong }
  //  begin
  //    ENDB:=MAKE_ID('E','N','D','B');
  //  end;


end.
