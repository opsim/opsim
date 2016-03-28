unit PP_vapor_pressure;

{$mode objfpc}{$H+}

interface

uses
  util,
  PhysProps;

{
  Calculate the vapor pressure from temperature for a pure component.

  @param(name is the name of the pure component)
  @param(T is the temperature in degrees Celcius)
  @returns(the calculated vapor pressure in mmHg)
}
function PP_vapor_pressure_calculate(name : string;
                                     T    : double): double;

{
  Register a new model to calculate the vapor pressure from temperature for a pure component.

  @param(name is the name of the pure component)
  @param(range is the temperature range the model is valid for in degrees Celcius)
  @param(coeff is the model coefficients)
  @param(method is the name of the model, internally the correct callback function is then found)
  @param(ref is the reference of the model parameters)
}
procedure PP_vapor_pressure_register(name        : string;
                                     range       : PPRange;
                                     coeff       : PPCoefficients;
                                     method, ref : string);

{
  Load a number of vapor pressure models in memory.

  @param(fname is the filename of the list of models)
}
procedure PP_vapor_pressure_load(fname : string);

{
  Free all allocated data

  @param(vm is the first vapor pressure model from the list)
}
procedure PP_vapor_pressure_free(vm : pPPModel);

implementation

uses
  math, SysUtils;

//different calculation models
function antoine_vapor_pressure(T     : double;
                                coeff : PPCoefficients) : double;
begin
  exit(power(10, coeff.data[0] - coeff.data[1] / (T + coeff.data[2])));
end;

function wrede_vapor_pressure(T     : double;
                              coeff : PPCoefficients) : double;
begin
  exit(power(10, coeff.data[0] - coeff.data[1] / T));
end;

procedure PP_vapor_pressure_register(name        : string;
                                     range       : PPRange;
                                     coeff       : PPCoefficients;
                                     method, ref : string);
var
  pvap : pPPModel;
begin
  pvap := callocN(sizeof(PPModel), 'vapor pressure callback ');

  pvap^.name := name;

  //copy the contents from the coefficient record
  move(coeff, pvap^.coeff, sizeof(coeff));

  pvap^.method := method;
  pvap^.reference := ref;

  //determine which callback function to use
  case lowercase(method) of
    'antoine': pvap^.callback := @antoine_vapor_pressure;
    else
      PP_error('unknown vapor pressure method', []);
  end;

  pvap^.range := range;

  addtail(pp_model^.vapor_pressure, pvap);
end;

function PP_vapor_pressure_calculate(name : string;
                                     T    : double): double;
var
  pvap : pPPModel;
begin
  pvap := pPPModel(pp_model^.vapor_pressure^.first);

  while pvap <> nil do
  begin
    //check component name
    if pvap^.name = name then

      //check if temperature within limits
      if (pvap^.range.min <= T) and (pvap^.range.max >= T) then
        exit(pp_T_callback(pvap^.callback)(T, pvap^.coeff));

    pvap := pvap^.id.next;
  end;
end;

//this procedure expect a simple text file format, later it can be changed
//for either some sort of python script or a database format
procedure PP_vapor_pressure_load(fname : string);

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
      PP_vapor_pressure_register(comp_name, range, coeff, model_name, ref);
    end;

    //close the file
    CloseFile(f);

  except
    PP_error('File handling error occurred.', []);
  end;
end;

procedure PP_vapor_pressure_free(vm: pPPModel);
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
