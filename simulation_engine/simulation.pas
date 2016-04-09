unit simulation;

interface

uses
  util, pars7, realtype;

type
  pModelVariable = ^ModelVariable;
  ModelVariable = record
    next, prev : pModelVariable;
    value      : double;
    spec       : integer;
  end;

  pModelEquation = ^ModelEquation;
  ModelEquation = record
    next, prev : pModelEquation;
    estack     : pparse;
    vstack     : ListBase;
  end;

  pModelBlock = ^ModelBlock;
  ModelBlock = record
    next, prev : pModelBlock;
    equations  : ListBase;
  end;

  pSimulationCase = ^SimulationCase;
  SimulationCase = record
    next, prev : pSimulationCase;
    blocks     : ListBase;
    sse        : float;
  end;

procedure calculate_residuals(sc: pSimulationCase);
procedure SIM_free_simulation_case(sc: pSimulationCase);

implementation

procedure calculate_residuals(sc: pSimulationCase);
var
  mb       : pModelBlock;
  me       : pModelEquation;
  r        : float = 0;
  mv1, mv2 : pModelVariable;
begin
  //sum square error
  sc^.sse := 0;

  mb := sc^.blocks.first;

  //for each modelblock
  while mb <> nil do
  begin
    me := mb^.equations.first;

    //for each model equation
    while me <> nil do
    begin
      mv1 := me^.vstack.first;
      mv2 := mv1^.next;

      me^.estack^.f(mv1^.value, mv2^.value, 0, r);
      sc^.sse += sqr(r);

      me := me^.next;
    end;

    mb := mb^.next;
  end;
end;

procedure SIM_free_simulation_case(sc: pSimulationCase);
var
  mb       : pModelBlock;
  me       : pModelEquation;
begin
  mb := sc^.blocks.first;

  //for each modelblock
  while mb <> nil do
  begin
    me := mb^.equations.first;

    //for each model equation
    while me <> nil do
    begin
      freelistN(@me^.vstack);
      dispose(me^.estack);

      me := me^.next;
    end;
    freelistN(@mb^.equations);

    mb := mb^.next;
  end;
  freelistN(@sc^.blocks);
end;

end.
