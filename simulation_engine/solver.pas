unit solver;

interface

uses
  simulation;

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

procedure simplex_solve;
var
 num : integer = 0;
begin
  calculate_residuals(sm.simulation);
  while (sm.simulation^.sse > sm.eps) and (num < 50) do
  begin
    writeln('run ', num, ' sse = ', sm.simulation^.sse);
    calculate_residuals(sm.simulation);
    inc(num);
  end;
  writeln('run ', num, ' sse = ', sm.simulation^.sse);
end;

end.
