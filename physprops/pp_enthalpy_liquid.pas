unit PP_enthalpy_liquid;

{$mode objfpc}{$H+}

interface

uses
  util,
  PP_coefficients;

(**
 * Calculate the liquid enthalpy from temperature for a pure component.
 *
 * \param name: the name of the pure component
 * \param T: the temperature in degrees Celcius
 * \return the calculated liquid enhtalpy in kj/kg.
 *)
function PP_enthalpy_liq_calculate(name : string;
                                   T    : double): double;

(**
 * Register a new model to calculate the liquid enthalpy from temperature for a pure component.
 *
 * \param name: the name of the pure component
 * \param range: the temperature range the model is valid for in degrees Celcius
 * \param coeff: the model coefficients
 * \param method: the name of the model, internally the correct callback function is then found
 * \param ref: the reference of the model parameters
 *)
procedure PP_enthalpy_liq_register(name        : string;
                                   range       : PPRange;
                                   coeff       : PPCoefficients;
                                   method, ref : string);

(**
 * Load a number of liquid enthalpy models in memory.
 *
 * \param fname: the filename of the list of models
 *)
procedure PP_enthalpy_liq_load(fname : string);

(**
 * Free all allocated data
 *
 * \param vm: the first liquid enthalpy model from the list
 *)
procedure PP_enthalpy_liq_free(vm : pPPModel);

implementation

uses
  math, SysUtils;

//different calculation models
function polynomial_enthalpy_liquid(T     : double;
                                    coeff : PPCoefficients) : double;
var
  temp : double = 0;
  i    : integer;
begin
  for i := 0 to coeff.totcoeff - 1 do
  begin
    temp += coeff.data[i] * power(T, i);
  end;

  exit(temp);
end;

procedure PP_enthalpy_liq_register(name        : string;
                                   range       : PPRange;
                                   coeff       : PPCoefficients;
                                   method, ref : string);
var
  enth : pPPModel;
begin
  enth := callocN(sizeof(PPModel), 'enthalpy liquid callback ');

  enth^.name := name;

  //copy the contents from the coefficient record
  move(coeff, enth^.coeff, sizeof(coeff));

  enth^.method := method;
  enth^.reference := ref;

  //determine which callback function to use
  case lowercase(method) of
    'polynomial': enth^.callback := @polynomial_enthalpy_liquid;

    else
      PP_error('unknown liquid enthalpy method', []);
  end;

  enth^.range := range;

  addtail(pp_model^.enthalpy_liq, enth);
end;

function PP_enthalpy_liq_calculate(name : string;
                                   T    : double): double;
var
  enth : pPPModel;
begin
  enth := pPPModel(pp_model^.enthalpy_liq^.first);

  while enth <> nil do
  begin
    //check component name
    if enth^.name = name then

      //check if temperature within limits
      if (enth^.range.min <= T) and (enth^.range.max >= T) then
        exit(pp_T_callback(enth^.callback)(T, enth^.coeff));

    enth := enth^.id.next;
  end;
end;

//this procedure expect a simple text file format, later it can be changed
//for either some sort of python script or a database format
procedure PP_enthalpy_liq_load(fname : string);

  function read_line_from_file(var f : Text): string;
  var
    s: string;
  begin
    s := '';

    //ignore comments and empty lines
    while (s = '') or (s[1] = '#') do
      readln(f, s);

    exit(s);
  end;

var
  f          : Text;
  comp_name  : string;
  model_name : string;
  ref        : string;
  coeff      : PPCoefficients;
  range      : PPRange;
  i          : integer;
begin
  AssignFile(f, fname);

  try
    //open the file for reading
    reset(f);

    //keep reading lines until the end of the file is reached
    while not eof(f) do
    begin
      //component name
      comp_name := read_line_from_file(f);
      //model name
      model_name := read_line_from_file(f);
      // reference
      ref := read_line_from_file(f);
      //temp range min
      range.min := StrToFloat(read_line_from_file(f));
      //temp range max
      range.max := StrToFloat(read_line_from_file(f));
      //number of coefficients
      coeff.totcoeff := StrToInt(read_line_from_file(f));
      //coefficients
      coeff.data := callocN(coeff.totcoeff * sizeof(double), pchar('coefficient data ' + comp_name));
      for i := 0 to coeff.totcoeff - 1 do
        coeff.data[i] := StrToFloat(read_line_from_file(f));

      //register the component
      PP_enthalpy_liq_register(comp_name, range, coeff, model_name, ref);
    end;

    //close the file
    CloseFile(f);

  except
    PP_error('File handling error occurred.', []);
  end;
end;

procedure PP_enthalpy_liq_free(vm: pPPModel);
var
  link : pPPModel;
begin
  link := pPPModel(vm);

  //free all coefficients from memory
  while link <> nil do
  begin
    freeN(link^.coeff.data);
    link := link^.id.next;
  end;
end;

end.
