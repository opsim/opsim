program p7demo;

{ $M 30000,0,655360}
{This is a demo program comparing speed of a parsed function versus
the precompiled version. It's cheating a little, because f is programmed
as a function, which is ever so slightly slower than a procedure. You
can also observe how much slower protected mode is, when you can use it.}

{ $N+,E+} {change the type "float" in realtype.pas to be able to compile
           in N- mode}
uses
  SysUtils,
  pars7,
  realtype;

const
  COUNT = 10000;

var
  s: string;
  r: float;
  i, j: integer;
  starttime: double;
  elapsed, elapsed1: double;
  error: boolean = false;
  myfunc: pparse;

  function f(x, y, t: float): float;
  begin
    f := -2/3*x+5*(x-4)-8;
  end;

begin
  writeln({$I %FPCTARGETCPU%}, '-', {$I %FPCTARGETOS%}, ' fpc:', {$I %FPCVERSION%});

  s := '-2/3*x+5*(x-4)-8';
  writeln('f(x,y) = ', s);
  writeln(COUNT * COUNT, ' evaluations');
  writeln;

  starttime := Now;
  for i := 1 to COUNT do
    for j := 1 to COUNT do
      r := f(i / 10, j / 10, 0);
  elapsed1 := (Now - starttime) * 24 * 3600;
  writeln('        compiled function: ', elapsed1: 6: 2, ' seconds');

  myfunc := new(pparse, init(s, False, error));
  starttime := Now;
  with myfunc^ do
    for i := 1 to COUNT do
      for j := 1 to COUNT do
        f(i / 10, j / 10, 0, r);
  elapsed := (Now - starttime) * 24 * 3600;
  writeln('          parsed function: ', elapsed: 6: 2, ' seconds, ratio: ', (elapsed/elapsed1-1)*100: 3: 0, '%');
  dispose(myfunc, done);

  myfunc := new(pparse, init('4.3333333333*x-28', False, error));
  starttime := Now;
  with myfunc^ do
    for i := 1 to COUNT do
      for j := 1 to COUNT do
        f(i / 10, j / 10, 0, r);
  elapsed := (Now - starttime) * 24 * 3600;
  writeln('optimized parsed function: ', elapsed: 6: 2, ' seconds, ratio: ', (elapsed/elapsed1-1)*100: 3: 0, '%');
  dispose(myfunc, done);

  writeln('Press any key');
  readln;
end.
