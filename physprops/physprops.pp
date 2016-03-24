program physprops;

{$mode objfpc}{$H+}

uses
  PP_coefficients,
  PP_vapor_pressure,
  util;

begin
  PP_initialize;

  PP_vapor_pressure_load('physprops.model');

  writeln(PP_vapor_pressure_calculate('Water', 100): 0: 2);
  writeln(PP_vapor_pressure_calculate('Water', 78.32): 0: 2);
  writeln(PP_vapor_pressure_calculate('Water', 300): 0: 2);

  writeln;

  writeln(PP_vapor_pressure_calculate('Ethanol', 100): 0: 2);
  writeln(PP_vapor_pressure_calculate('Ethanol', 78.32): 0: 2);
  writeln(PP_vapor_pressure_calculate('Ethanol', 200): 0: 2);

  PP_free;
  printmemlist;
  readln;
end.
