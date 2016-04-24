program test_physprops;

{$mode objfpc}{$H-}

uses
  PhysProps,
  PP_vapor_pressure,
  PP_enthalpy_liquid;

begin
  PP_initialize('./components');

  writeln;
  writeln('vapor pressure');
  writeln('got: ', PP_vapor_pressure_calculate('Water', 100): 0: 2, ' expected: 760mm Hg');
  writeln('got: ', PP_vapor_pressure_calculate('Ethanol', 78.32): 0: 2, ' expected: 760mm Hg');

  writeln;
  writeln('liquid enthalpy');
  writeln(PP_enthalpy_liq_calculate('Water', 25): 0: 2);
  writeln(PP_enthalpy_liq_calculate('Ethanol', 100): 0: 2);

  writeln;

  PP_free;
end.
