program unit_conv_test;

uses
  LinkedList, unit_conv;

var
  convert_from : pUnitConversion;
  convert_to   : pUnitConversion;

begin
  writeln('10000 kg = ', UNC_convert_unit(10000, 'kg', 'ton', False): 0: 2, ' ton');
  writeln('   1 ton = ', UNC_convert_unit(1, 'ton', 'kg', False): 0: 2, ' kg');
  writeln;
  writeln('100 degC = ', UNC_convert_unit(100, 'degC', 'degF', False): 0: 2, ' degF');
  writeln('     0 K = ', UNC_convert_unit(0, 'K', 'degC', False): 0: 2, ' degC');
  writeln;

  //Much faster way to convert units, lookup is not needed anymore
  //the converter structs are looked up once and can then be refrerenced by a
  //pointer. This can be done for each available value in the model.
  //Via the parentquant variable the parent quantities can be looked up
  //possible we could cache this as well, but we can decide later on for
  //performance issues to do so in the GUI
  convert_from := UNC_find_conversion(nil, 'degC');
  convert_to := UNC_find_conversion(nil, 'degF');
  writeln(' 38 degC = ', UNC_convert_unit(38, convert_from, convert_to, False): 0: 2, ' degF');

  UNC_free;
end.
