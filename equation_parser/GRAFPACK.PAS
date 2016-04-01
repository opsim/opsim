unit grafpack;
{$F+}
interface

uses
{$IFDEF DPMI}
  Dos,Crt,Graph, WinAPI,Realtype;
{$ELSE}
  Dos,Crt,Graph,Realtype;
{$ENDIF}
type
   D3World=record
           xw1,xw2,yw1,yw2,zw1,zw2:float;
           end;

var
  TheWorld:D3world;
  xwrot,zwrot:integer;
  basex,basey,basez,frontx,fronty,frontz,viewdist:float;
 Graphdriver,Graphmode,XTextglb,YTextglb,VESA16,xw1glb,xw2glb,yw1glb,
 yw2glb:integer;
 charfeedglb,linefeedglb,lineshiftglb:byte;
 Graphmodeglb,VesaGlb:boolean;
 OldOutput : Text;
 xaglb,xscaleglb,yaglb,yscaleglb:float;

Procedure InitGraphic(PathToDriver:string);
{Initializes graphics, Redirects the Write and GoToXY-procedures to
work on the graphics screen.}

Procedure LeaveGraphic;
{Restores Crt-mode, leaves graphics mode. Use if you want to
switch between the two modes in one program. Before final termination
you also have to use the CloseGraph-command from the Graph-Unit.}

Procedure EnterGraphic;
{Switches from Crt-Mode to graphics-mode, InitGraphic must be called
once before.}

procedure GotoXY(X, Y : integer);
{ Set the text position }

procedure setwindow(x1,y1,size:word);
{ Defines drawing area; (x1,y1) is upper left point in *text coordinates*,
 size is the *vertical* extension of the window in textlines. The window
 always comes out square. (Roughly)}

procedure setd3world(x1,y1,z1,x2,y2,z2,vdist:float;xrot,zrot:integer);
{defines what area of the "real" 3-d-world should be drawn, from what
distance it should be viewed(vdist) and what angles the camera has
with the x and z-axes (xrot,zrot). The 3d-world is always mapped into a
cube with length 2 in each direction that the camera moves around of,
looking into the center of the cube. It has a fixed viewing angle
(it's an older model with a fixed focal distance). The cube is then
projected to the window defined by setwindow. All drawing commands
are then in terms of 3-D-world coordinates.}

procedure rotatex(theta:integer);

procedure rotatez(theta:integer);

procedure zoomin;

procedure zoomout;

procedure d3drawpoint(x,y,z:float);

procedure d3line(xl1,yl1,zl1,xl2,yl2,zl2:float);

procedure drawd3axes(c1,c2,c3:string);

{self explaining, the rest}
Implementation

function XTextpixel(Xtextglb:byte):word;
begin
  XTextpixel:=(XTextglb-1)*Charfeedglb;
end;

function YTextpixel(Ytextglb:byte):word;
begin
  YTextpixel:=(YTextglb-1)*linefeedglb+lineshiftglb;
end;

var xchar,ychar:word;

procedure DC(c:byte);
var viewport:viewporttype; x,y:word;
begin
  getviewsettings(viewport);
  x:=xtextpixel(xtextglb); y:=ytextpixel(ytextglb);
  setviewport(x,y,x+xchar,y+ychar,true);
  clearviewport;
  outtextxy(0,0,chr(c));
  with viewport do setviewport(x1,y1,x2,y2,clip);
end;

function WriteGrafChars(var F : TextRec) : integer;
{ Used to output graphics characters through the standard output channel. }
const
  BackSpace = #8;
  LineFeed  = #10;
  Return    = #13;
var
  I : integer;
begin
  with F do
    if Mode = fmOutput then
    begin
      if BufPos > BufEnd then
      begin
        for I := BufEnd to Pred(BufPos) do  { Flush the output buffer }
        begin
          case BufPtr^[I] of
            BackSpace : if XTextGlb > 1 then
                          DEC(XTextGlb);

            LineFeed  : if YTextGlb < 25 then
                          INC(YTextGlb);

            Return    : XTextGlb := 1;
          else
            DC(ORD(BufPtr^[I]));
            if XTextGlb < 80 then
              INC(XTextGlb);
          end; { case }
        end; { for }
      end;
      BufPos := BufEnd;
    end; { if }
  WriteGrafChars := 0;
end; { WriteGrafChars }

function GrafCharZero(var F : TextRec) : integer;
{ Called when standard output is opened and closed }
begin
  GrafCharZero := 0;
end; { GrafCharZero }


procedure GrafCharsON;
{ Redirects standard output to the WriteGrafChars function. }
begin
  Move(Output, OldOutput, SizeOf(Output));  { Save old output channel }
  with TextRec(Output) do
  begin
    OpenFunc:=@GrafCharZero;       { no open necessary }
    InOutFunc:=@WriteGrafChars;    { WriteGrafChars gets called for I/O }
    FlushFunc:=@WriteGrafChars;    { WriteGrafChars flushes automatically }
    CloseFunc:=@GrafCharZero;      { no close necessary }
    Name[0]:=#0;
  end;
end; { GrafCharsON }

procedure GrafCharsOFF;
{ Restores original output I/O channel }
begin
  Move(OldOutput, Output, SizeOf(OldOutput));
end; { GrafCharsOFF }

procedure GotoXY{(X, Y : integer)};
{ Set the text position }
begin
  if (X >= 1) and (X <= 80) and    { Ignore illegal values }
     (Y >= 1) and (Y <= 25) then
  begin
    if GraphModeGlb then
      begin
        XTextGlb := X;      { Set text postion in graphics mode }
        YTextGlb := Y;
      end
    else
      Crt.GotoXY(X, Y);     { Set cursor position in text mode }
  end;
end; { GotoXY }


type
  VgaInfoBlock = record
    VESASignature: array[0..3] of Byte;
    VESAVersion: Word;
    OEMStringPtr: Pointer;
    Capabilities: array[0..3] of Byte;
    VideoModePtr: Pointer;
  end;

const
  VESA16Modes: array[0..2] of Word =
    ($0102, $0104, $0106);

{ Scan the supported mode table for the highest mode this card
  will provide
}

function GetHighestCap(Table: Pointer; Modes: Word; Size: Integer): Integer;
  near; assembler;
asm
        XOR     AX,AX
        LES     DI, Table
@@1:
        MOV     SI, Modes
        ADD     SI, Size
        ADD     SI, Size
        MOV     BX, ES:[DI]
        CMP     BX, 0FFFFH
        JE      @@4
        INC     DI
        INC     DI
        MOV     CX,Size
@@2:
        CMP     BX,[SI]
        JZ      @@3
        DEC     SI
        DEC     SI
        LOOP    @@2
@@3:
        CMP     AX,CX
        JA      @@1
        MOV     AX,CX
        JMP     @@1
@@4:
end;

{$IFDEF DPMI}
type
  TRealRegs = record
    RealEDI, RealESI, RealEBP, Reserved, RealEBX,
    RealEDX, RealECX, RealEAX: Longint;
    RealFlags, RealES, RealDS, RealFS, RealGS,
    RealIP, RealCS, RealSP, RealSS: Word;
  end;

function DetectVesa16: Integer; far; assembler;
var
  Segment, Selector, VesaCap: Word;
asm
{$IFOPT G+}
        PUSH    0000H
        PUSH    0100H
{$ELSE}
        XOR     AX,AX
        PUSH    AX
        INC     AH
        PUSH    AX
{$ENDIF}
        CALL    GlobalDosAlloc
        MOV     Segment,DX
        MOV     Selector,AX
        MOV     DI,OFFSET RealModeRegs
        MOV     WORD PTR [DI].TRealRegs.RealSP, 0
        MOV     WORD PTR [DI].TRealRegs.RealSS, 0
        MOV     WORD PTR [DI].TRealRegs.RealEAX, 4F00H
        MOV     WORD PTR [DI].TRealRegs.RealES, DX
        MOV     WORD PTR [DI].TRealRegs.RealEDI, 0
        MOV     AX,DS
        MOV     ES,AX
        MOV     AX,0300H
        MOV     BX,0010H
        XOR     CX,CX
        INT     31H
        MOV     DI,OFFSET RealModeRegs
        MOV     AX,grError
        PUSH    AX
        CMP     WORD PTR [DI].TRealRegs.RealEAX,004FH
        JNZ     @@Exit
        POP     AX
        MOV     ES,Selector
        XOR     DI,DI
        CMP     ES:[DI].VgaInfoBlock.VESASignature.Word[0], 'EV'
        JNZ     @@Exit
        CMP     ES:[DI].VgaInfoBlock.VESASignature.Word[2], 'AS'
        JNZ     @@Exit
        MOV     AX,0000
        MOV     CX,1
        INT     31H
        MOV     VesaCap,AX
        MOV     DX,ES:[DI].VgaInfoBlock.VideoModePtr.Word[2]
        MOV     CX,4
        XOR     AX,AX
@@Convert:
        SHL     DX,1
        RCL     AX,1
        LOOP    @@Convert
        ADD     DX,ES:[DI].VgaInfoBlock.VideoModePtr.Word[0]
        ADC     AX,0
        MOV     CX,AX
        MOV     BX,VesaCap
        MOV     AX,0007H
        INT     31H
        INC     AX
        XOR     CX,CX
        MOV     DX,0FFFFH
        INT     31H
        MOV     ES,BX
        PUSH    ES
        PUSH    DI
{$IFOPT G+}
        PUSH    OFFSET Vesa16Modes
        PUSH    0003H
{$ELSE}
        MOV     SI, OFFSET Vesa16Modes
        PUSH    SI
        MOV     AX, 5
        PUSH    AX
{$ENDIF}
        CALL    GetHighestCap
        PUSH    AX
        MOV     BX,VesaCap
        MOV     AX,0001H
        INT     31H
@@Exit:
        PUSH    Selector
        CALL    GlobalDosFree
        POP     AX
end;
{$ELSE}
function DetectVesa16: Integer; far; assembler;
var
  VesaInfo: array[0..255] of Byte;
asm
        MOV     AX,SS
        MOV     ES,AX
        LEA     DI,VesaInfo
        MOV     AX,4F00H
        INT     10H
        CMP     AX,004FH
        MOV     AX,grError
        JNZ     @@Exit
        CMP     ES:[DI].VgaInfoBlock.VESASignature.Word[0], 'EV'
        JNZ     @@Exit
        CMP     ES:[DI].VgaInfoBlock.VESASignature.Word[2], 'AS'
        JNZ     @@Exit
        LES     DI,ES:[DI].VgaInfoBlock.VideoModePtr
        PUSH    ES
        PUSH    DI
        MOV     AX, OFFSET Vesa16Modes
        PUSH    AX
        MOV     AX,3
        PUSH    AX
        CALL    GetHighestCap
@@Exit:
end;
{$ENDIF}

procedure initgraphic;
var error:word;
begin
  vesaglb:=false;
  VESA16:=InstallUserDriver('VESA16',@DetectVesa16);
  if GraphResult<>0 then begin
    writeln('Error installing Vesa16'); end;
  GraphDriver := Detect;
  InitGraph(GraphDriver, GraphMode, pathtodriver);
  if GraphResult <> grOk then
  begin
    Writeln('Graphics init error: ', GraphErrorMsg(GraphDriver));
    Halt(1);
  end;
  xchar:=7; ychar:=7;
  Case Graphdriver of
    CGA:begin Charfeedglb:=8; Linefeedglb:=8; Lineshiftglb:=0;
              end;
    EGA:begin Charfeedglb:=8;  Linefeedglb:=14;
               lineshiftglb:=3;
               end;
    EGA64:begin Charfeedglb:=8;  Linefeedglb:=14;
                 LineshiftGlb:=3;
                 end;
    EGAMono:begin Charfeedglb:=8;  Linefeedglb:=14;
                   Lineshiftglb:=3;
                   end;
    HercMono:begin Charfeedglb:=9;  Linefeedglb:=14;
                   LineshiftGlb:=3;
                   end;
    VGA:begin Charfeedglb:=8;  Linefeedglb:=19;
              Lineshiftglb:=3;
              end;
  else
    begin
      vesaglb:=true;
      graphmode:=0;
      setgraphmode(graphmode);
      charfeedglb:=10; linefeedglb:=24;
      lineshiftglb:=8; xchar:=7; ychar:=7;
    end;
  end;
  Graphmodeglb:=true;
  Grafcharson;
end;

procedure leavegraphic;
begin
  RestoreCrtMode;
  GraphmodeGlb:=false;
  Grafcharsoff;
end;

procedure entergraphic;
begin
  SetGraphmode(graphmode);
  Graphmodeglb:=true;
  GrafCharsOn;
end;

procedure setwindow;
begin
  xw1glb:=(x1-1)*charfeedglb; yw1glb:=(y1-1)*linefeedglb;
  yw2glb:= yw1glb+size*linefeedglb;
  xw2glb:=xw1glb+round(0.75*getmaxx/getmaxy*(yw2glb-yw1glb));
  setviewport(xw1glb,yw1glb,xw2glb,yw2glb,true);
end;

var thetax,thetaz,sinx,sinz,cosx,cosz:float;
    rightx, rightz:integer;

procedure makeradians;
begin
  thetax:=2*pi*xwrot/360; thetaz:=2*pi*zwrot/360;
  sinx:=sin(thetax); cosx:=cos(thetax);
  sinz:=sin(thetaz); cosz:= cos(thetaz);
  rightx:=(xwrot+90) mod 180;  rightz:=zwrot mod 180;
end;

function scalar(xb,yb,zb:float):float;
begin
  scalar:=yb*sinx*sinz+zb*cosz+xb*sinz*cosx;
end;

procedure initworld;
var umin,umax,vmin,vmax,d2world:float;
    i,j,k:integer;
begin
  makeradians;
  if viewdist<0 then viewdist:=0.00001;
  d2world:=0.25;
  xaglb:=(xw2glb-xw1glb)/2;
  xscaleglb:=(xw2glb-xw1glb)/2/d2world;
  yaglb:=(yw2glb-yw1glb)/2;
  yscaleglb:=(yw2glb-yw1glb)/2/d2world;
end;

procedure setd3world(x1,y1,z1,x2,y2,z2,vdist:float;xrot,zrot:integer);
var d:float;
begin
  with theworld do
  begin
    xw1:=x1;  xw2:=x2;  yw1:=y1; yw2:=y2; zw1:=z1;  zw2:=z2;
  end;
  zwrot:=zrot; xwrot:=xrot;  viewdist:=vdist;
  initworld;
end;

procedure rotatez(theta:integer);
begin
    zwrot:=zwrot+theta;  initworld;
end;

procedure rotatex(theta:integer);
begin
    xwrot:=xwrot+theta;  initworld;
end;

procedure zoomin;
var v:float;
begin
  viewdist:=viewdist-0.05; initworld;
end;

procedure zoomout;
begin
  viewdist:=viewdist+0.05; initworld;
end;

procedure blockx(x:float;var xb:float);
begin
  with TheWorld do
  xb:= -1+2*(x-xw1)/(xw2-xw1);
end;
procedure blocky(y:float;var yb:float);
begin
  with TheWorld do
  yb:= -1+2*(y-yw1)/(yw2-yw1);
end;
procedure blockz(z:float;var zb:float);
begin
  with TheWorld do
  zb:= -1+2*(z-zw1)/(zw2-zw1);
end;

procedure project(xb,yb,zb:float; var u,v:float;var visible:boolean);
var scal,d:float;
begin
  scal:=scalar(xb,yb,zb);
  d:=viewdist-scal;
  if d<=0.1 then visible:=false else
  begin
    if rightz<>0 then
      v:=(zb-scal*cosz)/sinz
    else
      v:=-(yb*sinx+xb*cosx)/cosz;
    if rightx<>0 then
      u:=(yb+sinx*(v*cosz-scal*sinz))/cosx
    else
      u:=-xb*sinx;
    u:=u/d;
    v:=v/d;
    visible:=(abs(u)<10) and (abs(v)<10);
  end;
end;

procedure d3window(x,y,z:float; var xs,ys:integer;var visible:boolean);
var xb,yb,zb,scal,d,u,v:float;
begin
  blockx(x,xb);  blocky(y,yb);  blockz(z,zb);
  project(xb,yb,zb,u,v,visible);
  if visible then
  begin
    xs:=round(u*xscaleglb+xaglb); ys:=round(yaglb-v*yscaleglb);
  end;
end;


procedure d3drawpoint(x,y,z:float);
var xs,ys:integer; var visible:boolean;
begin
  d3window(x,y,z,xs,ys,visible);
  if visible then  putpixel(xs,ys,getcolor);
end;

procedure d3line(xl1,yl1,zl1,xl2,yl2,zl2:float);
var u1,v1,u2,v2:integer; var visible:boolean;
begin
  d3window(xl1,yl1,zl1,u1,v1,visible);
  if visible then
  begin
    d3window(xl2,yl2,zl2,u2,v2,visible);
    if visible then  line(u1,v1,u2,v2);
  end;
end;

procedure drawd3axes(c1,c2,c3:string);

procedure drawoneaxis(x1,y1,z1,x2,y2,z2:float;c:string);
var norms,wx,wy:float;  visible:boolean;
    xs1,ys1,xs2,ys2:integer; vsx,vsy:float;
begin
  d3line(x1,y1,z1,x2,y2,z2);
  d3window(x1,y1,z1,xs1,ys1,visible);
  if visible then
  begin
  d3window(x2,y2,z2,xs2,ys2,visible);
  if visible then
  begin
  vsx:=(xs2-xs1); vsy:=(ys2-ys1);
  norms:=sqrt(vsx*vsx+vsy*vsy);
  if norms>0 then
  begin
    vsx:=vsx/norms; vsy:=vsy/norms;
    wx:=(-vsx+vsy)/sqrt(2); wy:=(-vsy-vsx)/sqrt(2);
    line(xs2,ys2,xs2+round(5*wx),ys2+round(5*wy));
    wx:=(-vsx-vsy)/sqrt(2); wy:=(-vsy+vsx)/sqrt(2);
    line(xs2,ys2,xs2+round(5*wx),ys2+round(5*wy));
    moveto(xs2-10,ys2-10);
    outtext(c);
  end;
  end;
  end;
end;

begin   {******* drawd3axes ******}
  with TheWorld do
  begin
    drawoneaxis(xw1,yw1,zw1,xw2,yw1,zw1,c1);
    drawoneaxis(xw1,yw1,zw1,xw1,yw2,zw1,c2);
    drawoneaxis(xw1,yw1,zw1,xw1,yw1,zw2,c3);
  end;
end;
end.