unit FSE_main;

{$mode objfpc}{$H-}

interface

uses
  GL,
  MEM_guardedalloc;

const
  FSE_PORT_INPUT = $01;
  FSE_PORT_OUTPUT = $02;

  FSE_PRIM_LINE = $01;
  FSE_PRIM_TEXT = $02;
  FSE_PRIM_ARC = $03;

type
  pFSEobj = ^FSEobj;

  FSEobj = record
    next, prev: pFSEobj;
    name: string[60];
    x: cardinal;
    y: cardinal;

    prim: ListBase;
    ports: ListBase;
    flag: word;
  end;

  (* FSE primitives *)

  //line primitive
  pFSEPline = ^FSEPline;

  FSEPline = record
    next, prev: pFSEPline;
    type_: byte;

    x1: cardinal;
    y1: cardinal;
    x2: cardinal;
    y2: cardinal;
  end;

  //text primitive
  pFSEPtext = ^FSEPtext;

  FSEPtext = record
    next, prev: pFSEPtext;
    type_: byte;

    x: cardinal;
    y: cardinal;
    text: string[255];
  end;

  //arc primitive
  pFSEParc = ^FSEParc;

  FSEParc = record
    next, prev: pFSEParc;
    type_: byte;

    x: cardinal;
    y: cardinal;
    radius: cardinal;
    angle_start: cardinal;
    angle: cardinal;
  end;

  //FSE port
  pFSEport = ^FSEport;

  FSEport = record
    next, prev: pFSEport;
    type_: byte;
    x: cardinal;
    y: cardinal;
    name: string[60];
  end;

procedure FSE_draw(lb: ListBase);

function FSE_object_new(lb: pListBase): pFSEobj;
function FSE_port_new(ob: pFSEobj; name: string; x, y: cardinal; type_: byte): pFSEport;

function FSE_primitive_line_new(ob: pFSEobj; x, y, w, h: cardinal): pFSEPline;
function FSE_primitive_text_new(ob: pFSEobj; text: string; x, y: cardinal): pFSEPtext;
function FSE_primitive_arc_new(ob: pFSEobj; x, y, radius, angle_start, angle: cardinal): pFSEParc;

implementation

uses
  Math;

function FSE_object_new(lb: pListBase): pFSEobj;
var
  ob: pFSEobj;
begin
  ob := callocN(sizeof(FSEobj), 'FSE object');
  ob^.name := newname(lb, 'FSE_object');

  addtail(lb, ob);

  exit(ob);
end;

procedure FSE_object_free(ob: pFSEobj);
var
  pr: pointer;
  po: pFSEport;
begin
  //1. free primitives
  pr := ob^.prim.first;
  while pr <> nil do
  begin

    freeN(pr);

    pr := pFSEPline(pr)^.next;
  end;

  //2. draw ports
  po := ob^.ports.first;
  while po <> nil do
  begin

    freeN(po);

    po := po^.next;
  end;
end;

function FSE_primitive_arc_new(ob: pFSEobj; x, y, radius, angle_start, angle: cardinal): pFSEParc;
var
  pa: pFSEParc;
begin
  pa := callocN(sizeof(FSEParc), 'FSE arc object');

  pa^.type_ := FSE_PRIM_ARC;
  pa^.x := x;
  pa^.y := y;
  pa^.radius := radius;
  pa^.angle_start := angle_start;
  pa^.angle := angle;

  addtail(@ob^.prim, pa);

  exit(pa);
end;

function FSE_primitive_line_new(ob: pFSEobj; x, y, w, h: cardinal): pFSEPline;
var
  pl: pFSEPline;
begin
  pl := callocN(sizeof(FSEPline), 'FSE line object');

  pl^.type_ := FSE_PRIM_LINE;
  pl^.x1 := x;
  pl^.y1 := y;
  pl^.x2 := x + w;
  pl^.y2 := y + h;

  addtail(@ob^.prim, pl);

  exit(pl);
end;

function FSE_primitive_text_new(ob: pFSEobj; text: string; x, y: cardinal): pFSEPtext;
var
  pt: pFSEPtext;
begin
  pt := callocN(sizeof(FSEPtext), 'FSE text object');

  pt^.type_ := FSE_PRIM_TEXT;
  pt^.x := x;
  pt^.y := y;
  pt^.text := text;

  addtail(@ob^.prim, pt);

  exit(pt);
end;

function FSE_port_new(ob: pFSEobj; name: string; x, y: cardinal; type_: byte): pFSEport;
var
  po: pFSEport;
begin
  po := callocN(sizeof(pFSEport), 'FSE port object');

  po^.type_ := type_;
  po^.x := x;
  po^.y := y;
  po^.name := newname(@ob^.ports, 'FSE_port');
  ;

  addtail(@ob^.ports, po);

  exit(po);
end;

procedure FSE_port_draw(x, y: cardinal; po: pFSEport);
const
  PORT_CROSS_SIZE = 15;
begin
  //draw port cross
  glLineWidth(1);
  glColor3f(0, 0, 1);

  glBegin(GL_LINES);
  glVertex3f(x + po^.x - PORT_CROSS_SIZE / 2, y + po^.y - PORT_CROSS_SIZE / 2, 0);
  glVertex3f(x + po^.x + PORT_CROSS_SIZE / 2, y + po^.y + PORT_CROSS_SIZE / 2, 0);
  glEnd;

  glBegin(GL_LINES);
  glVertex3f(x + po^.x - PORT_CROSS_SIZE / 2, y + po^.y + PORT_CROSS_SIZE / 2, 0);
  glVertex3f(x + po^.x + PORT_CROSS_SIZE / 2, y + po^.y - PORT_CROSS_SIZE / 2, 0);
  glEnd;
end;

//copied from: http://slabode.exofire.net/circle_draw.shtml
procedure draw_arc(cx, cy, r, start_angle, arc_angle, num_segments: cardinal);
var
  theta: double;
  tangetial_factor: double;
  radial_factor: double;
  x, y: double;
  ii: cardinal;
  tx, ty: double;
begin
  //theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
  theta := arc_angle / (num_segments - 1);
  tangetial_factor := tan(theta);
  radial_factor := cos(theta);

  x := r * cos(start_angle);//we now start at the start angle
  y := r * sin(start_angle);

  glBegin(GL_POINTS);//since the arc is not a closed curve, this is a strip now
  for  ii := 0 to num_segments - 1 do
  begin
    glVertex2f(x + cx, y + cy);

    tx := -y;
    ty := x;

    x += tx * tangetial_factor;
    y += ty * tangetial_factor;

    x *= radial_factor;
    y *= radial_factor;
  end;
  glEnd();
end;

procedure FSE_primitive_draw(x, y: cardinal; pr: pointer);
var
  pl: pFSEPline;
  pt: pFSEPtext;
  pa: pFSEParc;
begin

  case pFSEPline(pr)^.type_ of
    FSE_PRIM_LINE:
    begin
      pl := pFSEPline(pr);
      glLineWidth(2.5);
      glColor3f(1, 1, 1);

      glBegin(GL_LINES);
      glVertex3f(x + pl^.x1, y + pl^.y1, 0);
      glVertex3f(x + pl^.x2, y + pl^.y2, 0);
      glEnd;
    end;

    FSE_PRIM_TEXT:
    begin
      glBegin(GL_LINES);
      glEnd;
    end;

    FSE_PRIM_ARC:
    begin
      pa := pFSEParc(pr);
      glLineWidth(2.5);
      glColor3f(1, 1, 1);

      draw_arc(x + pa^.x, y + pa^.y, pa^.radius, pa^.angle_start, pa^.angle, 360);
    end;
  end;
end;

procedure FSE_draw(lb: ListBase);
var
  ob: pFSEobj;
  pr: pointer;
  po: pFSEport;
begin
  //1. draw primitives
  ob := lb.first;
  while ob <> nil do
  begin
    pr := ob^.prim.first;

    while pr <> nil do
    begin
      FSE_primitive_draw(ob^.x, ob^.y, pr);
      pr := pFSEPline(pr)^.next;
    end;

    ob := ob^.next;
  end;

  //2. draw ports
  ob := lb.first;
  while ob <> nil do
  begin
    po := ob^.ports.first;

    while po <> nil do
    begin
      FSE_port_draw(ob^.x, ob^.y, po);
      po := po^.next;
    end;

    ob := ob^.next;
  end;
end;

end.
