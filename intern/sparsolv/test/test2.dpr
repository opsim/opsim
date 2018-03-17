program Test2;
 {a SparSolv example to show various types of singular matrix}
 {suggest running from DOS,  test2>log  , then examine file log}

{you may have to adjust these defines .........}
{$DEFINE DELPHI}   {if using Delphi}
{$DEFINE DELPHI32} {if using Delphi 2 or later}
 
{Free Pascal was compiled with:
fpc -Mdelphi -gl test1.dpr
Free Pascal Compiler version 2.0.0 [2005/05/08] for i386

Next section is needed to make sure that a text window is opened when
either test program is run from Windows.  For 16-bit Windows, you need
the WinCrt unit.  For Delphi 2.0 and above, you need the APPTYPE CONSOLE
directive. }

{$IFDEF DELPHI32}{$APPTYPE CONSOLE}{$ENDIF} {32-bit Delphi only}

{$IFDEF DELPHI}
uses SparSolv, SysUtils;
{$ELSE} {TPW BPW or Turbo Pascal}
{$IFDEF Windows}
uses SparSolv, WinCrt; {TPW BPW}
{$ELSE}
uses SparSolv; {DOS}
{$ENDIF}
{$ENDIF}

const null = 666.666;

procedure setrow(const row: Integer; const c1, c2, c3, c4, rhs: Single);
var
  Reason: string;
  ErrNo1, ErrNo2, ErrNo3: Integer;
label err;
begin
  if (c1 < 100) then if not AddLHS(row, 1, c1) then goto err;
  if (c2 < 100) then if not AddLHS(row, 2, c2) then goto err;
  if (c3 < 100) then if not AddLHS(row, 3, c3) then goto err;
  if (c4 < 100) then if not AddLHS(row, 4, c4) then goto err;
  if (rhs < 100) then if not AddRHS(row, rhs) then goto err;
  Exit;
  err:
  GetErrorMsg(Reason, ErrNo1, ErrNo2, ErrNo3);
  WriteLn('SetRow Failed:  Error ', ErrNo1: 0, ' ', Reason, ' ', ' ', ErrNo2: 3, ' ', ErrNo3: 3);
  Halt(1);
end;

var
  Reason: string;
  ErrNo1, ErrNo2, ErrNo3: Integer;
  Example, row, N: Integer;
  Value: Double;
label Fail;
begin
  WriteLn('This program tests that error reporting routines work correctly.');
  WriteLn('Hit return to continue.'); ReadLn;

  N := 4; {size of matrix}
  for Example := 1 to 7 do begin
    if not InitStruc(N) then goto Fail;
    WriteLn; WriteLn('Example ', Example);
    case Example of
      1: begin
          WriteLn('Successful case');
          setrow(1, 0.50, null, 2.00, null, 6.50);
          setrow(2, null, 0.20, null, 5.00, 20.4);
          setrow(3, 0.75, null, 1.00, null, 3.75);
          setrow(4, 2.00, null, null, 6.00, 26.0);
        end;
      2: begin
          WriteLn('4th row is multiple of 2nd');
          setrow(1, null, 0.50, 2.00, null, 6.50);
          setrow(2, 0.20, null, null, 5.00, 20.4);
          setrow(3, 0.75, null, 1.00, null, 3.75);
          setrow(4, 1.50, null, 2.00, null, 26.0);
        end;
      3: begin
          WriteLn('4th col is multiple of 3rd');
          setrow(1, 0.50, null, 2.00, 1.00, 6.50);
          setrow(2, null, 0.20, null, null, 20.4);
          setrow(3, 0.75, null, 1.00, 0.50, 3.75);
          setrow(4, 2.00, null, null, null, 26.0);
        end;
      4: begin
          WriteLn('Empty row 2');
          setrow(1, 0.50, null, 2.00, 1.00, 6.50);
          setrow(2, null, null, null, null, null);
          setrow(3, null, 4.00, null, 0.50, 3.75);
          setrow(4, 2.00, null, null, null, 26.0);
        end;
      5: begin
          WriteLn('Row 2 without variables');
          setrow(1, 0.50, null, 2.00, null, 6.50);
          setrow(2, null, null, null, null, 7.30);
          setrow(3, 0.75, null, 1.00, null, 3.75);
          setrow(4, null, 1.00, null, 6.00, 26.0);
        end;
      6: begin
          WriteLn('No RHS for row 3');
          setrow(1, 0.50, null, 2.00, null, 6.50);
          setrow(2, null, 0.20, null, 5.00, 20.4);
          setrow(3, 0.75, null, 1.00, null, null);
          setrow(4, 2.00, null, null, 6.00, 26.0);
        end;
      7: begin
          WriteLn('Variables 2 and 4 each appear only in row 2');
          setrow(1, 0.50, null, 2.00, null, 6.50);
          setrow(2, null, 0.20, null, 5.00, 20.4);
          setrow(3, 0.75, null, 1.00, null, 3.75);
          setrow(4, 2.00, null, 1.00, null, 26.0);
        end;
    end; { case}
    showmat;
    if not Solve1 then begin
      GetErrorMsg(Reason, ErrNo1, ErrNo2, ErrNo3);
      WriteLn('Failed:  Error ', ErrNo1: 0, ' ', Reason, ' ', ' ', ErrNo2: 3, ' ', ErrNo3: 3);
    end else begin
      Write('Solved: ');
      for row := 1 to N do if GetAnswer(row, Value)
        then Write('  X(', row, ')=', Value: 0: 3)
        else goto Fail;
      WriteLn;
    end;
    ReleaseStruc;
      WriteLn('Hit return to continue.'); ReadLn; 
  end; {for example}
   {prevent DOS box closing before you can read it}
  WriteLn('hit return to finish'); ReadLn;
  Exit;
  Fail:
  GetErrorMsg(Reason, ErrNo1, ErrNo2, ErrNo3);
  WriteLn('Fatal error:  Error ', ErrNo1: 0, ' ', Reason, ' ', ' ', ErrNo2: 3, ' ', ErrNo3: 3);
{prevent DOS box closing before you can read it}
  WriteLn('hit return to finish'); ReadLn;
end.
