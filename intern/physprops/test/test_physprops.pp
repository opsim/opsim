program test_physprops;

{$mode objfpc}

uses
  MEM_guardedalloc,
  unit_conv,
  PP_models,
  PhysProps,
  PP_vapor_pressure,
  PP_cp_gas;

var
  v1, v2: SimVars;

begin
  PP_initialize('./components');

  writeln;
  writeln('vapor pressure');

  v1.value := callocN(sizeof(double), 'SimVars value');
  v1.value^ := 373.15;
  v1.uom := UNC_find_conversion(nil, 'K');

  v2 := PP_vapor_pressure_calculate('Water', v1);

  writeln('got: ', UNC_convert_unit(v2.value^, v2.uom, UNC_find_conversion(nil, 'bar')): 0: 4, ' expected: 1 bar');

  v1.value^ := 351.47;

  v2 := PP_vapor_pressure_calculate('Ethyl Alcohol', v1);

  writeln('got: ', UNC_convert_unit(v2.value^, v2.uom, UNC_find_conversion(nil, 'bar')): 0: 4, ' expected: 1 bar');

  writeln;
  writeln('liquid enthalpy');
  v1.value^ := 25;
  v1.uom := UNC_find_conversion(nil, 'degC');

  v2 := PP_cp_gas_calculate('Water', v1);

  writeln('got: ', UNC_convert_unit(v2.value^, v2.uom, UNC_find_conversion(nil, 'J/mol.K')): 0: 4, ' expected: 33.6382 J/mol.K');

  v1.value^ := 100;

  v2 := PP_cp_gas_calculate('Ethyl Alcohol', v1);

  writeln('got: ', UNC_convert_unit(v2.value^, v2.uom, UNC_find_conversion(nil, 'J/mol.K')): 0: 4, ' expected: 76.6853 J/mol.K');

  writeln;
  readln;

  PP_free;
end.
