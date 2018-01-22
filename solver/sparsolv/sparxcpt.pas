{ Addition to SparSolve unit by
  Alex Jakushev, VGTU, MMm-1
 <Alex.Jakushev@kemek.lt>

  Adds exception raising to SparSolv package

  This unit is designed for Delphi. It was tested
  with Delphi6, however, it should work with at
  least D4, D5, D6, maybe with other versions
  too.

  Vilnius, 2002
}
unit Sparxcpt;

interface

uses SysUtils, SparSolv;

type
  // generic exception class for SparSolv
  ESparSolvError = class( Exception )
  private
    FReason : string;
    FErrNo1: integer;
    FErrNo3: integer;
    FErrNo2: integer;
  public

    property Reason : string read FReason;
    property ErrNo1 : integer read FErrNo1;
    property ErrNo2 : integer read FErrNo2;
    property ErrNo3 : integer read FErrNo3;
  end;

  TSparSolvErrorClass = class of ESparSolvError;

  // ------- EXCEPTIONS -------
  ESparseSolvInitError      = class( ESparSolvError ) end;
  ESparseSolvAddLHSError    = class( ESparSolvError ) end;
  ESparseSolvAddRHSError    = class( ESparSolvError ) end;
  ESparseSolvSolveError     = class( ESparSolvError ) end;
  ESparseSolvAnswerError    = class( ESparSolvError ) end;

resourcestring
  eSparseSolveError = 'SparSolv error: %0:d ( %1:s )  %2:d  %3:d';


{ Use this procedure right after failed SparSolv command
  to get error message and raise exception on it.
}
procedure RaiseSparSolvError( e : TSparSolvErrorClass );

{ Calls InitStruc and raises an exception if this call
  failed.
}
procedure InitStrucA(const NumEq: Integer);

{ Calls AddLHS and raises an exception if this call
  failed.
}
procedure AddLHS_A(const ThisEqu, ThisVar: Integer; const ThisVal: Double);

{ Calls AddRHS and raises an exception if this call
  failed.
}
procedure AddRHS_A(const ThisEqu: Integer; const ThisVal: Double);

{ Calls Solve1 and raises an exception if this call
  failed.
}
procedure Solve1A;

{ Calls GetAnswer and raises an exception if this call
  failed.
}
procedure GetAnswerA(const ThisVar: Integer; var ThisVal: Double);

{ same as GetAnswerA, only as a function
}
function GetAnswerF( const ThisVar : integer ) : double;

implementation

procedure RaiseSparSolvError( e : TSparSolvErrorClass );
var
  reason : string;
  Err1, Err2, Err3 : integer;
  ee : ESparSolvError;
begin
  GetErrorMsg(Reason, Err1, Err2, Err3);
  ee := e.CreateFmt( eSparseSolveError, [
    Err1,
    Reason,
    Err2,
    Err3
  ] );
  ee.FReason := Reason;
  ee.FErrNo1 := Err1;
  ee.FErrNo2 := Err2;
  ee.FErrNo3 := Err3;

  raise ee;
end;

procedure InitStrucA(const NumEq: Integer);
begin
  if not InitStruc( NumEq )
  then RaiseSparSolvError( ESparseSolvInitError );
end;

procedure AddLHS_A(const ThisEqu, ThisVar: Integer; const ThisVal: Double);
begin
  if not AddLHS(ThisEqu, ThisVar, ThisVal)
  then RaiseSparSolvError( ESparseSolvAddLHSError );
end;

procedure AddRHS_A(const ThisEqu: Integer; const ThisVal: Double);
begin
  if not AddRHS(ThisEqu, ThisVal)
  then RaiseSparSolvError( ESparseSolvAddRHSError );
end;

procedure Solve1A;
begin
  if not Solve1
  then RaiseSparSolvError( ESparseSolvSolveError );
end;

procedure GetAnswerA(const ThisVar: Integer; var ThisVal: Double);
begin
  if not GetAnswer(ThisVar, ThisVal)
  then RaiseSparSolvError( ESparseSolvAnswerError );
end;

function GetAnswerF( const ThisVar : integer ) : double;
begin
  GetAnswerA( ThisVar, result );
end;

end.
