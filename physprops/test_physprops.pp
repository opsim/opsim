program test_physprops;

{$mode objfpc}{$H+}

uses
  PP_coefficients,
  PP_vapor_pressure,
  PP_enthalpy_liquid,
  util;

begin
  PP_initialize;

  PP_vapor_pressure_load('vap_press.pysprop');
  PP_enthalpy_liq_load('enth_liq.pysprop');

  writeln(PP_vapor_pressure_calculate('Water', 100): 0: 2);
  writeln(PP_vapor_pressure_calculate('Ethanol', 78.32): 0: 2);

  writeln;

  writeln(PP_enthalpy_liq_calculate('Water', 25): 0: 2);
  writeln(PP_enthalpy_liq_calculate('Ethanol', 100): 0: 2);

  PP_free;
  printmemlist;
  readln;
end.
