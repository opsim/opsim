unit cfuncs;

interface

{$mode delphi}

uses
  ctypes, Math;

type
  size_t = cuint64;

{ $DEFINE HASINLINE}

function strncpy(Dest, Source: PChar; MaxLen: cint): PChar; {$IFDEF HASINLINE}inline;{$ENDIF}
function strcpy(destination: PChar; const Source: PChar): PChar; {$IFDEF HASINLINE}inline;{$ENDIF}
function strcasecmp(const string1, string2: PChar): cint; {$IFDEF HASINLINE}inline;{$ENDIF}
function strncmp(const str1, str2: PChar; num: size_t): cint;{$IFDEF HASINLINE}inline;{$ENDIF}
function strcmp(const str1, str2: PChar): cint;{$IFDEF HASINLINE}inline;{$ENDIF}
function strchr(const str: PChar; character: char): PChar;{$IFDEF HASINLINE}inline;{$ENDIF}
function lstrlen ( const str: PChar):size_t;

function toupper(c: char): char; {$IFDEF HASINLINE}inline;{$ENDIF}

function sprintf(str: PChar; const _format: PChar; params: array of const): cint; {$IFDEF HASINLINE}inline;{$ENDIF}
function fprintf(var stream: textfile; const _format: PChar; params: array of const): cint; {$IFDEF HASINLINE}inline;{$ENDIF}

//procedure printf(fm: pchar;args: array of const);cdecl;external 'c';

function printf(const Text: PChar): cint; {$IFDEF HASINLINE}inline;{$ENDIF} overload;
function printf(const _format: PChar; params: array of const): cint; {$IFDEF HASINLINE}inline;{$ENDIF} overload;

function isdigit(c: char): boolean; {$IFDEF HASINLINE}inline;{$ENDIF}

function acos(x: cfloat): cfloat; {$IFDEF HASINLINE}inline;{$ENDIF}
function asin(x: cfloat): cfloat; {$IFDEF HASINLINE}inline;{$ENDIF}
function atan(x: cfloat): cfloat; {$IFDEF HASINLINE}inline;{$ENDIF}
function fsqrt(x: cfloat): cfloat; {$IFDEF HASINLINE}inline;{$ENDIF}

function atof(const str: PChar): cdouble; {$IFDEF HASINLINE}inline;{$ENDIF}
function atoi(const str: PChar): cint; {$IFDEF HASINLINE}inline;{$ENDIF}
function atol(const str: PChar): clong; {$IFDEF HASINLINE}inline;{$ENDIF}
function atoll(const str: PChar): clonglong; {$IFDEF HASINLINE}inline;{$ENDIF}

function pow(base, exponent: cfloat): cfloat; {$IFDEF HASINLINE}inline;{$ENDIF}

procedure bzero(s: pointer; n: size_t); {$IFDEF HASINLINE}inline;{$ENDIF}
procedure bcopy(const src: pointer; dest: pointer; n: cint); {$IFDEF HASINLINE}inline;{$ENDIF}
function memmove(destination: pointer; const Source: pointer; num: size_t): pointer;{$IFDEF HASINLINE}inline;{$ENDIF}
function memcpy(destination: pointer; const Source: pointer; num: size_t): pointer;{$IFDEF HASINLINE}inline;{$ENDIF}
function malloc(size : size_t): pointer;
function calloc(num, size : size_t): pointer;
procedure free(ptr: pointer);

function ffloor(x: cfloat): cfloat;{$IFDEF HASINLINE}inline;{$ENDIF}

function booltoint(f: boolean): cint;{$IFDEF HASINLINE}inline;{$ENDIF}

procedure usleep(time: cuint);{$IFDEF HASINLINE}inline;{$ENDIF}

function iif(f: boolean; a, b: cfloat): cfloat;

//file functions

implementation

uses
  drivers, SysUtils;

function strncpy(Dest, Source: PChar; MaxLen: cint): PChar;
var
  counter: integer;
begin
  counter := 0;

  while (Source[counter] <> #0) and (counter < MaxLen) do
  begin
    Dest[counter] := Source[counter];
    Inc(counter);
  end;

  Inc(counter);

  { pad the string }
  while counter < MaxLen do
  begin
    Dest[counter] := #0;
    Inc(counter);
  end;

  strncpy := Dest;
end;

function strcpy(destination: PChar; const Source: PChar): PChar;
begin
  strcpy := strcopy(destination, Source);
end;

function strcasecmp(const string1, string2: PChar): cint;
begin
  strcasecmp := stricomp(string1, string2);
end;

function strncmp(const str1, str2: PChar; num: size_t): cint;
begin
  strncmp := strlcomp(str1, str2, num);
end;

function strcmp(const str1, str2: PChar): cint;
begin
  strcmp := strcomp(str1, str2);
end;

function strchr(const str: PChar; character: char): PChar;
var
  i: SizeInt;
  s: string;
begin
  s := strpas(str);
  i := pos(character, s);
  strchr := @str[i];
end;

function lstrlen(const str: PChar): size_t;
var
  counter: size_t = 0;
begin
  while (str[counter] <> #0) do
    Inc(counter);

  exit(counter);
end;

function toupper(c: char): char;
begin
  toupper := upcase(c);
end;

function sprintf(str: PChar; const _format: PChar; params: array of const): cint;
var
  res: shortstring = '';
begin
  res := format(_format, params);
  strcopy(str, @res[0]);
  sprintf := length(res);
end;

function fprintf(var stream: textfile; const _format: PChar; params: array of const): cint;
begin
  writeln(stream, format(_format, params));
end;

function printf(const Text: PChar): cint;
var
  str: string;
begin
  str := strpas(Text);
  str := stringreplace(str, '\t', #9, [rfReplaceAll]);
  str := stringreplace(str, '\n', #10, [rfReplaceAll]);
  Write(str);
end;

function printf(const _format: PChar; params: array of const): cint;
var
  str: string;
begin
  str := strpas(_format);
  printf(PChar(format(str, params)));
end;

function isdigit(c: char): boolean;
begin
  isdigit := c in ['0'..'9'];
end;

function acos(x: cfloat): cfloat;
begin
  acos := arccos(x);
end;

function asin(x: cfloat): cfloat;
begin
  asin := arcsin(x);
end;

function atan(x: cfloat): cfloat;
begin
  atan := arctan(x);
end;

function fsqrt(x: cfloat): cfloat;
begin
  fsqrt := sqrt(x);
end;

function atof(const str: PChar): cdouble;
begin
  atof := strtofloat(str);
end;

function atoi(const str: PChar): cint;
begin
  atoi := StrToInt(str);
end;

function atol(const str: PChar): clong;
begin
  atol := StrToInt(str);
end;

function atoll(const str: PChar): clonglong;
begin
  atoll := StrToInt(str);
end;

function pow(base, exponent: cfloat): cfloat;
begin
  pow := Power(base, exponent);
end;

procedure bzero(s: pointer; n: size_t);
begin
  fillchar(s^, n, #0);
end;

procedure bcopy(const src: pointer; dest: pointer; n: cint);
begin
  move(src^, dest^, n);
end;

function memmove(destination: pointer; const Source: pointer; num: size_t): pointer;
var
  t: PChar;
begin
  t := getmem(num);
  move(Source^, t^, num);
  move(t^, destination^, num);
  freemem(t);
end;

function memcpy(destination: pointer; const Source: pointer; num: size_t): pointer;
begin
  move(Source, destination, num);
  memcpy := destination;
end;

function malloc(size: size_t): pointer;
var
  p: pointer;
begin
  p := getmem(size);
  initialize(p);
  exit(p);
end;

function calloc(num, size: size_t): pointer;
var
  p: Pointer;
begin
  p:= malloc(num * size);
  fillchar(p^, num * size, #0);
  exit(p);
end;

procedure free(ptr: pointer);
begin
  finalize(ptr);
  freemem(ptr);
end;

function ffloor(x: cfloat): cfloat;
begin
  ffloor := floor(x);
end;

function booltoint(f: boolean): cint;
begin
  if f then
    booltoint := 1
  else
    booltoint := 0;
end;

procedure usleep(time: cuint);
begin
  sleep(time);
end;

function iif(f: boolean; a, b: cfloat): cfloat;
begin
  if f then iif := a else iif := b;
end;

end.
