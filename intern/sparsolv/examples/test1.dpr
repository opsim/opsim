program Test1;
 {a SparSolv example to test performance and memory use}

{you may have to adjust these defines .........}
{$DEFINE DELPHI}   {if using Delphi}
{$DEFINE DELPHI32} {if using Delphi 2 or later or any 32-bit pascal}

{Free Pascal was compiled with:
fpc -Mdelphi -gl test1.dpr
Free Pascal Compiler version 2.0.0 [2005/05/08] for i386

Next section is needed to make sure that a text window is opened when
either test program is run from Windows.  For 16-bit Windows, you need
the WinCrt unit.  For Delphi 2.0 and above, you need the APPTYPE CONSOLE
directive. }

{$IFDEF DELPHI32}{$APPTYPE CONSOLE}{$ENDIF} {Delphi 2,3,4,5 only}

{$IFDEF DELPHI}
uses SparSolv, SysUtils;
{$ELSE} {TPW BPW or Turbo Pascal}
{$IFDEF Windows}
uses SparSolv, WinCrt; {TPW BPW}
{$ELSE}
uses SparSolv; {DOS}
{$ENDIF}
{$ENDIF}

{$IFDEF DELPHI}
function Seconds: Double;
var Hour, Min, Sec, MSec: Word;
begin
  DecodeTime(Now, Hour, Min, Sec, MSec);
  Seconds := 3600 * Hour + 60 * Min + sec + MSec / 1000;
end;
{$ELSE}
function Seconds: Double;
begin
  Seconds := 0.0; {timer only works under Delphi}
end;
{$ENDIF}

const EntriesPerRow = 6;
var
  Reason: string;
  ErrNo1, ErrNo2, ErrNo3: Integer;
  Criterion, Density: Single;
  Col, Row, N: Integer;
  Count: LongInt;
  InitMemUsed: LongInt;
  Total, Value: Double;
  Start, Stop: Double;
  ErrSum: Double;
label Fail, EndProg;
begin

  WriteLn('Setting up matrix...');

  N := 3000; {size of matrix - set larger if you want}
  Count := 0;
  if not InitStruc(N) then goto Fail;

 {construct a matrix with random elements}
 {RHS is set so that variable V will have value V}
 {diagonal elements are all filled to prevent singularity}
  Start := Seconds;
  for Row := 1 to N do begin
    Total := 0.0;
    for Col := N downto 1 {1 to N} do begin
      Criterion := (EntriesPerRow - 1) * (Row + Col) / (LongInt(N) * N);
   {this causes some rows and cols to be fuller than others}
      if ((Row = Col) or (Random < Criterion)) then begin
        Inc(Count);
        Value := N * Random + Row + Col;
        if not AddLHS(Row, Col, Value) then goto Fail;
        Total := Total + Col * Value;
      end;
    end;
    if not AddRHS(Row, Total) then goto Fail;
  end; {for Row:= 1 TO N}
  Stop := Seconds;
  Density := 100.0 * (Count / N) / N;
  InitMemUsed := SparMemUsed;

  WriteLn('N': 30, N: 12);
  WriteLn('Count': 30, Count: 12);
  WriteLn('Density': 30, Density: 12: 3, '%');
  WriteLn('Vars per Row': 30, (Count / N): 12: 1);
  WriteLn('Initial Memory Used': 30, InitMemUsed: 12);
  WriteLn('Load time': 30, (Stop - Start): 12: 1, ' seconds');
 {pause to read information so far}
  WriteLn('hit return to go on'); ReadLn;

  WriteLn('Solving...Please Wait');
  Start := Seconds;
  if not Solve1 then goto Fail;
  Stop := Seconds;
  ErrSum := 0.0;
  for Row := 1 to N do begin
    if not GetAnswer(Row, Value) then goto Fail;
    ErrSum := ErrSum + Abs(Value - Row);
    if ((Row = 1) or (Row = N) or ((Row mod (N div 10)) = 0)) then
      WriteLn(Row: 5, Value: 15: 5);
  end;
  WriteLn('Solve time': 30, (Stop - Start): 12: 1, ' seconds');
  WriteLn('Maximum Memory Used': 30, MaxMemUsed: 12);
  WriteLn('Memory Growth Factor': 30, (MaxMemUsed / InitMemUsed): 12: 1);
  WriteLn('Sum of Errors:': 30, ErrSum: 12: 5);
  goto EndProg;

  Fail:
  GetErrorMsg(Reason, ErrNo1, ErrNo2, ErrNo3);
  WriteLn('Failed:  Error', ErrNo1: 0, ' ', Reason, ' ', ' ', ErrNo2: 3, ' ', ErrNo3: 3);
  EndProg:
  ReleaseStruc;
  WriteLn('Memory Used after ReleaseStruc': 30, SparMemUsed: 12);
 {prevent DOS box closing before you can read it}
  WriteLn('hit return to finish'); ReadLn;
end.
