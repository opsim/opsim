program simulation_engine_test;

uses
  util,
  simulation,
  solver,
  pars7;

var
  sc       : SimulationCase;
  mb       : pModelBlock;
  me       : pModelEquation;
  error    : boolean = false;
  mv1, mv2 : pModelVariable;
begin
  //a simulation case holds one or more model blocks
  mb := callocN(sizeof(ModelBlock), 'model block');
  addtail(@sc.blocks, mb);

  //a model block holds one or more equations
  me := callocN(sizeof(ModelEquation), 'model equation');
  addtail(@mb^.equations, me);

  //load an equation in memory
  me^.estack := new(pparse, init('x-273.15-y', True, error));

  //todo: create the right amount of variables
  //a model equation holds one or more variables and equations
  mv1 := callocN(sizeof(ModelVariable), 'model variable');
  mv1^.spec := 1;   //fixed value
  mv1^.value := 0;  //degrees Kelvin
  addtail(@me^.vstack, mv1);

  mv2 := callocN(sizeof(ModelVariable), 'model variable');
  mv2^.spec := 0;   //free value
  mv2^.value := 10; //initial value
  addtail(@me^.vstack, mv2);

  //setup the solver
  //sm := callocN(sizeof(SolverMgr), 'solver manager');
  sm.simulation := @sc;
  sm.eps := 1e-4;
  sm.calc_residuals := @calculate_residuals;
  sm.solve := @simplex_solve;;

  //run the simulations
  sm.solve;

  SIM_free_simulation_case(@sc);

  printmemlist;
  writeln('done.');
  readln;
end.
