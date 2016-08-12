program test_physprops;

{$mode objfpc}{$H-}

uses
  LinkedList,
  unit_conv,
  PP_models,
  PhysProps,
  PP_vapor_pressure,
  PP_enthalpy_liquid;

var
  v1, v2: SimVars;

begin
  PP_initialize('./components');

  writeln;
  writeln('vapor pressure');

  v1.value := callocN(sizeof(double));
  v1.value^ := 373.15;
  v1.uom := UNC_find_conversion(nil, 'degC');

  v2 := PP_vapor_pressure_calculate('Water', v1);

  writeln('got: ', v2.value^ : 0: 2, ' expected: 1 bar');

  v1.value^ := 351.47;

  v2 := PP_vapor_pressure_calculate('Ethyl Alcohol', v1);

  writeln('got: ', v2.value^: 0: 2, ' expected: 1 bar');

  writeln;
  writeln('liquid enthalpy');
  writeln(PP_enthalpy_liq_calculate('Water', 25): 0: 2);
  writeln(PP_enthalpy_liq_calculate('Ethanol', 100): 0: 2);

  writeln;

  PP_free;
end.
