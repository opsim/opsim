program p7demo;
{$M 30000,0,655360}
{This is a demo program comparing speed of a parsed function versus
the precompiled version. It's cheating a little, because f is programmed
as a function, which is ever so slightly slower than a procedure. You
can also observe how much slower protected mode is, when you can use it.}
{$N+,E+} {change the type "float" in realtype.pas to be able to compile
           in N- mode}
uses dos,crt,pars7,realtype;

var s:string; r,rold,xmin,xmax,ymin,ymax,fmin,fmax,x,y:float;
     i,j,n,m:integer;
     hour1,minute1,second1,sec1001,
     hour2,minute2,second2,sec1002:word;
     elapsed1,elapsed2,start,ende:float;
     ans:char;
     error:boolean;
     myfunc:pparse;

procedure wait;
begin
  repeat
  until keypressed;
  if readkey='*' then;
end;


function f(x,y,t:float):float;
begin
  f:=cos(4*x*y*(exp(-0.3*x*x)*sin(3*sin(3*x) - 4*cos(4*y)) +
               2*exp(-0.5*y*y)*cos(5*sin(3*x) + 2*cos(4*y))));
end;

begin
  clrscr;
  s:='cos(4*x*y*(exp(-0.3*x^2)*sin(3*sin(3*x)-4*cos(4*y))+2*exp(-0.5*y^2)*cos(5*sin(3*x)+2*cos(4*y))))';
  writeln('f(x,y) = ',s);
  myfunc:=new(pparse,init(s,true,error));
  writeln;
  writeln('Done parsing.');
  writeln('Now the compiled function f is being used.');
  gettime(hour1,minute1,second1,sec1001);
  for i:=1 to 50 do
    for j:=1 to 50 do
      r:=f(i/10,j/10,0);
  gettime(hour2,minute2,second2,sec1002);
  start:=(hour1*60+minute1)*60+second1+sec1001/100;
  ende:=(hour2*60+minute2)*60+second2+sec1002/100;
  elapsed1:=ende-start;
  writeln('time for 2500 calls: ',elapsed1:6:2,' seconds');
  writeln('Press any key');
  wait;
  writeln('Now the parsed function f is being used');
  gettime(hour1,minute1,second1,sec1001);
  with myfunc^ do
  for i:=1 to 50 do
    for j:=1 to 50 do
      f(i/10,j/10,0,r);
  gettime(hour2,minute2,second2,sec1002);
  start:=(hour1*60+minute1)*60+second1+sec1001/100;
  ende:=(hour2*60+minute2)*60+second2+sec1002/100;
  elapsed2:=ende-start;
  writeln('time for 2500 calls: ',elapsed2:6:2,' seconds');
  writeln('Press any key');
  wait;
  dispose(myfunc,done);
end.
