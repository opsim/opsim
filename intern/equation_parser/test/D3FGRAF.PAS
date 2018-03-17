program d3fgraf;
{$M 30000,1024,655360}
{This displays 3d-graphs of functions of two variables x and y that
 can be entered by the user. You give it the function string and the
 x,y-window bordered by xmin,xmax,ymin,ymax, and it draws you the
 graph in the graphics mode it detects. It does detect vesa, but
 does not use the highest mode to save some time and to keep your
 eyes from being damaged by the 8x8 font. You can rotate, zoom in
 an out of a display.}

uses dos,crt,pars7,graph,grafpack,realtype;

const enter=#13; esc=#27; functionkey=#0;
      Uparrow='H'; Downarrow='P';
      leftarrow='K'; rightarrow='M';
      greater='>'; less='<';
      Updown=''; rightleft=chr(29);
var s:string; xmin,xmax,ymin,ymax,fmin,fmax,x,y,r,rcenter,rright,rfront,
               deltax,deltay:float;
     i,j,grid:integer;  ans: char;
     error,firsttime:boolean;
     myfunc:PParse;

procedure wait;
var ans:char;
begin
  repeat until keypressed;
  ans:=readkey;
  if ans=functionkey then ans:=readkey;
end;

procedure getkey(var ans:char);
begin
  repeat until keypressed;
  ans:=readkey;
  if ans=functionkey then ans:=readkey;
end;

begin
  initgraphic('D:\BP\BGI');  {Put you own BGI-path here}
  leavegraphic;
  repeat
    error:=false;
    write('f(x,y) = ');readln(s); writeln;
    myfunc:=New(PParse,init(s,true,error));
    if error then begin
      writeln('Can''t understand that term.');
      myfunc:=nil end
    else
    begin
      firsttime:=true;
      repeat
        if firsttime then
        begin
          write('xmin =  '); readln(xmin);
          write('xmax =  '); readln(xmax);
          write('ymin =  '); readln(ymin);
          write('ymax =  '); readln(ymax);
          write('gridsize (number of meshpoints in either x or y direction) = ');
          readln(grid);
          fmin:=100000000.0; fmax:=-1000000000.0;
          for i:=0 to 80 do
          for j:=0 to 80 do
          begin
            myfunc^.f(xmin+i*(xmax-xmin)/80,ymin+j*(ymax-ymin)/80,0,r);
            fmin:=min(r,fmin);
            fmax:=max(r,fmax);
          end;
          r:=(fmax-fmin)/20;
          if r=0 then r:=0.01;
          fmax:=fmax+r;
          fmin:=fmin-r;
          entergraphic;
          setwindow(2,2,22);
          viewdist:=6;xwrot:=30;zwrot:=60;
          setd3world(xmin,ymin,fmin,xmax,ymax,fmax,viewdist,xwrot,zwrot);
          gotoxy(3,1);  write('f(x,y) = '+s);
          gotoxy(60,3); write('X-Y-window:');
          gotoxy(60,4); write(xmin:10:4,ymin:10:4);
          gotoxy(60,5); write(xmax:10:4,ymax:10:4);
          gotoxy(60,6); write('Min and Max of f:');
          gotoxy(61,7); write(fmin:10:4,fmax:10:4);
        end;
        clearviewport;
        rectangle(0,0,xw2glb-xw1glb,yw2glb-yw1glb);
        drawd3axes('x','y','z');
        if 0>=xmin then if 0<=xmax then if 0>=ymin then if 0<=ymax then
        d3line(0,0,fmin,0,0,fmax);
        if 0>=xmin then if 0<=xmax then if 0>=fmin then if 0<=fmax then
        d3line(0,ymin,0,0,ymax,0);
        if 0>=fmin then if 0<=fmax then if 0>=ymin then if 0<=ymax then
        d3line(xmin,0,0,xmax,0,0);
        deltax:=(xmax-xmin)/grid;
        deltay:=(ymax-ymin)/grid;
        x:=xmin;
        for i:=0 to grid do
        begin
          y:=ymin;
          myfunc^.f(x,y,0,rcenter);
          for j:=1 to grid do
          begin
            myfunc^.f(x,y+deltay,0,rright);
            myfunc^.f(x+deltax,y,0,rfront);
            d3line(x,y,rcenter,x,y+deltay,rright);
            d3line(x,y,rcenter,x+deltax,y,rfront);
            rcenter:=rright; y:=y+deltay;
          end;
          x:=x+deltax;
        end;
        gotoxy(2,25);
                       write('R: Rotate      W: New Window    F: New Function   Esc: Exit');
        getkey(ans);
        if upcase(ans)='R' then
        begin
          gotoxy(2,25);
          write(updown,rightleft,': Rotate around the world    <>: Zoom       Enter: Draw ');
          clearviewport;
          drawd3axes('x','y','z');
          if 0>=xmin then if 0<=xmax then if 0>=ymin then if 0<=ymax then
          d3line(0,0,fmin,0,0,fmax);
          if 0>=xmin then if 0<=xmax then if 0>=fmin then if 0<=fmax then
          d3line(0,ymin,0,0,ymax,0);
          if 0>=fmin then if 0<=fmax then if 0>=ymin then if 0<=ymax then
          d3line(xmin,0,0,xmax,0,0);
          repeat
            getkey(ans);
            setcolor(0);
            drawd3axes('x','y','z');
            if 0>=xmin then if 0<=xmax then if 0>=ymin then if 0<=ymax then
            d3line(0,0,fmin,0,0,fmax);
            if 0>=xmin then if 0<=xmax then if 0>=fmin then if 0<=fmax then
            d3line(0,ymin,0,0,ymax,0);
            if 0>=fmin then if 0<=fmax then if 0>=ymin then if 0<=ymax then
            d3line(xmin,0,0,xmax,0,0);
            if ans=uparrow then rotatez(-1);
            if ans=downarrow then rotatez(1);
            if ans=leftarrow then rotatex(-1);
            if ans=rightarrow then rotatex(1);
            if ans=less then zoomin;
            if ans=greater then zoomout;
            setcolor(getmaxcolor);
            drawd3axes('x','y','z');
            if 0>=xmin then if 0<=xmax then if 0>=ymin then if 0<=ymax then
            d3line(0,0,fmin,0,0,fmax);
            if 0>=xmin then if 0<=xmax then if 0>=fmin then if 0<=fmax then
            d3line(0,ymin,0,0,ymax,0);
            if 0>=fmin then if 0<=fmax then if 0>=ymin then if 0<=ymax then
            d3line(xmin,0,0,xmax,0,0);
          until ans=enter;
          firsttime:=false;
        end;
        if upcase(ans)='W' then begin leavegraphic; firsttime:=true; end;
      until (ans=esc) or (upcase(ans)='F');
      leavegraphic;
    end;
    if myfunc<>nil then dispose(myfunc,done);
    firsttime:=true;
  until ans=esc;
  if vesaglb then
  begin
    entergraphic;
    setgraphmode(1);
    leavegraphic;
  end;
  closegraph;
end.
