unit blenglob;

interface

uses
blender, blendef,
screen, space, util;

type
  pGlobal = ^Global;
  Global = record
    vd : pView3D;
    v2d : pView2D;
    sipo : pSpaceIpo;
    buts : pSpaceButs;
    sima : pSpaceImage;
    soops : pSpaceOops;
    stext : pSpaceText;
    main : pMain;
    scene : pScene;
    curscreen : pbScreen;
    obedit : pObject;
    font : pointer;
    fonts : pointer;
    fontss : pointer;
    mainbase : ListBase;
    ima : array[0..159] of char;
    sce : array[0..159] of char;
    lib : array[0..159] of char;
    psx : array[0..159] of char;
    totobj : smallint;
    totlamp : smallint;
    totobjsel : smallint;
    totcurve : smallint;
    totmesh : smallint;
    totmat : smallint;
    totvert : longint;
    totface : longint;
    totvertsel : longint;
    totfacesel : longint;
    time : longint;
    cputime : longint;
    machine : smallint;
    afbreek : smallint;
    f : smallint;
    moving : smallint;
    colact : smallint;
    zbuf : smallint;
    qual : smallint;
    background : smallint;
    imagewin : smallint;
    animspeed : smallint;
    version : smallint;
    versionfile : smallint;
    simulf : smallint;
    fields : smallint;
    order : smallint;
    rt : smallint;
    edve : ListBase;
    eded : ListBase;
    edvl : ListBase;
    textcurs : array[0..3] of array[0..1] of single;
    //cursector : pSector;
    //sectorbuf : array[0..(SE_MAXBUF)-1] of pSector;
    lifebuf : array[0..(LF_MAXBUF)-1] of pObject;
    actvarar : psmallint;
    dfra : longint;
    dfrao : longint;
    dfras : smallint;
    totsect : smallint;
    maxsect : smallint;
    totlife : smallint;
    renderd : longint;
    real_sfra : longint;
    real_efra : longint;
    save_over : longint;
    special1 : smallint;
    special2 : smallint;
  end;

var
  G: Global;

implementation

end.
