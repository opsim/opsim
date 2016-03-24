program unit_conv_test;

uses
  util, unit_conv;

var
  convert_from : pUnitConversion;
  convert_to   : pUnitConversion;

begin
  //mass
  UNC_define_conversion('Mass', 'kg', 1, 0, UNC_FLAG_METRIC);
  UNC_define_conversion('Mass', 'tonne', 1000, 0, UNC_FLAG_METRIC);

  //temperature
  UNC_define_conversion('Temperature', 'C', 1, 0, UNC_FLAG_METRIC);
  UNC_define_conversion('Temperature', 'F', 0.55555556, -17.77778, UNC_FLAG_IMPERIAL or UNC_FLAG_US_CUSTOMARY);

  writeln(UNC_convert_unit(10000, 'kg', 'tonne', False): 0: 2);
  writeln(UNC_convert_unit(1, 'tonne', 'kg', False): 0: 2);
  writeln;
  writeln(UNC_convert_unit(100, 'C', 'F', False): 0: 2);
  writeln(UNC_convert_unit(1, 'F', 'C', False): 0: 2);

  //Much faster way to convert units, lookup is not needed anymore
  //the converter structs are looked up once and can then be refrerenced by a
  //pointer. This can be done for each available value in the model.
  //Via the parentquant variable the parent quantities can be looked up
  //possible we could cache this as well, but we can decide later on for
  //performance issues to do so in the GUI
  convert_from := UNC_find_conversion(nil, 'C');
  convert_to := UNC_find_conversion(nil, 'F');
  writeln;
  writeln(UNC_convert_unit(38, convert_from, convert_to, False): 0: 2);

  UNC_free;

  printmemlist;
  readln;
end.
