unit PP_vapor_pressure;

{$mode objfpc}{$H+}

interface

uses
  util,
  PP_coefficients;

type
  pvap_func = function(T     : double;
                       coeff : PPCoefficients): double;

  pPPVapPressModel = ^PPVapPressModel;
  PPVapPressModel = record
    id         : ID;

    name       : string;
    temp_range : PPRange;
    coeff      : PPCoefficients;
    method     : string;
    reference  : string;
    callback   : pvap_func;
  end;

(**
 * Calculate the vapor pressure from temperature for a pure component.
 *
 * \param name: the name of the pure component
 * \param T: the temperature in degrees Celcius
 * \return the calculated vapor pressure in mmHg.
 *)
function PP_vapor_pressure_calculate(name : string;
                                     T    : double): double;

(**
 * Register a new model to calculate the vapor pressure from temperature for a pure component.
 *
 * \param name: the name of the pure component
 * \param range: the temperature range the model is valid for in degrees Celcius
 * \param coeff: the model coefficients
 * \param method: the name of the model, internally the correct callback function is then found
 * \param ref: the reference of the model parameters
 *)
procedure PP_vapor_pressure_register(name        : string;
                                     range       : PPRange;
                                     coeff       : PPCoefficients;
                                     method, ref : string);

(**
 * Load a number of vapor pressure models in memory.
 *
 * \param fname: the filename of the list of models
 *)
procedure PP_vapor_pressure_load(fname : string);

(**
 * Free all vapor pressure models in memory.
 *
 * \param vm: the first vapor pressure model from the list
 *)
procedure PP_vapor_pressure_free(vm : pListBase);

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
  pvap : pPPVapPressModel;
begin
  pvap := callocN(sizeof(PPVapPressModel), 'register vapor pressure callback ');

  pvap^.name := name;

  //copy the contents from the coefficient record
  //pvap^.coeff.data := callocN(coeff.totcoeff * sizeof(double), 'vapor pressure coefficient data');
  move(coeff, pvap^.coeff, sizeof(coeff));

  pvap^.method := method;
  pvap^.reference := ref;

  //determine which callback function to use
  case lowercase(method) of
    'antoine': pvap^.callback := @antoine_vapor_pressure;
    else
      PP_error('unknown vapor pressure method', []);
  end;

  pvap^.temp_range := range;

  addtail(PPModel^.vapor_pressure, pvap);
end;

function PP_vapor_pressure_calculate(name : string;
                                     T    : double): double;
var
  pvap : pPPVapPressModel;
begin
  pvap := pPPVapPressModel(PPModel^.vapor_pressure^.first);

  while pvap <> nil do
  begin
    //check component name
    if pvap^.name = name then

      //check if temperature within limits
      if (pvap^.temp_range.min <= T) and (pvap^.temp_range.max >= T) then
        exit(pvap^.callback(T, pvap^.coeff));

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
      coeff.data := callocN(coeff.totcoeff * sizeof(double), 'coefficient data');
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

procedure PP_vapor_pressure_free(vm : pListBase);
var
  link : pPPVapPressModel;
begin
  link := pPPVapPressModel(vm);

  //free all coefficients from memory
  while link <> nil do
  begin
    freeN(link^.coeff.data);
    link := link^.id.next;
  end;
end;

end.
