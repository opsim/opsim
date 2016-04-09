unit solver;

interface

uses
  simulation, simplex;

type
  pSolverMgr = ^SolverMgr;
  SolverMgr = record
    eps            : double;
    simulation     : pSimulationCase;
    solve          : procedure;
    calc_residuals : procedure(sc: pSimulationCase);
  end;

var
 sm : SolverMgr;

procedure simplex_solve;

implementation

function func(p: vec): real;
begin
  //ok hardcoded the variable for now, later we need to address the free
  //variables via an array
  pModelVariable(pModelEquation(pModelBlock(sm.simulation^.blocks.first)^.equations.first)^.vstack.first)^.next^.value:=p[1];

  calculate_residuals(sm.simulation);
  func := sm.simulation^.sse;
end;

procedure simplex_solve;
var
  P: MAT;
  Y, PT: VEC;
  I, ITER, NDIM: integer;
  FTOL: double;
begin
  //todo: determine number of variables from simulation case
  //todo: put adresses of free variables in array
  //todo: drop static vector/matrix by dynamically allocated
  //todo: put solver parameters in settings record
  //todo: assign starting values based on initial values +/- random value (size of random?)

  ITER := 0;
  NDIM := 1;      { 2 variables }
  FTOL := sm.eps;   { User given tolerance }

  {define NDIM+1 initial vertices (one by row) }
  P[1, 1] := random;
  P[1, 2] := random;
  P[2, 1] := random;
  P[2, 2] := random;

  {Initialize Y to the values of FUNC evaluated
   at the NDIM+1 vertices (rows] of P }
  for I := 1 to NDIM + 1 do
  begin
    PT[1] := P[I, 1];
    Y[I] := FUNC(PT);
  end;

  {call main subroutine}
  AMOEBA(P, Y, MP, NP, NDIM, FTOL, ITER, @func);

  writeln;
  writeln('iterations=', ITER, ' sse=', sm.simulation^.sse, ' value=', p[1,1]);
end;

end.
