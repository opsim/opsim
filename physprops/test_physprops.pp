program test_physprops;

{$mode objfpc}{$H-}

uses
  PP_models,
  PhysProps,
  PP_vapor_pressure,
  PP_enthalpy_liquid;

begin
  PP_initialize('./components');

  writeln;
  writeln('vapor pressure');
  writeln('got: ', PP_vapor_pressure_calculate('Water', 373.15): 0: 2, ' expected: 1 bar');
  writeln('got: ', PP_vapor_pressure_calculate('Ethyl Alcohol', 351.47): 0: 2, ' expected: 1 bar');

  writeln;
  writeln('liquid enthalpy');
  writeln(PP_enthalpy_liq_calculate('Water', 25): 0: 2);
  writeln(PP_enthalpy_liq_calculate('Ethanol', 100): 0: 2);

  writeln;

  PP_free;
end.
