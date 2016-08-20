program endian_test;

type
  Q = record
    case Boolean of
      True: (i: Integer);
      False: (p: array[1..4] of Byte);
  end;

var
  x: ^Q;
  order: word;
begin
  // First indicate what the compiler thinks we (should) be using:
  {$IFDEF ENDIAN_LITTLE}
  writeln('FPC compiler: little endian');
  {$ENDIF}
  {$IFDEF ENDIAN_BIG}
  writeln('FPC compiler: big endian');
  {$ENDIF}

  New(x);
  x^.i := 5;
  if x^.p[1] = 5 then
    writeln('application: little endian')
  else
  if x^.p[4] = 5 then
    writeln('application: big endian');
  Dispose(x);

  order := 1;
  if pchar(@order)^ = #1 then
    writeln('application: little endian')
  else
    writeln('application: big endian');

  { Make it wait so we can see the results }

  writeln;
  write('Press enter when you finish reading this ');
  ReadLn;
end.
