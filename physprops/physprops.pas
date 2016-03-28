unit PhysProps;

{$mode objfpc}{$H+}

interface

uses
  util;

type
  ID = record
    next, prev : pointer;
  end;

  PPCoefficients = record
    data     : pdouble;
    totcoeff : integer;
  end;

  pp_T_callback = function(T     : double;
                           coeff : PPCoefficients): double;

  PPRange = record
    min : double;
    max : double;
  end;

  pPPModel = ^PPModel;
  PPModel = record
    id        : ID;

    name      : string;
    range     : PPRange;
    coeff     : PPCoefficients;
    method    : string;
    reference : string;
    callback  : pointer;
  end;

  PPModelData = record
    enthalpy_liq     : pListBase;
    vapor_pressure   : pListBase;
    //conductivity_liq : pListBase;
    //conductivity_vap : pListBase;
    //density_liq      : pListBase;
    //density_vap      : pListBase;
    //enthalpy_vap     : pListBase;
    //molweight        : pointer;
    //surface_tension  : pListBase;
    //viscosity_liq    : pListBase;
    //viscosity_vap    : pListBase;
  end;
  pPPModelData = ^PPModelData;

var
  pp_model: pPPModelData;

{
  Display any error from the physical properties library

  @param(fmt is the formatting string for the error)
  @param(args is an array of arguments)
}
procedure PP_error(const fmt  : string;
                   const args : array of Const);

{
  Initialize the physical properties library
}
procedure PP_initialize;

{
  Free all data from the physical properties library
}
procedure PP_free;

implementation

uses
  SysUtils,
  PP_vapor_pressure,
  PP_enthalpy_liquid;

procedure PP_error(const fmt  : string;
                   const args : array of Const);
begin
  writeln(Format(fmt, args));
end;

procedure PP_initialize;
begin
  pp_model := callocN(sizeof(PPModelData), 'pp_model');

  pp_model^.vapor_pressure := callocN(sizeof(ListBase), 'vapor_pressure');
  pp_model^.enthalpy_liq := callocN(sizeof(ListBase), 'liquid_enthalpy');
end;

procedure PP_free;
begin
  PP_vapor_pressure_free(pp_model^.vapor_pressure^.first);
  PP_enthalpy_liq_free(pp_model^.enthalpy_liq^.first);

  //free all models from memory
  freelistN(pp_model^.vapor_pressure);
  freelistN(pp_model^.enthalpy_liq);

  //free the model list variables
  freeN(pp_model^.vapor_pressure);
  freeN(pp_model^.enthalpy_liq);

  freeN(pp_model);
end;

end.

