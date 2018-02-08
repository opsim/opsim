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

  { flags voor wegschrijven/lezen scene  }

  { INTEGER CODES  }

  //{ was #define dname def_expr }
  //function FORM : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function DDG1 : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function DDG2 : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function DDG3 : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function DDG4 : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function GOUR : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function BLEN : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function DER_ : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function V100 : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function DATA : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function GLOB : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function IMAG : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function DNA1 : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function TEST : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function REND : longint; { return type might be wrong }

  //{ was #define dname def_expr }
  //function USER : longint; { return type might be wrong }

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

  pBHead = ^BHead;
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

  //procedure make_structDNA;

  //function findstruct_name(sdna:PSDNA; str:Pchar):psmallint;

  //function findstruct_nr(sdna:PSDNA; str:Pchar):longint;

  //function struct_compare(stype:Pchar; one:pointer; two:pointer):longint;

  //  var
  //    cur_sdna : SDNA;cvar;external;

  //function text_to_curve(ob:PObject; mode:longint):^chartrans;

  //procedure freestructDNA(sdna:PSDNA);

  //procedure freestructDNA_all;

  //procedure init_structDNA(sdna:PSDNA);

  //procedure reconstruct(oldSDNAnr:longint; blocks:longint; data:Ppointer);

  //procedure switch_endian_struct(oldSDNAnr:longint; data:Pchar);

  //{ readfile.c  }
  //function gethome:^char;

  //procedure add_data_adr(old:pointer; _new:pointer);

  //procedure add_lib_adr(old:pointer; _new:pointer);

  //procedure append_id_part(filedata:Pchar; main:PMain; id:PID);

  //procedure append_named_part(sfile:PSpaceFile; main:PMain; name:Pchar; idcode:longint);

  //procedure change_libadr(old:pointer; _new:pointer);

  //function convertstringcode(str:Pchar):longint;

  //procedure do_versions(main:PMain);

  //function find_bhead(old:pointer; filedata:Pchar):^BHead;

  //procedure give_base_to_objects(sce:PScene; lb:PListBase);

  //procedure inst_file(filename:Pchar; data:Pchar; size:longint);

  //function is_yet_read(main:PMain; bhead:PBHead):^ID;

  //procedure library_append(sfile:PSpaceFile);

  //{ append aan G
  //extern    void link_global(FileGlobal *fg);
  //extern    void link_list(ListBase *lb);		/* alleen direkte data  }
  //procedure link_recurs_seq(lb:PListBase);

  //procedure makestringcode(str:Pchar);

  //function mallocNN(len:longint; str:Pchar):pointer;

  //function newadr(adr:pointer):pointer;

  //{ alleen direkte datablokken  }
  //function newlibadr(lib:pointer; adr:pointer):pointer;

  //{ alleen Lib datablokken  }
  //function newlibadr_us(lib:pointer; adr:pointer):pointer;

  //{ hoogt usernummer op  }
  //function newlibadr_us_type(_type:smallint; adr:pointer):pointer;

  //{ alleen Lib datablokken  }
  //procedure newstructname(bh:PBHead);

  //function openblenderfile(name:Pchar; filelen:Plongint):^char;

  //procedure read_autosavefile;

  //procedure read_file(dir:Pchar);

  //function read_file_dna(filedata:Pchar; filelen:longint):longint;

  //function read_homefile:longint;

  //function read_libblock(main:PMain; bhead:PBHead; flag:longint):longint;

  //procedure read_libraries;

  //function read_libstruct(bh:PBHead):pointer;

  //procedure read_struct(bh:PBHead);

  //procedure splitdirstring(di:Pchar; fi:Pchar);

  //procedure switch_endian_bheads(filedata:Pchar; filelen:longint);

  //procedure switch_endian_structs(bhead:PBHead);

  //function testextensie(str:Pchar; ext:Pchar):longint;

  //procedure vcol_to_fcol(me:PMesh);

  //{ ******* writefile.c  }
  //procedure add_cam_info(mode:smallint);

  //{ alleen current camera  }
  //procedure add_network_info(mode:smallint);

  //{ alleen met CFRA  }
  //function add_scal_info(ob:PObject; mode:smallint):longint;

  //{ return OK  }
  //procedure delete_autosave;

  //procedure do_history(name:Pchar);

  //procedure bgnwrite(file:longint);

  //procedure endwrite;

  //procedure flushwrite;

  //procedure initrender_infofiles;

  //procedure make_mdec_movies;

  //procedure mdec_to_pupstring(str:PPchar);

  //procedure readBlog;

  //procedure remove_mdec_chunks(id:longint);

  //procedure test_Mdec_movie(name:Pchar);

  //procedure writeBlog;

  //procedure write_file(dir:Pchar);

  //function write_homefile:longint;

  //procedure writedata(filecode:longint; len:longint; adr:pointer);

  //{ geen struct  }
  //procedure writestruct(filecode:longint; structname:Pchar; nr:longint; adr:pointer);

//{$endif}
  { FILE_H  }

implementation

  //{ was #define dname def_expr }
  //function FORM : longint; { return type might be wrong }
  //  begin
  //    FORM:=MAKE_ID('F','O','R','M');
  //  end;

  //{ was #define dname def_expr }
  //function DDG1 : longint; { return type might be wrong }
  //  begin
  //    DDG1:=MAKE_ID('3','D','G','1');
  //  end;

  //{ was #define dname def_expr }
  //function DDG2 : longint; { return type might be wrong }
  //  begin
  //    DDG2:=MAKE_ID('3','D','G','2');
  //  end;

  //{ was #define dname def_expr }
  //function DDG3 : longint; { return type might be wrong }
  //  begin
  //    DDG3:=MAKE_ID('3','D','G','3');
  //  end;

  //{ was #define dname def_expr }
  //function DDG4 : longint; { return type might be wrong }
  //  begin
  //    DDG4:=MAKE_ID('3','D','G','4');
  //  end;

  //{ was #define dname def_expr }
  //function GOUR : longint; { return type might be wrong }
  //  begin
  //    GOUR:=MAKE_ID('G','O','U','R');
  //  end;

  //{ was #define dname def_expr }
  //function BLEN : longint; { return type might be wrong }
  //  begin
  //    BLEN:=MAKE_ID('B','L','E','N');
  //  end;

  //{ was #define dname def_expr }
  //function DER_ : longint; { return type might be wrong }
  //  begin
  //    DER_:=MAKE_ID('D','E','R','_');
  //  end;

  //{ was #define dname def_expr }
  //function V100 : longint; { return type might be wrong }
  //  begin
  //    V100:=MAKE_ID('V','1','0','0');
  //  end;

  //{ was #define dname def_expr }
  //function DATA : longint; { return type might be wrong }
  //  begin
  //    DATA:=MAKE_ID('D','A','T','A');
  //  end;

  //{ was #define dname def_expr }
  //function GLOB : longint; { return type might be wrong }
  //  begin
  //    GLOB:=MAKE_ID('G','L','O','B');
  //  end;

  //{ was #define dname def_expr }
  //function IMAG : longint; { return type might be wrong }
  //  begin
  //    IMAG:=MAKE_ID('I','M','A','G');
  //  end;

  //{ was #define dname def_expr }
  //function DNA1 : longint; { return type might be wrong }
  //  begin
  //    DNA1:=MAKE_ID('D','N','A','1');
  //  end;

  //{ was #define dname def_expr }
  //function TEST : longint; { return type might be wrong }
  //  begin
  //    TEST:=MAKE_ID('T','E','S','T');
  //  end;

  //{ was #define dname def_expr }
  //function REND : longint; { return type might be wrong }
  //  begin
  //    REND:=MAKE_ID('R','E','N','D');
  //  end;

  //{ was #define dname def_expr }
  //function USER : longint; { return type might be wrong }
  //  begin
  //    USER:=MAKE_ID('U','S','E','R');
  //  end;

  //{ was #define dname def_expr }
  //function ENDB : longint; { return type might be wrong }
  //  begin
  //    ENDB:=MAKE_ID('E','N','D','B');
  //  end;

end.
