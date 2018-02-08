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
(* usiblender.c  jan 94  GRAPHICS
 * 
 * 
 * 
 *)

program usiblender;

uses
GL, GLU, GLUT,
screen, blender, util, blenglob, cfuncs, blendef, graphics,
drawobject, drawview, mywindow, Button, scene, drawscene,
mydevice, toolbox, font;

var
bprogname: array [0..127] of char;
//U: UserDef;  moved to blender

procedure exit_usiblender;
begin
//  {fsmenu: pchar; }{<= !!!5 external variable}(* filesel.c *)
//  if G.background)and(G.scene then
//  begin 
//    (* network patch *)
//  end_camera_network(); 
//end_sectors(); 
//end;
//freeAllRad(); 
//if G.obedit<>nil then
//begin 
//  if G.obedit^._type=OB_FONT then
//  begin 
//    free_editText(); 
//  end
//  else
//  if G.obedit^._type=OB_MBALL then
//  freelistN( and editelems); 
//  free_editmesh(); 
//end;
//free_editLatt(); 
//free_blender(); 
//free_hashedgetab(); 
//(* blender.c, doet hele library *)
//FreeButs(0); 
//free_ipocopybuf(); 
//freefastshade(); 
//free_vertexpaint(); 
//freeNurblist( and editNurb); 
//(* editnurb kan blijven bestaan buiten editmode *)
//if fsmenu<>nil then
//freeN(fsmenu); 
//free_render_data(); 
//free_filt_mask(); 
//if txt_cut_buffer<>nil then
//freeN(txt_cut_buffer); 
//end_python(); 
//if totblock<>0 then
//begin 
//  printf('Error Totblck: %d\n',totblock);
//  printmemlist(); 
//end;
//delete_autosave(); 
printf('Blender quit\n');
//exit(G.afbreek=1);
end;

var
map: array [0..31] of word;

procedure initcursors;
var
a: smallint;
begin
(* cursor 1= kruisje
 * cursor 2= wait
 * cursor 3= vpaint
 * cursor 4= faceselect
 *)

  for a:=0 to 31 do
  map[a]:= 0;

  (* wait *)
  for a:=0 to 15 do
  map[a]:= $FFFF;

  (* kruisje *)
  for a:=0 to 15 do
  map[a]:= 256+128;
  map[6]:= 0;
  map[7]:= $FFFF-512-256-128-64;
  map[8]:= map[7];
  map[9]:= 0;

  (* vpaint *)
  (* facesel *)
  for a:=0 to 15 do
  map[a]:= 256+128;
  for a:=16 to 31 do
  map[a]:= 256+128;
  map[6]:= 0;
  map[7]:= $FFFF-512-256-128-64;
  map[8]:= map[7];
  map[9]:= 0;
  map[6+16]:= 0;
  map[7+16]:= $FFFF-512-256-128-64;
  map[8+16]:= map[7+16];
  map[9+16]:= 0;
end;

procedure initbuttons;
var
  //bbuf, pbuf: pImBuf;
helvfont: pinteger;
str: array [0..255] of char;
begin
  G.font:= GLUT_BITMAP_HELVETICAB_12;
  G.fonts:= GLUT_BITMAP_HELVETICAB_10;
  G.fontss:= GLUT_BITMAP_HELVETICAB_8;

  AutoButFontSize(G.fonts, G.fontss);

  (* colors for headerbuttons *)
  DefButCol(1, OVERDRAW, 0, 1,4,3,1, 1,1,1,1);
  DefButCol(2, RGBDRAW, $909090, $A0A0A0, 0, $FFFFFF, 0);	(* grijs *)
  DefButCol(3, RGBDRAW, $909090, $80A080, 0, $FFFFFF, 0);	(* groen *)
  DefButCol(4, RGBDRAW, $909090, $A08080, 0, $FFFFFF, 0);	(* blauw *)
  DefButCol(5, RGBDRAW, $909090, $80A0B0, 0, $FFFFFF, 0);	(* zalm *)
  DefButCol(6, RGBDRAW, $909090, $808080, 0, $FFFFFF, 0);	(* middelgrijs *)
  DefButCol(7, RGBDRAW, $909090, $4040B0, 0, $0000FF, 0);	(* red alert *)

  (* colors for blenderbuttons *)
  DefButCol(10, RGBDRAW, $606060, $909090, 0, $FFFFFF, 0);	(* grijs *)
  DefButCol(11, RGBDRAW, $606060, $949070, 0, $FFFFFF, 0);	(* groen *)
  DefButCol(12, RGBDRAW, $606060, $A09090, 0, $FFFFFF, 0);	(* blauw *)
  DefButCol(13, RGBDRAW, $606060, $8090A0, 0, $FFFFFF, 0);	(* zalm *)
  DefButCol(14, RGBDRAW, $607070, $998892, 0, $FFFFFF, 0);	(* paars *)

  (* IKONEN INLADEN *)

  //bbuf:= loadiffmem(datatoc_blenderbuttons, LI_rect);

  //if bbuf=nil then
  //begin
  //  printf('Can''t load buttonimage\n');
  //  exit(0);
  //end;

  //DefButIcon(0,bbuf.rect,bbuf.x,bbuf.y,20,21);
  //bbuf.rect:= nil;
  //freeImBuf(bbuf);

  //ipofont:= GLUT_BITMAP_HELVETICA_10;
  //clear_matcopybuf();
end;

procedure initpatterns;
var
pat: array [0..15] of word;
x: word;
begin

end;

var
count: integer = 0;

procedure breekaf(sig: integer);
begin
  G.afbreek:= 2;

  if sig=2 then
  begin
    if G.renderd=0 then
    begin
      if count<>0 then
      halt(2);
      printf('Press ^C again if once doesn''t work\n');
    end
    else
    begin
      if count=20 then
      halt(2);
    end;
    inc(count);
  end;
end;

procedure savecore;
//var
//scestr: array [0..Pred(FILE_MAXDIR)] of char; 
//tstr: array [0..Pred(FILE_MAXFILE)] of char; 
begin
//  {noBlog: integer; }{<= !!!5 external variable}

//  procedure write_file; 
//  noBlog:= 1; 
//  strcpy(scestr,G.sce); 
//  G.f:= G.f or (G_DISABLE_OK); 
//  write_file('/usr/tmp/core.blend'); 
//  (* even bewaren *)
//  G.f:= G.f and ( not G_DISABLE_OK); 
//  strcpy(G.sce,scestr); 
//  noBlog:= 0; 
end;

procedure fpe_handler(sig: integer);
begin
  printf('SIGFPE trapped\n');
end;

var
  sce: pScene;
  a: integer;
  stax: integer;
  stay: integer;
  sizx: integer;
  sizy: integer;
  cp: pchar;
  tstr: array [0..Pred(100)] of char;
  dofork: smallint = 1;

(* deze mainfunktie alleen voor editor, background krijgt andere *)
begin
  {$ifdef FREEBSD}
  fpsetmask(0);
  {$endif}
  {$if defined(__sgi)}
  signal(SIGFPE,fpe_handler);
  {$endif}

  (* voor play anim *)
  strcpy(bprogname,argv[0]);

  initglobals();      (* blender.c *)

  (* eerste testen op background *)
  G.f:= G.f or G_SCENESCRIPT;

  a := 1;
  while a < argc do
  begin
    if argv[a][0]='-' then
    begin
      case argv[a][1] of
        'a':
        begin
          //playanim(argc-1,argv+1);
          halt(0);
        end;
        'b':
        begin
          if strcmp(argv[a],'-badzr252')=0 then
          begin
          //  {badzr252: integer; }{<= !!!5 external variable}
          //  badzr252:= 1;
          //    {!!!7 possible troubles with "for" =>}
          //    continue
          end;
          (* background met lage prioriteit *)

          {$ifdef __sgi}
          if schedctl(NDPRI,0,NDPLOMIN-2)=-1 then
          printf('no ndpri \n');
          (*if (schedctl(SLICE, 0, 1) == -1) printf("no slice \n");*)
          schedctl(SLICE,0,1);
          {$endif}
        end;
        'B':
        begin
        (* background met normale prioriteit *)

          G.background:= 1;
          a:= argc;
        end;
        'y':
        begin
          G.f:= G.f and ( not G_SCENESCRIPT);
        end;
        'Y':
        begin
  	printf ('-y was used to disable scene scripts because,\n');
  	printf ('\t-p being taken, Ton was of the opinion that Y\n');
  	printf ('\tlooked like a split (disabled) snake, and also\n');
  	printf ('\twas similar to a python''s tongue (unproven).\n\n');

  	printf ('\tZr agreed because it gave him a reason to add a\n');
  	printf ('\tcompletely useless text into Blender.\n\n');

  	printf ('\tADDENDUM! Ton, in defense, found this picture of\n');
  	printf ('\tan Australian python, exhibiting her (his/its) forked\n');
  	printf ('\tY tongue. It could be part of an H Zr retorted!\n\n');
  	printf ('\thttp://www.users.bigpond.com/snake.man/\n');

          halt(252);
        end;
        'h':
        begin
	printf ('Blender V %d.%d\n', [G.version/100, G.version mod 100]);
	printf ('Usage: blender [options ...] [file]\n');

	printf ('\nRender options:\n');
	printf ('  -b <file>\tRender <file> in background\n');
	printf ('    -S <name>\tSet scene <name>\n');
	printf ('    -f <frame>\tRender frame <frame> and save it\n');
	printf ('    -s <frame>\tSet start to frame <frame> (use with -a)\n');
	printf ('    -e <frame>\tSet end to frame (use with -a)<frame>\n');
	printf ('    -a\t\tRender animation\n');

	printf ('\nAnimation options:\n');
	printf ('  -a <file(s)>\tPlayback <file(s)>\n');
	printf ('    -m\t\tRead from disk (Don''t buffer)\n');

	printf ('\nWindow options:\n');
	printf ('  -w\t\tForce opening with borders\n');
{$ifdef WINDOWS}
	printf ('  -W\t\tForce opening without borders\n');
{$endif}
	printf ('  -p <sx> <sy> <w> <h>\tOpen with lower left corner at <sx>, <sy>\n');
	printf ('                      \tand width and height <w>, <h>\n');

	printf ('\nMisc options:\n');
	printf ('  -f\t\tPrevent forking in foreground mode\n');
	printf ('  -d\t\tTurn debugging on\n');
	printf ('  -h\t\tPrint this help text\n');
	printf ('  -y\t\tDisable OnLoad scene scripts, use -Y to find out why its -y\n');

        halt(0);
        end;
      end;
    end;
    inc(a);
  end;

  {$ifdef __sgi}

  	setuid(getuid()); (* einde superuser *)

  	(* Signalen zetten. Even wachten met USR1 en INT, want die kunnen
  	 * ook het lezen van de file onderbreken. Geeft van die slordige
  	 * foutmeldingen....
  	 *)

  	sigset(SIGUSR2, savecore);	(* als er een SIGUSR2 gegeven word dump dan de file *)
  	sighold(SIGUSR1);
  	sighold(SIGINT);
  	sigset(SIGUSR1, breekaf);   (* nette kill gebruikt door [traces|render]daemon *)
  	sigset(SIGINT, breekaf);   (* nette kill gebruikt door [traces|render]daemon *)

  {$endif} (* __sgi *)

  //bzero( @cur_sdna,sizeof(SDNA));
  //cur_sdna.data:= DNAstr+4;
  //cur_sdna.datalen:= *( {pinteger(}DNAstr);
  //init_structDNA( @cur_sdna);

  //init_render_data();   (* moet vooraan staan ivm R.winpos uit defaultfile *)

  if G.background=0 then
  begin
    a:= 1;   (* patch zodat glutinit van de argumenten afblijft *)
    glutInit(@argc,argv);

    getdisplaysize();

    a:=1;
    while a < argc do
    begin
      if argv[a][0]='-' then
      begin
        case argv[a][1] of
          'p':       (* prefsize *)
           begin
            if argc-a<5 then
            begin
              printf('-p requires four arguments\n');
              halt(1);
            end;
            inc(a);
            stax:= atoi(argv[a]);
            inc(a);
            stay:= atoi(argv[a]);
            inc(a);
            sizx:= atoi(argv[a]);
            inc(a);
            sizy:= atoi(argv[a]);

            setprefsize(stax,stay,sizx,sizy);
            fullscreen:= 0;
            (* a= argc; *)
          end;
          'd':
          begin
            G.f:= G.f or G_DEBUG;   (* std output printf's *)
            printf('Blender V %d.%d\n',[G.version div 100,G.version mod 100]);
          end;
          'f':
          begin
            argv[a][1]:= 'F'; (* bypass frame rendering *)
            dofork:= 0;
          end;
          'w':
          begin
            borderless:= 0;
          end;
          {$ifdef WINDOWS}
          'W':
          begin
            //forceborderless:= 1;
          end;
           {$endif}
        end;
      end;
      inc(a);
    end;

    (* fork voordat X opstart *)
    {$if not defined(BEOS) and not defined(WINDOWS) and not defined(MESA31)}
    if dofork<>0 then
      if fork() then
        exit(0);
    {$endif}

    //read_key();     (* has debug mode! *)
    //start_python();

    initpatterns();
    initscreen();
    initcursors();  (* voor (visuele) snelheid, dit eerst, dan setscreen *)
    init_screen_cursors();
    initbuttons();
    start_autosave(0);
    init_draw_rects();  (* drawobject.c *)
    init_gl_stuff();    (* drawview.c *)
    myContextSetup();   (* mywindow.c *)
    //glutDrawInit();

    //read_homefile();
    //load_firstfont();
    //if G.main^.vfont.first=nil then
    //begin
    //  printf('can''t find default vectorfont\n');
    //  halt(0);
    //end;

    //readBlog();
    strcpy(G.lib,G.sce);
  end
  else
  begin
//    read_key(); 
//    start_python(); 
  end;

//  init_filt_mask();

  (* OK we zijn er klaar voor *)

  {$ifdef __sgi}
  sigrelse(SIGUSR1);
  sigrelse(SIGINT);
  {$endif}

  a:=1;
  while a < argc  do
  begin
    if G.afbreek=1 then
    break;

    if argv[a][0]='-' then
    begin
      case argv[a][1] of
        'p':   (* prefsize *)
        begin
          a:= a + 4;
        end;
        'I':
        (* lees standard in voor nieuwe frames *)
        (* gets geeft error op PC *)
        begin
        {$ifdef __sgi}
          if G.scene<>nil then
          begin
            G.renderd:= 1;
            G.real_sfra:= SFRA;
            G.real_efra:= EFRA;
            printf('blender: reading stdin\n');
            fflush(stdout);
            while gets(tstr)
            do
            begin
              if G.afbreek=1 then
              break; {<= !!!b possible in "switch" - then remove this line}
              if lstrcmp(tstr,'EXIT')=0 then
              begin
                EFRA:= G.real_efra;
                SFRA:= G.real_sfra;
                exit_render_stuff();
              end;
              else
              begin
                SFRA:= atoi(tstr);
                EFRA:= SFRA;
                animrender();
              end;
              fflush(stdout);
              (* zorg ervoor dat alle printf's aangekomen zijn *)
              fflush(stderr);
              usleep(100);
              if G.afbreek=1 then
              break; {<= !!!b possible in "switch" - then remove this line}
              (* zend signaal naar ouder dat ik klaar ben *)
              if getppid() then
              kill(getppid(),SIGUSR1);
            end;
          end;
          {$endif}
        end;
        'f':
        begin
          inc(a);
          if G.scene<>nil then
          begin
            //G.real_sfra:= SFRA;
            //G.real_efra:= EFRA;
            //SFRA:= atoi(argv[a]);
            //EFRA:= SFRA;
            //animrender();
          end;
        end;
        'a':
        begin
          if G.scene<>nil then
          begin
            //G.real_sfra:= SFRA;
            //G.real_efra:= EFRA;
            //animrender();
            //exit_render_stuff();
          end;
        end;
        'S':
        begin
          //inc(a);
          //if a<argc then
          //  set_scene_name(argv[a]);
        end;
        's':
        begin
          //inc(a);
          //if G.scene<>nil then
          //begin
          //  if a<argc then
          //  SFRA:= atoi(argv[a]);
          //end;
        end;
        'e':
        begin
          //inc(a);
          //if G.scene<>nil then
          //begin
          //  if a<argc then
          //  EFRA:= atoi(argv[a]);
          //end;
        end;
      end;
    end;
    //else
    //  read_file(argv[a]);

    inc(a);
  end;

  if G.background<>0 then
    exit_usiblender();
  setscreen(G.curscreen);

  //if G.main^.scene.first=nil then
  //begin
  //  sce:= add_scene('1');
  //  set_scene(sce);
  //end;

  qenter(Q_FIRSTTIME,1);

  {$ifdef WINDOWS}
  if glutGet(GLUT_WINDOW_DEPTH_SIZE)<=8 then
  begin
    error('Blender requires more than 256 colors to run!');
    halt(1);
  end;
  {$endif}

  //DB: debug
  printmemlist;

  glutMainLoop();

  halt(0);
end.
